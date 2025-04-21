//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "VulkanDescriptorTable.h"
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/Buffer.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Texture/VulkanTexture.h>
#include <Plugins/VulkanRHI/Buffer/VulkanBuffer.h>
#include <Plugins/VulkanRHI/Sampler/VulkanSampler.h>
#include <Plugins/VulkanRHI/RootSignature/VulkanRootSignature.h>

namespace ob::rhi::vulkan
{

	//! @brief              コンストラクタ
	//!
	//! @param type         デスクリプタに設定するリソースの種類
	//! @param elementNum   要素数
	VulkanDescriptorTable::VulkanDescriptorTable(VulkanRHI& rhi, const Ref<RootSignature>& signature, s32 slot)
		: m_rhi(rhi)
		, m_signature(signature.cast<VulkanRootSignature>())
		, m_slot(slot)
	{
		if (m_signature == nullptr) return;

		s32 itemCount = m_signature->getItemCount(slot);

		m_elemetns.resize(itemCount);

		HashMap<vk::DescriptorType, u32> descTypeCount;


		auto& rSlot = m_signature ? m_signature->getDesc().slots.at(m_slot) : m_desc;

		for (auto& item : rSlot.items) {
			switch (item.type)
			{
			case BindingType::Texture:
				descTypeCount[vk::DescriptorType::eSampledImage]++;
				break;
			case BindingType::RWTexture:
				descTypeCount[vk::DescriptorType::eStorageImage]++;
				break;
			case BindingType::Buffer:
			case BindingType::StructuredBuffer:
			case BindingType::ByteAddressBuffer:
				descTypeCount[vk::DescriptorType::eStorageBuffer]++;
				break;
			case BindingType::RWBuffer:
			case BindingType::RWStructuredBuffer:
			case BindingType::RWByteAddressBuffer:
				descTypeCount[vk::DescriptorType::eStorageBuffer]++;
				break;
			case BindingType::ConstantBuffer:
				descTypeCount[vk::DescriptorType::eUniformBuffer]++;
				break;
			}
		}

		FixedVector<vk::DescriptorPoolSize, 16> descPoolSizes;
		for (auto& [type, count] : descTypeCount) {
			auto& descPoolSize = descPoolSizes.emplace_back();
			descPoolSize.type = type;
			descPoolSize.descriptorCount = count;
			descPoolSizes.push_back(descPoolSize);
		}

		vk::DescriptorPoolCreateInfo info;
		info.poolSizeCount = descPoolSizes.size();
		info.pPoolSizes = descPoolSizes.data();
		info.maxSets = 1;
		info.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;



		auto& device = rhi.getDevice();

		m_pool = device.createDescriptorPool(info, m_rhi.getAllocationCallbacks());

		

		vk::DescriptorSetLayout descSetLayouts[] = { m_signature->getLayouts(slot)};

		vk::DescriptorSetAllocateInfo allocInfo;
		allocInfo.descriptorPool = m_pool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = descSetLayouts;

		auto sets = device.allocateDescriptorSets(allocInfo);

		if (sets.size() != 1) {
			LOG_ERROR("不正な呼び出し。DescriptorSetの取得に失敗しました。");
			return;
		}

		m_set = std::move(sets.front());

		manage();
	}

	VulkanDescriptorTable::VulkanDescriptorTable(VulkanRHI& rhi, const BindingSlot& desc)
		: m_rhi(rhi)
		, m_desc(desc)
	{

		m_elemetns.resize(desc.items.size());

		manage();
	}


	//! @brief      名前を取得
	const String& VulkanDescriptorTable::getName()const {
		return m_name;
	}


	//! @brief  バッファリソースを設定
	bool VulkanDescriptorTable::setResource(s32 index,const Ref<Buffer>& resource) {

		vk::DescriptorType type;

		if (!resource) {
			LOG_ERROR("不正な引数。DescriptorTableに空のバッファを指定しました。[index={}]", index);
			return false;
		}
		if (!tryGetRangeType(index, resource, type)) {
			LOG_ERROR("不正な呼び出し。異なるタイプのDescriptorTableにバッファを指定しました。[index={}]", index);
			return false;
		}
		if (!is_in_range(index, m_elemetns)) {
			LOG_ERROR("範囲外エラー。DescriptorTableのインデックスが不正です。[index={}]", index);
			return false;
		}

		m_elemetns.at(index) = BufferElement{ resource };
		auto& element = std::get<BufferElement>(m_elemetns.at(index));

		if (auto p = resource.cast<VulkanBuffer>()) {

			vk::DescriptorBufferInfo descBufInfo[1];
			descBufInfo[0].buffer = p->getNative();
			descBufInfo[0].offset = m_slot;
			descBufInfo[0].range = 1;

			vk::WriteDescriptorSet writeDescSet;
			writeDescSet.dstSet = m_set;
			writeDescSet.dstBinding = index;
			writeDescSet.dstArrayElement = 0;
			writeDescSet.descriptorType =type;
			writeDescSet.descriptorCount = 1;
			writeDescSet.pBufferInfo = descBufInfo;

			m_set.getDevice().updateDescriptorSets({ writeDescSet }, {});

		}

		return false;
	}


	//! @brief  テクスチャリソースを設定
	bool VulkanDescriptorTable::setResource(s32 index, const Ref<Texture>& resource) {

		vk::DescriptorType type;
		
		if (!resource) {
			LOG_ERROR("不正な引数。DescriptorTableに空のバッファを指定しました。[index={}]", index);
			return false;
		}
		if (!tryGetRangeType(index, resource, type)) {
			LOG_ERROR("不正な呼び出し。異なるタイプのDescriptorTableにバッファを指定しました。[index={}]", index);
			return false;
		}
		if (!is_in_range(index, m_elemetns)) {
			LOG_ERROR("範囲外エラー。DescriptorTableのインデックスが不正です。[index={}]", index);
			return false;
		}

		m_elemetns.at(index) = TextureElement{ resource , nullptr };
		auto& element = std::get<TextureElement>(m_elemetns.at(index));

		if (auto p = resource.cast<VulkanTexture>()) {

			p->createSRV(element.view);

			vk::DescriptorImageInfo imageInfo;
			imageInfo.imageView = element.view;
			imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

			vk::WriteDescriptorSet writeDescSet;
			writeDescSet.dstSet = m_set;
			writeDescSet.dstBinding = index;
			writeDescSet.dstArrayElement = 0;
			writeDescSet.descriptorType = type;
			writeDescSet.descriptorCount = 1;
			writeDescSet.pImageInfo = &imageInfo;

			m_set.getDevice().updateDescriptorSets({ writeDescSet }, {});
		}
		return true;


		return false;
	}

	//! @brief  サンプラーリソースを設定
	bool VulkanDescriptorTable::setResource(s32 index, const Ref<Sampler>& resource) {
		
		vk::DescriptorType type;

		if (!resource) {
			LOG_ERROR("不正な引数。DescriptorTableに空のサンプラーを指定しました。[index={}]", index);
			return false;
		}
		if (!tryGetRangeType(index, resource, type)) {
			LOG_ERROR("不正な呼び出し。異なるタイプのDescriptorTableにバッファを指定しました。[index={}]", index);
			return false;
		}
		if (!is_in_range(index, m_elemetns)) {
			LOG_ERROR("範囲外エラー。DescriptorTableのインデックスが不正です。[index={}]", index);
			return false;
		}

		m_elemetns.at(index) = SamplerElement{ resource };
		auto& element = std::get<SamplerElement>(m_elemetns.at(index));

		if (auto p = resource.cast<VulkanSampler>()) {

			vk::DescriptorImageInfo imageInfo;
			imageInfo.sampler = p->getNative();
			
			vk::WriteDescriptorSet writeDescSet;
			writeDescSet.dstSet = m_set;
			writeDescSet.dstBinding = index;
			writeDescSet.dstArrayElement = 0;
			writeDescSet.descriptorType = type;
			writeDescSet.descriptorCount = 1;
			writeDescSet.pImageInfo = &imageInfo;
			
			m_set.getDevice().updateDescriptorSets({ writeDescSet }, {});
		}
		return true;
	}

	//! @brief CommandListに記録
	void VulkanDescriptorTable::record(vk::CommandBuffer commandBuffer, s32 slot) const {
		commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_signature->getNative(), slot, *m_set, {});
	}



	bool VulkanDescriptorTable::tryGetRangeType(s32 index, const Ref<rhi::Buffer>& buffer, vk::DescriptorType& type) const {

		auto& slot = m_signature ? m_signature->getDesc().slots.at(m_slot) : m_desc;

		if (!buffer) return false;
		if (!is_in_range(index, slot.items)) return false;

		auto& desc = buffer->getDesc();
		bool hasSRV = desc.flags.has(BufferFlag::ShaderResource);
		bool hasUAV = desc.flags.has(BufferFlag::UnorderedAccess);

		switch (slot.items[index].type) {
		case BindingType::Buffer:
			type = vk::DescriptorType::eUniformBuffer;
			return hasSRV;

		case BindingType::RWBuffer:
			type = vk::DescriptorType::eStorageBuffer;
			return hasUAV;

		case BindingType::StructuredBuffer:
			type = vk::DescriptorType::eUniformBuffer;
			return hasSRV && 0 < desc.stride;

		case BindingType::RWStructuredBuffer:
			type = vk::DescriptorType::eStorageBuffer;
			return hasUAV && 0 < desc.stride;

		case BindingType::ByteAddressBuffer:
			type = vk::DescriptorType::eUniformBuffer;
			return hasSRV;

		case BindingType::RWByteAddressBuffer:
			type = vk::DescriptorType::eStorageBuffer;
			return hasUAV;

		case BindingType::ConstantBuffer:
			type = vk::DescriptorType::eUniformBuffer;
			return desc.state == BufferState::Constant;
		}
		return false;
	}
	bool VulkanDescriptorTable::tryGetRangeType(s32 index, const Ref<rhi::Texture>& texture, vk::DescriptorType& type) const {

		auto& slot = m_signature ? m_signature->getDesc().slots.at(m_slot) : m_desc;

		if (!texture) return false;
		if (!is_in_range(index, slot.items)) return false;

		auto& desc = texture->desc();
		bool hasSRV = desc.flags.has(TextureFlag::ShaderResource);
		bool hasUAV = desc.flags.has(TextureFlag::UnorderedAccess);

		switch (slot.items[index].type) {
		case BindingType::Texture:
			type = vk::DescriptorType::eSampledImage;
			return hasSRV;

		case BindingType::RWTexture:
			type = vk::DescriptorType::eStorageImage;
			return hasUAV;
		}
		return false;
	}
	bool VulkanDescriptorTable::tryGetRangeType(s32 index, const Ref<rhi::Sampler>& sampler, vk::DescriptorType& type) const {

		auto& slot = m_signature ? m_signature->getDesc().slots.at(m_slot) : m_desc;

		if (!sampler) return false;
		if (!is_in_range(index, slot.items)) return false;

		switch (slot.items[index].type) {
		case BindingType::Sampler:
			type = vk::DescriptorType::eSampler;
			return true;
		}
		return false;
	}
}