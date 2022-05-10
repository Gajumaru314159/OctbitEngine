//***********************************************************
//! @file
//! @brief		デスクリプタ・ヒープ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "DescriptorHeap.h"
#include <Framework/Core/Math/BitOp.h>
#include <Plugins/GraphicDirectX12/Device/DeviceImpl.h>
#include <Plugins/GraphicDirectX12/Utility/TypeConverter.h>
#include <Plugins/GraphicDirectX12/Utility/Utility.h>
#include <Plugins/GraphicDirectX12/Descriptor/DescriptorHandle.h>

namespace ob::graphic::dx12 {

	//===============================================================
	// 0b0011'1001'0101'0000
	//     +--------|			FirstLevel			7
	//		+-+					SecondLevel			11(2) = 3
	//		         |-----|	LinearManagement	下位6ビット
	// 
	// 0b0000'0000'0101'0000
	//              |			FirstLevel			0
	//		         ++			SecondLevel			01(2) = 1
	//		         |-----|	LinearManagement	下位6ビット
	// 
	// 0b0000'0000'0000'1010
	//              |			FirstLevel			0
	//		         ++			SecondLevel			00(2) = 0
	//		         |-----|	LinearManagement	下位6ビット
	//===============================================================
	s32 DescriptorHeap::s_maxSecondLevelLog2 = 2;
	s32 DescriptorHeap::s_linearManagementSizeLog2 = 6;

	s32 DescriptorHeap::s_maxSecondLevel = 1 << DescriptorHeap::s_maxSecondLevelLog2;		// 4
	s32 DescriptorHeap::s_linearManagementSize = 1 << DescriptorHeap::s_linearManagementSizeLog2;	// 64

	s32 DescriptorHeap::s_secondLevelShift = DescriptorHeap::s_linearManagementSizeLog2 - DescriptorHeap::s_maxSecondLevelLog2;	// 4

	//@―---------------------------------------------------------------------------
	//! @brief          コンストラクタ
	//! 
	//! @param device   デバイス
	//! @param type     アロケート・タイプ
	//! @param capacity 容量
	//@―---------------------------------------------------------------------------
	DescriptorHeap::DescriptorHeap(DeviceImpl& device, DescriptorHeapType type, s32 capacity)
		: m_capacity(0)
		, m_type(type)
	{
		capacity = get_max(capacity, s_linearManagementSize);
		{
			D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
			descHeapDesc.Type = TypeConverter::convert(type);
			descHeapDesc.NumDescriptors = (UINT)capacity;
			descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			descHeapDesc.NodeMask = 0;
			if (descHeapDesc.Type == D3D12_DESCRIPTOR_HEAP_TYPE_RTV ||
				descHeapDesc.Type == D3D12_DESCRIPTOR_HEAP_TYPE_DSV) {
				descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			}

			m_descriptorSize = device.getNativeDevice()->GetDescriptorHandleIncrementSize(descHeapDesc.Type);

			HRESULT result;
			result = device.getNativeDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(m_heap.ReleaseAndGetAddressOf()));
			if (FAILED(result)) {
				Utility::outputFatalLog(result, TC("ID3D12Device::CreateDescriptorHeap()"));
				return;
			}
		}
		m_capacity = capacity;
		m_buffer.resize(m_capacity);

		// 必要な容量を計算
		s32 firstLevel, secondLevel, blockIndex;
		getLevelAndIndex(m_capacity, firstLevel, secondLevel, blockIndex);

		// アロケート
		m_freeFLI = 0;
		m_freeSLI.resize(firstLevel+1, 0);
		m_blocks.resize(blockIndex+1, nullptr);

		// フリーリスト確保
		m_freeList.reserve(capacity);
		for (auto& block : m_buffer) {
			m_freeList.push_back(&block);
		}

		// 初回ブロックを生成
		{
			auto pBlock = m_freeList.back();
			m_freeList.pop_back();
			pBlock->pHeap = this;
			pBlock->allocated = false;
			pBlock->capacity = m_capacity;
			pBlock->index = 0;
			pBlock->pPrev = pBlock->pFreePrev = nullptr;
			pBlock->pNext = pBlock->pFreeNext = nullptr;
			addFreeBlock(*pBlock);
		}

	}


	//@―---------------------------------------------------------------------------
	//! @brief          デストラクタ
	//@―---------------------------------------------------------------------------
	DescriptorHeap::~DescriptorHeap() {
		// m_blocks の最上位に1つ残っているのが正常
		OB_CHECK_ASSERT(m_freeList.size()+1 == m_capacity, "未開放のDescriptorHandleがあります。");
	}


	//@―---------------------------------------------------------------------------
	//! @brief          ハンドルをアロケート
	//! 
	//! @param handle   アロケート先ハンドル
	//! @param viewNum  割り当て個数
	//@―---------------------------------------------------------------------------
	void DescriptorHeap::allocateHandle(class DescriptorHandle& handle, s32 size) {

		handle.release();
		if (size <= 0)return;

		lock_guard_type lock(m_mutex);

		BBlock* pBlock = allocateFreeBlock(size);
		if (pBlock == nullptr) {
			LOG_FATAL_EX("Graphic", "DescriptorHandleのアロケートに失敗しました。[size={}]", size);
			return;
		}

		handle.m_pBlock = pBlock;

	}


	//@―---------------------------------------------------------------------------
	//! @brief          ハンドルを解放
	//@―---------------------------------------------------------------------------
	void DescriptorHeap::releaseHandle(class DescriptorHandle& handle) {

		auto pFreeBlock = handle.m_pBlock;
		if (pFreeBlock == nullptr)return;

		lock_guard_type lock(m_mutex);

		pFreeBlock->allocated = false;

		// 前ブロックがフリーブロックなら結合
		if (pFreeBlock->pPrev) {
			if (!pFreeBlock->pPrev->allocated) {
				pFreeBlock = mergeFreeBlocks(*pFreeBlock->pPrev, *pFreeBlock);
			}
		}

		// 後ブロックがフリーなら結合
		if (pFreeBlock->pNext) {
			if (!pFreeBlock->pNext->allocated) {
				pFreeBlock = mergeFreeBlocks(*pFreeBlock, *pFreeBlock->pNext);
			}
		}

		// フリーブロックリストに追加
		addFreeBlock(*pFreeBlock);

	}


	//@―---------------------------------------------------------------------------
	//! @brief          CPUハンドルを取得
	//@―---------------------------------------------------------------------------
	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::getCpuHandle(u32 index) {
		OB_ASSERT_RANGE(index, 0, m_capacity);
		OB_CHECK_ASSERT(m_heap,"ヒープが空です。");
		D3D12_CPU_DESCRIPTOR_HANDLE handle = m_heap->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += index * m_descriptorSize;
		return handle;
	}


	//@―---------------------------------------------------------------------------
	//! @brief          CPUハンドルを取得
	//@―---------------------------------------------------------------------------
	D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::getGpuHandle(u32 index) {
		OB_ASSERT_RANGE(index, 0, m_capacity);
		OB_CHECK_ASSERT(m_heap, "ヒープが空です。");
		D3D12_GPU_DESCRIPTOR_HANDLE handle = m_heap->GetGPUDescriptorHandleForHeapStart();
		handle.ptr += index * m_descriptorSize;
		return handle;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  タイプを取得
	//@―---------------------------------------------------------------------------
	DescriptorHeapType DescriptorHeap::getHeapType()const {
		return m_type;
	}


	//@―---------------------------------------------------------------------------
	//! @brief          フリーブロックを確保
	//! 
	//! @details		内部で firstLevel と secondLevel が変更されます。
	//@―---------------------------------------------------------------------------
	BBlock* DescriptorHeap::allocateFreeBlock(s32 size) {

		s32 firstLevel, secondLevel;
		getLevelIndex(size, firstLevel, secondLevel);

		// 最大第1レベルを超えたので失敗
		if (m_freeSLI.size() <= firstLevel)return nullptr;

		// 割り当て可能なブロックがあるかチェック
		u32 slMap = m_freeSLI[firstLevel] & (~0U << secondLevel);
		if (!slMap) {
			u32 flMap = m_freeFLI & (~0U << (firstLevel + 1));
			if (!flMap) {
				// 割り当て可能なブロックがない
				return nullptr;
			}

			// ビットマスクから割り当て可能なレベルを再計算
			firstLevel = BitOp::getLSB(flMap);
			slMap = m_freeSLI[firstLevel];
			OB_CHECK_ASSERT(slMap, "内部エラー。割り当て可能なブロックがない状態で処理が継続されています。ロジックを見直してください。");
			secondLevel = BitOp::getLSB((u32)slMap);
		}

		s32 blockIndex = getFreeBlockIndex(firstLevel, secondLevel);
		auto pBlock = m_blocks.at(blockIndex);
		OB_CHECK_ASSERT(pBlock, "内部エラー。allocateFreeBlock()にバグがあります。");
		OB_CHECK_ASSERT(size <= pBlock->capacity, "内部エラー。allocateFreeBlock()にバグがあります。");

		separateFreeList(*pBlock);
		returnSurplusBlock(*pBlock, size);

		return pBlock;

	}


	//@―---------------------------------------------------------------------------
	//! @brief	ブロックを必要分だけ切り出して残りをフリーリストに戻す。
	//@―---------------------------------------------------------------------------
	void DescriptorHeap::returnSurplusBlock(BBlock& block, s32 size) {

		OB_CHECK_ASSERT(size, "サイズ0で分割できません。");
		OB_CHECK_ASSERT(!block.allocated, "アロケート済みのブロックは分割できません。");
		OB_CHECK_ASSERT(!block.pFreePrev, "フリーブロックに接続されています。");
		OB_CHECK_ASSERT(!block.pFreeNext, "フリーブロックに接続されています。");

		if (block.capacity == size)return;

		OB_CHECK_ASSERT(!m_freeList.empty(), "DescriptorHeaoの容量が不足しています。");

		auto pRemain = m_freeList.back();
		m_freeList.pop_back();
		pRemain->pHeap = this;
		pRemain->index = block.index + size;
		pRemain->capacity = block.capacity - size;
		pRemain->allocated = false;
		pRemain->pPrev = &block;
		pRemain->pNext = block.pNext;
		pRemain->pFreePrev = pRemain->pFreeNext = nullptr;
		addFreeBlock(*pRemain);

		if(block.pNext)block.pNext->pPrev = pRemain;
		block.pNext = pRemain;
		block.capacity = size;
		block.allocated = true;

	}


	//@―---------------------------------------------------------------------------
	//! @brief	ブロックをフリーリストから分離
	//@―---------------------------------------------------------------------------
	void DescriptorHeap::separateFreeList(BBlock& block) {

		OB_CHECK_ASSERT(!block.allocated, "フリーリストにないブロックは分離できません。");

		// フリーリストの前後をつなげる
		auto pFreePrev = block.pFreePrev;
		auto pFreeNext = block.pFreeNext;
		if (pFreeNext)pFreeNext->pFreePrev = pFreePrev;
		if (pFreePrev)pFreePrev->pFreeNext = pFreeNext;

		// 自分はリセット
		block.pFreePrev = nullptr;
		block.pFreeNext = nullptr;

		// フリーリストの先頭の場合は先頭を入れなおす
		s32 firstIndex, secondIndex, blockIndex;
		getLevelAndIndex(block.capacity, firstIndex, secondIndex, blockIndex);
		if (m_blocks.at(blockIndex) == &block) {
			m_blocks.at(blockIndex) = pFreeNext;
		}

		// ビット更新
		removeFreeListBitState(firstIndex, secondIndex);

	}


	//@―---------------------------------------------------------------------------
	//! @brief		2つのブロックをマージしてフリーブロックから取り出す
	//! 
	//! @details	結合後に block2 はフリーリストに返却されます。
	//! @return		結合後のブロックのポインタ
	//@―---------------------------------------------------------------------------
	BBlock* DescriptorHeap::mergeFreeBlocks(BBlock& block1, BBlock& block2) {

		OB_CHECK_ASSERT(!block1.allocated, "アロケート済みのブロックはマージできません。");
		OB_CHECK_ASSERT(!block2.allocated, "アロケート済みのブロックはマージできません。");
		OB_CHECK_ASSERT(block1.pNext == &block2 && &block1 == block2.pPrev, "連続していないブロックはマージできません。");

		separateFreeList(block1);
		separateFreeList(block2);

		// block1を拡張
		block1.capacity += block2.capacity;

		// 再連結
		block1.pNext = block2.pNext;
		if (block2.pNext) {
			block2.pNext->pPrev = &block1;
		}

		// リセット
		block2.capacity = 0;
		block2.index = 0;
		block2.pPrev = block2.pNext = nullptr;

		m_freeList.push_back(&block2);

		return &block1;

	}


	//@―---------------------------------------------------------------------------
	//! @brief	フリーブロックをカテゴリに追加
	//@―---------------------------------------------------------------------------
	void DescriptorHeap::addFreeBlock(BBlock& block) {

		OB_CHECK_ASSERT(!block.allocated, "アロケート済みのブロックはフリーリストに追加できません。");
		OB_CHECK_ASSERT(!block.pFreePrev && !block.pFreeNext, "連結されているブロックはフリーリストに追加できません。");

		s32 firstLevel, secondLevel, blockIndex;
		getLevelAndIndex(block.capacity, firstLevel, secondLevel, blockIndex);
		auto& pTop = m_blocks.at(blockIndex);
		if (pTop) {
			block.pFreeNext = pTop;
			pTop->pFreePrev = &block;
		}
		pTop = &block;

		entryFreeListBitState(firstLevel, secondLevel);

	}


	//@―---------------------------------------------------------------------------
	//! @brief	フリーリストビットへ登録
	//@―---------------------------------------------------------------------------
	void DescriptorHeap::entryFreeListBitState(s32 firstLevel, s32 secondLevel) {
		m_freeFLI |= 1U << firstLevel;
		m_freeSLI.at(firstLevel) |= 1U << secondLevel;
	}


	//@―---------------------------------------------------------------------------
	//! @brief	フリーリストビットから削除
	//@―---------------------------------------------------------------------------
	void DescriptorHeap::removeFreeListBitState(s32 firstLevel, s32 secondLevel) {
		m_freeSLI.at(firstLevel) &= ~(1U << secondLevel);
		if (m_freeSLI[firstLevel] == 0) {
			m_freeFLI &= ~(1U << firstLevel);
		}
	}


	//@―---------------------------------------------------------------------------
	//! @brief	レベルからフリーブロックリストのインデックスを計算
	//@―---------------------------------------------------------------------------
	s32 DescriptorHeap::getFreeBlockIndex(s32 firstLevel, s32 secondLevel)const noexcept {
		return firstLevel * s_maxSecondLevel + secondLevel;
	}


	//@―---------------------------------------------------------------------------
	//! @brief	サイズから各レベルのカテゴリを計算
	//@―---------------------------------------------------------------------------
	void DescriptorHeap::getLevelIndex(s32 size, s32& firstLevel, s32& secondLevel)const noexcept {
		if (size < s_linearManagementSize) {
			firstLevel = 0;
			secondLevel = size >> s_secondLevelShift;
		} else {
			firstLevel = get_max(0, BitOp::getMSB((u32)size) - s_linearManagementSizeLog2);
			secondLevel = size >> (firstLevel - s_maxSecondLevel);
			secondLevel &= (s_maxSecondLevel - 1);
		}
	}


	//@―---------------------------------------------------------------------------
	//! @brief	サイズからレベルとブロックインデックスを計算
	//@―---------------------------------------------------------------------------
	void DescriptorHeap::getLevelAndIndex(s32 size, s32& firstLevel, s32& secondLevel, s32& index)const noexcept {
		getLevelIndex(size, firstLevel, secondLevel);
		index = getFreeBlockIndex(firstLevel, secondLevel);
	}

}// namespcae ob::graphic::dx12