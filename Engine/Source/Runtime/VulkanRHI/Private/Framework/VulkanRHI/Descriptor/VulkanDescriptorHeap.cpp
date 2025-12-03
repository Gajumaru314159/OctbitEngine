//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include "VulkanDescriptorHeap.h"
#include <Framework/VulkanRHI/VulkanDevice.h>
#include <Framework/VulkanRHI/Descriptor/VulkanDescriptorHandle.h>

namespace ob::rhi {

	//! @brief          コンストラクタ
	VulkanDescriptorHeap::VulkanDescriptorHeap(VulkanDevice& device, s32 resourceCapacity, s32 samplerCapacity)
		: m_samplerMapper(samplerCapacity)
		, m_resourceMapper(resourceCapacity)
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

		// バインディングフラグの配列を作成
		vk::DescriptorBindingFlags bindingFlags[2] = {
			vk::DescriptorBindingFlags{} | vk::DescriptorBindingFlagBits::eUpdateAfterBind | vk::DescriptorBindingFlagBits::ePartiallyBound,
			vk::DescriptorBindingFlags{} | vk::DescriptorBindingFlagBits::eUpdateAfterBind | vk::DescriptorBindingFlagBits::ePartiallyBound,
		};

		// バインディングフラグ拡張構造体の作成
		vk::DescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsInfo;
		bindingFlagsInfo.setBindingFlags(bindingFlags);

		vk::MutableDescriptorTypeListEXT mutableList[2];
		mutableList[0].setDescriptorTypes(samplerType);
		mutableList[1].setDescriptorTypes(resourceTypes);

		vk::MutableDescriptorTypeCreateInfoEXT mutableInfo;
		mutableInfo.setMutableDescriptorTypeLists(mutableList);
		mutableInfo.setPNext(&bindingFlagsInfo);

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

		vk::DescriptorSetLayout descSetLayouts[] = { *m_layout };
		vk::DescriptorSetAllocateInfo allocInfo;
		allocInfo.descriptorPool = *m_pool;
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

		desc.dstSet = *m_set;
		desc.dstBinding = (desc.descriptorType == vk::DescriptorType::eSampler) ? 0 : 1;
		desc.dstArrayElement = handle.m_pBlock->index;
		desc.descriptorCount = 1;

		{
			ScopeLock lock(m_requestsLock);
			auto& request = m_requests.emplace_back();
			request = desc;

			auto& subrequest = m_subrequests.emplace_back();
			if (desc.pImageInfo) subrequest = *desc.pImageInfo;
			if (desc.pBufferInfo) subrequest = *desc.pBufferInfo;

		}


	}


	//! @brief          ハンドルを解放
	void VulkanDescriptorHeap::releaseHandle(class VulkanDescriptorHandle& handle) {

		ScopeLock lock(m_mutex);
		if (m_resourceMapper.contains(handle.m_pBlock)) m_resourceMapper.free(handle.m_pBlock);
		if (m_samplerMapper.contains(handle.m_pBlock))  m_samplerMapper.free(handle.m_pBlock);
		handle.m_pBlock = nullptr;

	}


	//! @brief          デスクリプタセットをコマンドバッファに記録
	void VulkanDescriptorHeap::recordDescriptorHeap(const vk::raii::CommandBuffer& commandBuffer, vk::PipelineLayout pipeline, s32 slot) const {
		commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline, slot, *m_set, {});
	}


	//! @brief          ハンドルを解放
	void VulkanDescriptorHeap::update() {
		ScopeLock lock(m_requestsLock);

		if (m_requests.empty()) {
			return;
		}

		for(s32 i=0;i<m_requests.size();++i) {
			auto& request = m_requests[i];
			auto& subrequest = m_subrequests[i];

			if (request.pBufferInfo) request.pBufferInfo = &std::get<vk::DescriptorBufferInfo>(subrequest);
			if (request.pImageInfo) request.pImageInfo = &std::get<vk::DescriptorImageInfo>(subrequest);
		}

		m_set.getDevice().updateDescriptorSets(m_requests, {});

		m_requests.clear();
		m_subrequests.clear();
	}

}