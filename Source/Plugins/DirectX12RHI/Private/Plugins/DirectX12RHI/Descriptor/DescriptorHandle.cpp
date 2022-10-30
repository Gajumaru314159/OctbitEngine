//***********************************************************
//! @file
//! @brief		デスクリプタ・ハンドル
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "DescriptorHandle.h"
#include "DescriptorHeap.h"

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
	DescriptorHandle::DescriptorHandle(DescriptorHandle&& rhs) {
		m_pBlock = rhs.m_pBlock;
		rhs.m_pBlock = nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ムーブ代入演算子
	//@―---------------------------------------------------------------------------
	DescriptorHandle& DescriptorHandle::operator = (DescriptorHandle&& rhs) {
		if (this != &rhs) {
			release();
			m_pBlock = rhs.m_pBlock;
			rhs.m_pBlock = nullptr;
		}
		return *this;
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
		if (m_pBlock)m_pBlock->pHeap->releaseHandle(*this);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  タイプを取得
	//@―---------------------------------------------------------------------------
	DescriptorHeapType DescriptorHandle::getHeapType()const {
		OB_CHECK_ASSERT(m_pBlock, "空のハンドルです。");
		return m_pBlock->pHeap->getHeapType();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  CPUハンドルを取得
	//@―---------------------------------------------------------------------------
	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle::getCpuHandle(s32 index)const {
		OB_CHECK_ASSERT(m_pBlock, "空のハンドルです。");
		return m_pBlock->pHeap->getCpuHandle(m_pBlock->index + index);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  GPUハンドルを取得
	//@―---------------------------------------------------------------------------
	D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHandle::getGpuHandle(s32 index)const {
		OB_CHECK_ASSERT(m_pBlock, "空のハンドルです。");
		return m_pBlock->pHeap->getGpuHandle(m_pBlock->index + index);
	}

}// namespcae ob::rhi::dx12