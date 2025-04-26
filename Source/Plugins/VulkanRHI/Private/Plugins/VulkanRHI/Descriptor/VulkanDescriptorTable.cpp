//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Descriptor/VulkanDescriptorTable.h>
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/Buffer.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Texture/VulkanTexture.h>
#include <Plugins/VulkanRHI/Buffer/VulkanBuffer.h>
#include <Plugins/VulkanRHI/Sampler/VulkanSampler.h>
#include <Plugins/VulkanRHI/Descriptor/VulkanDescriptorLayout.h>
#include <magic_enum.hpp>

namespace ob::rhi::vulkan
{

	//! @brief              コンストラクタ
	//!
	//! @param type         デスクリプタに設定するリソースの種類
	//! @param elementNum   要素数
	VulkanDescriptorTable::VulkanDescriptorTable(VulkanRHI& rhi, const DescriptorTableDesc& desc)
		: m_rhi(rhi)
		, m_desc(desc)
	{

		auto& device = rhi.getDevice();

		m_layout = m_desc.layout.cast<VulkanDescriptorLayout>();

		if (m_layout == nullptr) throw Exception("RootSignatureが未設定です");

		s32 itemCount = m_layout->getDesc().items.size();

		m_elemetns.resize(itemCount);

		// TODO FixedHashMapを使う
		constexpr auto TYPE_NUM = 5;
		HashMap<vk::DescriptorType, u32> descTypeCount;


		auto& items = m_layout->getDesc().items;

		// タイプごとのアイテム数を計算
		for (auto& item : items) {
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

		// タイプごとのアイテム数をDescriptorPoolSizeに格納
		FixedVector<vk::DescriptorPoolSize, TYPE_NUM> descPoolSizes;
		for (auto& [type, count] : descTypeCount) {
			auto& descPoolSize = descPoolSizes.emplace_back();
			descPoolSize.type = type;
			descPoolSize.descriptorCount = count;
			descPoolSizes.push_back(descPoolSize);
		}

		// Poolを生成
		vk::DescriptorPoolCreateInfo info;
		info.maxSets = 1;
		info.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
		info.setPoolSizes(descPoolSizes);

		m_pool = device.createDescriptorPool(info, m_rhi.getAllocationCallbacks());

		
		// DescriptorSetを生成
		vk::DescriptorSetLayout descSetLayouts[] = { m_layout->getNative()};
		vk::DescriptorSetAllocateInfo allocInfo;
		allocInfo.descriptorPool = m_pool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = descSetLayouts;

		auto sets = device.allocateDescriptorSets(allocInfo);
		m_set = std::move(sets.front());
		
		m_rhi.setName(m_pool, "Octbit Descriptor");
		m_rhi.setName(m_set, "Octbit Descriptor");

		manage();
	}


	//! @brief      名前を取得
	const String& VulkanDescriptorTable::getName()const {
		return m_desc.name;
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

			// TODO 引数再確認
			vk::DescriptorBufferInfo descBufInfo[1];
			descBufInfo[0].buffer = p->getNative();
			descBufInfo[0].offset = index;
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

	//! @brief  バインドレスハンドルに使用するインデックスを取得
	BindlessHandle VulkanDescriptorTable::getBindlessHandle(s32 index)const {
		return BindlessHandle{ };
	}


	//! @brief CommandListに記録
	void VulkanDescriptorTable::record(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipeline, s32 slot) const {
		commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline, slot, *m_set, {});
	}


	//! @brief  Bufferから適したDescrptorTypeを取得
	bool VulkanDescriptorTable::tryGetRangeType(s32 index, const Ref<rhi::Buffer>& buffer, vk::DescriptorType& type) const {

		auto& items = m_layout->getDesc().items;

		if (!buffer) return false;
		if (!is_in_range(index, items)) return false;

		auto& desc = buffer->getDesc();
		bool hasSRV = desc.flags.has(BufferFlag::ShaderResource);
		bool hasUAV = desc.flags.has(BufferFlag::UnorderedAccess);

		switch (items[index].type) {
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


	//! @brief  Textureから適したDescrptorTypeを取得
	bool VulkanDescriptorTable::tryGetRangeType(s32 index, const Ref<rhi::Texture>& texture, vk::DescriptorType& type) const {

		auto& items = m_layout->getDesc().items;

		if (!texture) return false;
		if (!is_in_range(index, items)) return false;

		auto& desc = texture->desc();
		bool hasSRV = desc.flags.has(TextureFlag::ShaderResource);
		bool hasUAV = desc.flags.has(TextureFlag::UnorderedAccess);

		switch (items[index].type) {
		case BindingType::Texture:
			type = vk::DescriptorType::eSampledImage;
			return hasSRV;

		case BindingType::RWTexture:
			type = vk::DescriptorType::eStorageImage;
			return hasUAV;
		}
		return false;
	}


	//! @brief  Samplerから適したDescrptorTypeを取得
	bool VulkanDescriptorTable::tryGetRangeType(s32 index, const Ref<rhi::Sampler>& sampler, vk::DescriptorType& type) const {

		auto& items = m_layout->getDesc().items;

		if (!sampler) return false;
		if (!is_in_range(index, items)) return false;

		switch (items[index].type) {
		case BindingType::Sampler:
			type = vk::DescriptorType::eSampler;
			return true;
		}
		return false;
	}
}