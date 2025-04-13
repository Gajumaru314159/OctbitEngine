//***********************************************************
//! @file
//! @brief		デスクリプタ・ヒープ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "DescriptorHeap.h"
#include <Framework/Core/Math/BitOp.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/Descriptor/DescriptorHandle.h>

namespace ob::rhi::vulkan {




	//! @brief          コンストラクタ
	//! 
	//! @param device   デバイス
	//! @param type     アロケート・タイプ
	//! @param capacity 容量
	DescriptorHeap::DescriptorHeap(VulkanRHI& rhi, vk::DescriptorType type, s32 capacity)
		: m_type(type)
	{
		auto& device = rhi.getDevice();

		// Uniform Buffer用のディスクリプタプールサイズを設定
		vk::DescriptorPoolSize uniformBufferPoolSize;
		uniformBufferPoolSize.type = vk::DescriptorType::eUniformBuffer;
		uniformBufferPoolSize.descriptorCount = 100; // 必要な数に余裕を持たせる

		// Combined Image Sampler用のディスクリプタプールサイズを設定
		vk::DescriptorPoolSize combinedImageSamplerPoolSize;
		combinedImageSamplerPoolSize.type = vk::DescriptorType::eCombinedImageSampler;
		combinedImageSamplerPoolSize.descriptorCount = 50; // 必要な数に余裕を持たせる

		// プールサイズを配列にまとめる
		std::array<vk::DescriptorPoolSize, 2> poolSizes = { uniformBufferPoolSize, combinedImageSamplerPoolSize };

		// DescriptorPoolの作成
		vk::DescriptorPoolCreateInfo poolInfo;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = 150; // 最大セット数

		m_pool = device.createDescriptorPool(poolInfo, rhi.getAllocationCallbacks());

	}


	//! @brief          デストラクタ
	DescriptorHeap::~DescriptorHeap() {
	}


	//! @brief          ハンドルをアロケート
	//! 
	//! @param handle   アロケート先ハンドル
	//! @param viewNum  割り当て個数
	void DescriptorHeap::allocateHandle(class DescriptorHandle& handle, s32 size) {

		std::vector<vk::DescriptorSetLayout> layouts(100, *descriptorSetLayout);
		vk::DescriptorSetAllocateInfo allocInfo;
		allocInfo.descriptorPool = *descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(layouts.size());
		allocInfo.pSetLayouts = layouts.data();

		m_pool.getDevice().allocateDescriptorSets(allocInfo);
	}


	//! @brief          ハンドルを解放
	void DescriptorHeap::releaseHandle(class DescriptorHandle& handle) {

		ScopeLock lock(m_mutex);
		m_mapper.free(handle.m_pBlock);
		handle.m_pBlock = nullptr;

	}



	//! @brief  タイプを取得
	DescriptorHeapType DescriptorHeap::getHeapType()const {
		return m_type;
	}

}