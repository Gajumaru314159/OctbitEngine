//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "VulkanDescriptorHeap.h"
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Descriptor/VulkanDescriptorHandle.h>

namespace ob::rhi {

	//! @brief          コンストラクタ
	//! 
	//! @param device   デバイス
	//! @param type     アロケート・タイプ
	//! @param capacity 容量
	VulkanDescriptorHeap::VulkanDescriptorHeap(VulkanRHI& device, s32 resourceCapacity, s32 samplerCapacity)
		: m_resourceMapper(std::min(samplerCapacity, 100))
		, m_samplerMapper(std::min(samplerCapacity,100))
	{
		// SamplerDescriptorHeap
		vk::DescriptorType samplerType[] = {
			vk::DescriptorType::eSampler,
		};
		// ResourceDescriptorHeap
		vk::DescriptorType resourceTypes[] = {
			vk::DescriptorType::eSampledImage,
			vk::DescriptorType::eStorageImage,
			vk::DescriptorType::eUniformTexelBuffer,
			vk::DescriptorType::eStorageTexelBuffer,
			vk::DescriptorType::eUniformBuffer,
			vk::DescriptorType::eStorageBuffer,
		};

		vk::MutableDescriptorTypeListEXT mutableList[2];
		mutableList[0].setDescriptorTypes(samplerType);
		mutableList[1].setDescriptorTypes(resourceTypes);

		vk::MutableDescriptorTypeCreateInfoEXT mutableInfo;
		mutableInfo.setMutableDescriptorTypeLists(mutableList);

		vk::DescriptorSetLayoutBinding bindings[2];
		bindings[0].binding = 0; // VulkanShaderの-fvk-bind-sampler-heapと合わせる
		bindings[0].descriptorType = vk::DescriptorType::eMutableEXT;
		bindings[0].descriptorCount = m_samplerMapper.capacity();
		bindings[0].stageFlags = vk::FlagTraits<vk::ShaderStageFlagBits>::allFlags;
		bindings[0].pImmutableSamplers = nullptr;
		bindings[1].binding = 1; // VulkanShaderの-fvk-bind-resource-heapと合わせる
		bindings[1].descriptorType = vk::DescriptorType::eMutableEXT;
		bindings[1].descriptorCount = m_resourceMapper.capacity();
		bindings[1].stageFlags = vk::FlagTraits<vk::ShaderStageFlagBits>::allFlags;
		bindings[1].pImmutableSamplers = nullptr;

		vk::DescriptorSetLayoutCreateInfo info;
		info.setBindings(bindings);
		info.setPNext(&mutableInfo);
		info.flags |= vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPoolEXT;

		vk::DescriptorSetLayoutSupport support = device.getDevice().getDescriptorSetLayoutSupport(info);
		if (!support.supported) {
			LOG_FATAL("Bindlessレンダリングをサポートしていないハードウェアです");
		}

		m_layout = device.getDevice().createDescriptorSetLayout(info, device.getAllocationCallbacks());

		// TODO set
		vk::DescriptorPoolSize poolSizes[2];
		poolSizes[0].type = vk::DescriptorType::eMutableEXT;
		poolSizes[0].descriptorCount = m_samplerMapper.capacity();
		poolSizes[1].type = vk::DescriptorType::eMutableEXT;
		poolSizes[1].descriptorCount = m_resourceMapper.capacity();

		vk::DescriptorPoolCreateInfo poolInfo;
		poolInfo.maxSets = 1;
		poolInfo.flags |= vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
		poolInfo.flags |= vk::DescriptorPoolCreateFlagBits::eUpdateAfterBindEXT; // Bindless用
		poolInfo.setPoolSizes(poolSizes);

		m_pool = device.getDevice().createDescriptorPool(poolInfo, device.getAllocationCallbacks());

		vk::DescriptorSetLayout descSetLayouts[] = { m_layout };
		vk::DescriptorSetAllocateInfo allocInfo;
		allocInfo.descriptorPool = m_pool;
		allocInfo.setSetLayouts(descSetLayouts);

		auto sets = device.getDevice().allocateDescriptorSets(allocInfo);
		m_set = std::move(sets.front());

		device.setName(m_pool, "BindlessPool");
		device.setName(m_layout, "BindlessLayout");
		device.setName(m_set, "BindlessSet");
	}


	//! @brief          デストラクタ
	VulkanDescriptorHeap::~VulkanDescriptorHeap() {
	}


	//! @brief          ハンドルをアロケート
	//! 
	//! @param handle   アロケート先ハンドル
	//! @param viewNum  割り当て個数
	void VulkanDescriptorHeap::allocateHandle(VulkanDescriptorHandle& handle, vk::WriteDescriptorSet& desc) {

		handle.release();

		const TLSFBlock* pBlock = nullptr;
		{
			// TODO フリーリスト化
			ScopeLock lock(m_mutex);
			if (desc.descriptorType == vk::DescriptorType::eSampler) pBlock = m_samplerMapper.allocate(1);
			if (desc.descriptorType != vk::DescriptorType::eSampler) pBlock = m_resourceMapper.allocate(1);
		}
		if (pBlock == nullptr) {
			LOG_FATAL_EX("Graphic", "DescriptorHandleのアロケートに失敗しました。[size={}]", 1);
			return;
		}
		const_cast<TLSFBlock*>(pBlock)->pUser = this;
		handle.m_pBlock = pBlock;

		desc.dstSet = m_set;
		desc.dstBinding = (desc.descriptorType == vk::DescriptorType::eSampler) ? 0 : 1;
		desc.dstArrayElement = handle.m_pBlock->index;
		desc.descriptorCount = 1;

		m_set.getDevice().updateDescriptorSets({ desc }, {});

	}


	//! @brief          ハンドルを解放
	void VulkanDescriptorHeap::releaseHandle(class VulkanDescriptorHandle& handle) {

		ScopeLock lock(m_mutex);
		if (m_resourceMapper.contains(handle.m_pBlock)) m_resourceMapper.free(handle.m_pBlock);
		if (m_samplerMapper.contains(handle.m_pBlock))  m_samplerMapper.free(handle.m_pBlock);
		handle.m_pBlock = nullptr;

	}


	//! @brief          デスクリプタセットをコマンドバッファに記録
	void VulkanDescriptorHeap::recordDescriptorHeap(vk::raii::CommandBuffer& commandBuffer, vk::PipelineLayout pipeline, s32 slot) {
		commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline, slot, *m_set, {});
	}

}