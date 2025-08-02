//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Allocator/Utility/TLSFMapper.h>
#include <Framework/Core/Log/Assertion.h>
#include <Framework/Core/Math/BitOp.h>

namespace ob::core {

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
	static constexpr s32 s_maxSecondLevelLog2 = 2;
	static constexpr s32 s_linearManagementSizeLog2 = 6;

	static constexpr s32 s_maxSecondLevel = 1 << s_maxSecondLevelLog2;		// 4
	static constexpr s32 s_linearManagementSize = 1 << s_linearManagementSizeLog2;	// 64

	static constexpr s32 s_secondLevelShift = s_linearManagementSizeLog2 - s_maxSecondLevelLog2;	// 4

	//! @brief          コンストラクタ
	//! 
	//! @param capacity 容量
	TLSFMapper::TLSFMapper(s32 capacity)
		: m_capacity(std::max(capacity, s_linearManagementSize))
	{
		
		m_buffer.resize(m_capacity);

		// 必要な容量を計算
		s32 firstLevel, secondLevel, blockIndex;
		GetLevelAndIndex(m_capacity, firstLevel, secondLevel, blockIndex);

		// アロケート
		m_freeFLI = 0;
		m_freeSLI.resize(static_cast<size_t>(firstLevel) + 1, 0);
		m_blocks.resize(static_cast<size_t>(blockIndex) + 1, nullptr);

		// フリーリスト確保
		m_freeList.reserve(capacity);
		for (auto& block : m_buffer) {
			m_freeList.push_back(&block);
		}

		// 初回ブロックを生成
		{
			auto pBlock = m_freeList.back();
			m_freeList.pop_back();
			pBlock->pUser = nullptr;
			pBlock->allocated = false;
			pBlock->capacity = m_capacity;
			pBlock->index = 0;
			pBlock->pPrev = pBlock->pFreePrev = nullptr;
			pBlock->pNext = pBlock->pFreeNext = nullptr;
			addFreeBlock(*pBlock);
		}

	}


	//! @brief          デストラクタ
	TLSFMapper::~TLSFMapper() {
		// m_blocks の最上位に1つ残っているのが正常
		Vector<TLSFBlock*> blocks;
		if (m_freeList.size() + 1 != m_capacity) {
			for (auto& item : m_buffer) {
				if (item.allocated) {
					blocks.push_back(&item);
				}
			}
		}
		OB_ASSERT(m_freeList.size() + 1 == m_capacity, "{}つの未開放のTLSFBlockがあります。RHI::finalize()の呼び出しを確認してください。",blocks.size());
	}


	//! @brief          ハンドルをアロケート
	//! @param size  割り当て個数
	auto TLSFMapper::allocate(s32 size) -> const TLSFBlock*{
		
		if (size <= 0)return nullptr;

		TLSFBlock* pBlock = allocateFreeBlock(size);
		if (pBlock == nullptr) {
			LOG_FATAL_EX("Graphic", "TLSFBlockのアロケートに失敗しました。[size={}]", size);
			return nullptr;
		}
#ifdef ENABLE_TLSF_STACK_TRACE
		pBlock->stack = StackTrace::Capture();
#endif
		return pBlock;
	}


	//! @brief          ハンドルを解放
	void TLSFMapper::free(const TLSFBlock* block) {

		auto pFreeBlock = const_cast<TLSFBlock*>(block);

		if (pFreeBlock == nullptr)return;

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


	//! @brief          キャパシティを取得
	bool TLSFMapper::contains(const TLSFBlock* block) const {
		auto distance = std::distance(m_buffer.data(), block);
		return 0 <= distance && distance < m_capacity;
	}


	//! @brief          フリーブロックを確保
	//! 
	//! @details		内部で firstLevel と secondLevel が変更されます。
	TLSFBlock* TLSFMapper::allocateFreeBlock(s32 size) {

		s32 firstLevel, secondLevel;
		GetLevelIndex(size, firstLevel, secondLevel);

		// 最大第1レベルを超えたので失敗
		if (m_freeSLI.size() <= firstLevel)return nullptr;

		// 割り当て可能なブロックがあるかチェック
		u32 slMap = m_freeSLI[firstLevel] & (~0U << secondLevel);
		if (!slMap) {
			// 上位レベルに空きが存在するか
			u32 flMap = m_freeFLI & (~0U << (firstLevel + 1));
			if (!flMap) {
				// 割り当て可能なブロックがない
				return nullptr;
			}

			// ビットマスクから割り当て可能なレベルを再計算
			firstLevel = BitOp::GetLSB(flMap);
			slMap = m_freeSLI[firstLevel];
			OB_ASSERT(slMap, "内部エラー。割り当て可能なブロックがない状態で処理が継続されています。ロジックを見直してください。");
			secondLevel = BitOp::GetLSB((u32)slMap);
		} else if (!(slMap & (1 << secondLevel))) {
			// SecondLevelの対象から検索
			secondLevel = BitOp::GetLSB((u32)slMap);
		}

		s32 blockIndex = GetFreeBlockIndex(firstLevel, secondLevel);
		auto pBlock = m_blocks.at(blockIndex);
		OB_ASSERT(pBlock, "内部エラー。allocateFreeBlock()にバグがあります。[pBlock==null]");
		OB_ASSERT(size <= pBlock->capacity, "内部エラー。allocateFreeBlock()にバグがあります。[{}<={}]", size, pBlock->capacity);
		separateFreeList(*pBlock);
		returnSurplusBlock(*pBlock, size);

		return pBlock;

	}


	//! @brief	ブロックを必要分だけ切り出して残りをフリーリストに戻す。
	void TLSFMapper::returnSurplusBlock(TLSFBlock& block, s32 size) {

		OB_ASSERT(size, "サイズ0で分割できません。");
		OB_ASSERT(!block.allocated, "アロケート済みのブロックは分割できません。");
		OB_ASSERT(!block.pFreePrev, "フリーブロックに接続されています。");
		OB_ASSERT(!block.pFreeNext, "フリーブロックに接続されています。");

		if (block.capacity == size) {
			block.allocated = true;
			return;
		}

		OB_ASSERT(!m_freeList.empty(), "TLSFMapperの容量が不足しています。");

		auto pRemain = m_freeList.back();
		m_freeList.pop_back();
		pRemain->index = block.index + size;
		pRemain->capacity = block.capacity - size;
		pRemain->allocated = false;
		pRemain->pPrev = &block;
		pRemain->pNext = block.pNext;
		pRemain->pFreePrev = pRemain->pFreeNext = nullptr;
		addFreeBlock(*pRemain);

		if (block.pNext)block.pNext->pPrev = pRemain;
		block.pNext = pRemain;
		block.capacity = size;
		block.allocated = true;

	}


	//! @brief	ブロックをフリーリストから分離
	void TLSFMapper::separateFreeList(TLSFBlock& block) {

		OB_ASSERT(!block.allocated, "フリーリストにないブロックは分離できません。");

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
		GetLevelAndIndex(block.capacity, firstIndex, secondIndex, blockIndex);
		if (m_blocks.at(blockIndex) == &block) {
			m_blocks.at(blockIndex) = pFreeNext;

			// カテゴリのフリーリストの先頭が空の場合はビット更新が必要
			if (pFreeNext == nullptr) {
				removeFreeListBitState(firstIndex, secondIndex);
			}
		}

	}


	//! @brief		2つのブロックをマージしてフリーブロックから取り出す
	//! 
	//! @details	結合後に block2 はフリーリストに返却されます。
	//! @return		結合後のブロックのポインタ
	TLSFBlock* TLSFMapper::mergeFreeBlocks(TLSFBlock& block1, TLSFBlock& block2) {

		OB_ASSERT(!block1.allocated, "アロケート済みのブロックはマージできません。");
		OB_ASSERT(!block2.allocated, "アロケート済みのブロックはマージできません。");
		OB_ASSERT(block1.pNext == &block2 && &block1 == block2.pPrev, "連続していないブロックはマージできません。");

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


	//! @brief	フリーブロックをカテゴリに追加
	void TLSFMapper::addFreeBlock(TLSFBlock& block) {

		OB_ASSERT(!block.allocated, "アロケート済みのブロックはフリーリストに追加できません。");
		OB_ASSERT(!block.pFreePrev && !block.pFreeNext, "連結されているブロックはフリーリストに追加できません。");

		s32 firstLevel, secondLevel, blockIndex;
		GetLevelAndIndex(block.capacity, firstLevel, secondLevel, blockIndex);
		auto& pTop = m_blocks.at(blockIndex);
		if (pTop) {
			block.pFreeNext = pTop;
			pTop->pFreePrev = &block;
		}
		pTop = &block;

		entryFreeListBitState(firstLevel, secondLevel);

	}


	//! @brief	フリーリストビットへ登録
	void TLSFMapper::entryFreeListBitState(s32 firstLevel, s32 secondLevel) {
		m_freeFLI |= 1U << firstLevel;
		m_freeSLI.at(firstLevel) |= 1U << secondLevel;
	}


	//! @brief	フリーリストビットから削除
	void TLSFMapper::removeFreeListBitState(s32 firstLevel, s32 secondLevel) {
		m_freeSLI.at(firstLevel) &= ~(1U << secondLevel);
		if (m_freeSLI[firstLevel] == 0) {
			m_freeFLI &= ~(1U << firstLevel);
		}
	}


	//! @brief	レベルからフリーブロックリストのインデックスを計算
	s32 TLSFMapper::GetFreeBlockIndex(s32 firstLevel, s32 secondLevel) noexcept {
		return firstLevel * s_maxSecondLevel + secondLevel;
	}


	//! @brief	サイズから各レベルのカテゴリを計算
	void TLSFMapper::GetLevelIndex(s32 size, s32& firstLevel, s32& secondLevel) noexcept {
		if (size < s_linearManagementSize) {
			firstLevel = 0;
			secondLevel = size >> s_secondLevelShift;
		} else {
			firstLevel = std::max(0, BitOp::GetMSB(static_cast<u32>(size)) + 1 - s_linearManagementSizeLog2);
			secondLevel = size >> (firstLevel + s_secondLevelShift);
			secondLevel &= (s_maxSecondLevel - 1);
		}
	}


	//! @brief	サイズからレベルとブロックインデックスを計算
	void TLSFMapper::GetLevelAndIndex(s32 size, s32& firstLevel, s32& secondLevel, s32& index) noexcept {
		GetLevelIndex(size, firstLevel, secondLevel);
		index = GetFreeBlockIndex(firstLevel, secondLevel);
	}

}