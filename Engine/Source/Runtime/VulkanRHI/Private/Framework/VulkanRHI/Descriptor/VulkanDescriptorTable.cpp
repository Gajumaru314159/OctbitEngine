//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/VulkanRHI/Descriptor/VulkanDescriptorTable.h>
#include <Framework/VulkanRHI/VulkanDevice.h>
#include <Framework/VulkanRHI/Texture/VulkanTexture.h>
#include <Framework/VulkanRHI/Buffer/VulkanBuffer.h>
#include <Framework/VulkanRHI/Sampler/VulkanSampler.h>
#include <Framework/VulkanRHI/Descriptor/VulkanDescriptorLayout.h>

namespace ob::rhi {

	//! @brief              コンストラクタ
	VulkanDescriptorTable::VulkanDescriptorTable(VulkanDevice& device, const DescriptorTableDesc& desc)
		: m_device(device)
		, m_desc(desc)
	{

		auto& vkdevice = device.getDevice();

		m_layout = m_desc.layout.cast<VulkanDescriptorLayout>();

		if (m_layout == nullptr) throw Exception("RootSignatureが未設定です");

		m_elements.resize(m_layout->getDesc().items.size());

		// TODO FixedHashMapを使う
		constexpr auto TYPE_NUM = 6;
		HashMap<vk::DescriptorType, u32> descTypeCount;


		auto& items = m_layout->getDesc().items;

		// タイプごとのアイテム数を計算
		for (auto& item : items) {
			switch (item.type) {
			case BindingType::Texture:
				descTypeCount[vk::DescriptorType::eSampledImage]++;
				break;
			case BindingType::RWTexture:
				descTypeCount[vk::DescriptorType::eStorageImage]++;
				break;
			case BindingType::Buffer:
				descTypeCount[vk::DescriptorType::eUniformTexelBuffer]++;
				break;
			case BindingType::RWBuffer:
				descTypeCount[vk::DescriptorType::eStorageTexelBuffer]++;
				break;
			case BindingType::StructuredBuffer:
			case BindingType::RWStructuredBuffer:
			case BindingType::ByteAddressBuffer:
			case BindingType::RWByteAddressBuffer:
				descTypeCount[vk::DescriptorType::eStorageBuffer]++;
				break;
			case BindingType::ConstantBuffer:
				descTypeCount[vk::DescriptorType::eUniformBuffer]++;
				break;
			case BindingType::Sampler:
				descTypeCount[vk::DescriptorType::eSampler]++;
				break;
			}
		}

		// タイプごとのアイテム数をDescriptorPoolSizeに格納
		FixedVector<vk::DescriptorPoolSize, TYPE_NUM> descPoolSizes;
		for (auto& [type, count] : descTypeCount) {
			auto& descPoolSize = descPoolSizes.emplace_back();
			descPoolSize.type = type;
			descPoolSize.descriptorCount = count;
		}

		// Poolを生成
		vk::DescriptorPoolCreateInfo info;
		info.maxSets = 1;
		info.flags |= vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
		info.flags |= vk::DescriptorPoolCreateFlagBits::eUpdateAfterBindEXT; // Bindless用
		info.setPoolSizes(descPoolSizes);

		m_pool = vkdevice.createDescriptorPool(info, m_device.getAllocationCallbacks());

		// DescriptorSetを生成
		// NOTE 同じレイアウトのものは巨大なプールにする必要があるかもしれない
		vk::DescriptorSetLayout descSetLayouts[] = { m_layout->getNative()};
		vk::DescriptorSetAllocateInfo allocInfo;
		allocInfo.descriptorPool = *m_pool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = descSetLayouts;

		auto sets = vkdevice.allocateDescriptorSets(allocInfo);
		m_set = std::move(sets.front());
		
		m_device.setName(m_pool, m_desc.name);
		m_device.setName(m_set, m_desc.name);

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
		if (!is_in_range(index, m_elements)) {
			LOG_ERROR("範囲外エラー。DescriptorTableのインデックスが不正です。[index={}]", index);
			return false;
		}

		m_elements.at(index) = BufferElement{ resource };

		if (auto p = resource.cast<VulkanBuffer>()) {

			vk::DescriptorBufferInfo descBufInfo[1];
			descBufInfo[0].buffer = p->getNative();
			descBufInfo[0].offset = 0;
			descBufInfo[0].range = p->getDesc().size;

			vk::WriteDescriptorSet writeDescSet;
			writeDescSet.dstSet = *m_set;
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
		if (!is_in_range(index, m_elements)) {
			LOG_ERROR("範囲外エラー。DescriptorTableのインデックスが不正です。[index={}]", index);
			return false;
		}

		m_elements.at(index) = TextureElement{ resource , nullptr };
		auto& element = std::get<TextureElement>(m_elements.at(index));

		if (auto p = resource.cast<VulkanTexture>()) {

			// TODO TextureがTextureViewを兼ねるのでTextureからImageViewを取得しても良い
			p->createView(element.view);

			vk::DescriptorImageInfo imageInfo;
			imageInfo.imageView = *element.view;
			imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

			vk::WriteDescriptorSet writeDescSet;
			writeDescSet.dstSet = *m_set;
			writeDescSet.dstBinding = index;
			writeDescSet.dstArrayElement = 0;
			writeDescSet.descriptorType = type;
			writeDescSet.descriptorCount = 1;
			writeDescSet.pImageInfo = &imageInfo;

			m_set.getDevice().updateDescriptorSets({ writeDescSet }, {});
		}

		return true;
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
		if (!is_in_range(index, m_elements)) {
			LOG_ERROR("範囲外エラー。DescriptorTableのインデックスが不正です。[index={}]", index);
			return false;
		}

		m_elements.at(index) = SamplerElement{ resource };

		if (auto p = resource.cast<VulkanSampler>()) {

			vk::DescriptorImageInfo imageInfo;
			imageInfo.sampler = p->getNative();
			
			vk::WriteDescriptorSet writeDescSet;
			writeDescSet.dstSet = *m_set;
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
	void VulkanDescriptorTable::record(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipeline, s32 slot) const {
		commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline, slot, *m_set, {});
	}


	//! @brief  Bufferから適したDescriptorTypeを取得
	bool VulkanDescriptorTable::tryGetRangeType(s32 index, const Ref<rhi::Buffer>& buffer, vk::DescriptorType& type) const {

		auto& items = m_layout->getDesc().items;

		if (!buffer) return false;
		if (!is_in_range(index, items)) return false;

		auto& desc = buffer->getDesc();
		bool hasSRV = desc.flags.has(BufferFlag::ShaderResource);
		bool hasUAV = desc.flags.has(BufferFlag::UnorderedAccess);

		switch (items[index].type) {
		case BindingType::Buffer:
			type = vk::DescriptorType::eUniformTexelBuffer;
			return hasSRV;

		case BindingType::RWBuffer:
			type = vk::DescriptorType::eStorageTexelBuffer;
			return hasUAV;

		case BindingType::StructuredBuffer:
			type = vk::DescriptorType::eStorageBuffer;
			return hasSRV && 0 < desc.stride;

		case BindingType::RWStructuredBuffer:
			type = vk::DescriptorType::eStorageBuffer;
			return hasUAV && 0 < desc.stride;

		case BindingType::ByteAddressBuffer:
			type = vk::DescriptorType::eStorageBuffer;
			return hasSRV;

		case BindingType::RWByteAddressBuffer:
			type = vk::DescriptorType::eStorageBuffer;
			return hasUAV;

		case BindingType::ConstantBuffer:
			type = vk::DescriptorType::eUniformBuffer;
			return desc.state == BufferState::Constant;
			
		default: return false;
		}
	}


	//! @brief  Textureから適したDescriptorTypeを取得
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

		default: return false;
		}
	}


	//! @brief  Samplerから適したDescriptorTypeを取得
	bool VulkanDescriptorTable::tryGetRangeType(s32 index, const Ref<rhi::Sampler>& sampler, vk::DescriptorType& type) const {

		auto& items = m_layout->getDesc().items;

		if (!sampler) return false;
		if (!is_in_range(index, items)) return false;

		switch (items[index].type) {
		case BindingType::Sampler:
			type = vk::DescriptorType::eSampler;
			return true;
		default: return false;
		}
	}
}