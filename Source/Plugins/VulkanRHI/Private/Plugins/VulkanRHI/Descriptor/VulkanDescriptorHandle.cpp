//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Plugins/VulkanRHI/Descriptor/VulkanDescriptorHandle.h>
#include <Plugins/VulkanRHI/Descriptor/VulkanDescriptorHeap.h>

namespace ob::rhi {

	//! @brief  コンストラクタ
	VulkanDescriptorHandle::VulkanDescriptorHandle()
		:m_pBlock(nullptr)
	{

	}


	//! @brief  デストラクタ
	VulkanDescriptorHandle::~VulkanDescriptorHandle() {
		release();
	}


	//! @brief  ムーブコンストラクタ
	VulkanDescriptorHandle::VulkanDescriptorHandle(VulkanDescriptorHandle&& rhs)noexcept {
		m_pBlock = rhs.m_pBlock;
		rhs.m_pBlock = nullptr;
	}


	//! @brief  ムーブ代入演算子
	VulkanDescriptorHandle& VulkanDescriptorHandle::operator = (VulkanDescriptorHandle&& rhs)noexcept {
		if (this != &rhs) {
			release();
			m_pBlock = rhs.m_pBlock;
			rhs.m_pBlock = nullptr;
		}
		return *this;
	}

	//! @brief  有効なハンドルか
	VulkanDescriptorHandle::operator bool()const noexcept {
		return !empty();
	}

	//! @brief  空か
	bool VulkanDescriptorHandle::empty()const noexcept {
		return !m_pBlock;
	}


	//! @brief  解放
	void VulkanDescriptorHandle::release() {
		if (m_pBlock)static_cast<VulkanDescriptorHeap*>(m_pBlock->pUser)->releaseHandle(*this);
		m_pBlock = nullptr;
	}


	//! @brief  バインドレスハンドルに使用するインデックスを取得
	u32 VulkanDescriptorHandle::getBindlessIndex(s32 index)const {
		return m_pBlock->index + index;
	}

}