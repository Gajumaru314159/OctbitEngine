//***********************************************************
//! @file
//! @brief		デスクリプタ・ハンドル
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Plugins/DirectX12RHI/Descriptor/DescriptorHandle.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorHeap.h>

namespace ob::rhi::dx12 {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	DescriptorHandle::DescriptorHandle()
		:m_pBlock(nullptr)
	{

	}


	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	DescriptorHandle::~DescriptorHandle() {
		release();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ムーブコンストラクタ
	//@―---------------------------------------------------------------------------
	DescriptorHandle::DescriptorHandle(DescriptorHandle&& rhs)noexcept {
		m_pBlock = rhs.m_pBlock;
		rhs.m_pBlock = nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ムーブ代入演算子
	//@―---------------------------------------------------------------------------
	DescriptorHandle& DescriptorHandle::operator = (DescriptorHandle&& rhs)noexcept {
		if (this != &rhs) {
			release();
			m_pBlock = rhs.m_pBlock;
			rhs.m_pBlock = nullptr;
		}
		return *this;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  有効なハンドルか
	//@―---------------------------------------------------------------------------
	DescriptorHandle::operator bool()const noexcept {
		return !empty();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  空か
	//@―---------------------------------------------------------------------------
	bool DescriptorHandle::empty()const noexcept {
		return !m_pBlock;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  解放
	//@―---------------------------------------------------------------------------
	void DescriptorHandle::release() {
		if (m_pBlock)static_cast<DescriptorHeap*>(m_pBlock->pUser)->releaseHandle(*this);
		m_pBlock = nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  タイプを取得
	//@―---------------------------------------------------------------------------
	DescriptorHeapType DescriptorHandle::getHeapType()const {
		OB_ASSERT(m_pBlock, "空のハンドルです。");
		return static_cast<DescriptorHeap*>(m_pBlock->pUser)->getHeapType();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  CPUハンドルを取得
	//@―---------------------------------------------------------------------------
	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle::getCpuHandle(s32 index)const {
		OB_ASSERT(m_pBlock, "空のハンドルです。");
		return static_cast<DescriptorHeap*>(m_pBlock->pUser)->getCpuHandle(m_pBlock->index + index);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  GPUハンドルを取得
	//@―---------------------------------------------------------------------------
	D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHandle::getGpuHandle(s32 index)const {
		OB_ASSERT(m_pBlock, "空のハンドルです。");
		return static_cast<DescriptorHeap*>(m_pBlock->pUser)->getGpuHandle(m_pBlock->index + index);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  バインドレスハンドルに使用するインデックスを取得
	//@―---------------------------------------------------------------------------
	u32 DescriptorHandle::getBindlessIndex(s32 index)const {
		return m_pBlock->index + index;
	}

}