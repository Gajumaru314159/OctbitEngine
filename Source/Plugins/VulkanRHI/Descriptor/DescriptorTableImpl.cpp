//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "DescriptorTableImpl.h"
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/Buffer.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Descriptor/DescriptorHeap.h>
#include <Plugins/VulkanRHI/Texture/TextureImpl.h>
#include <Plugins/VulkanRHI/Buffer/BufferImpl.h>
#include <Plugins/VulkanRHI/Sampler/SamplerImpl.h>
#include <Plugins/VulkanRHI/RootSignature/RootSignatureImpl.h>

namespace ob::rhi::vulkan
{

	//! @brief              コンストラクタ
	//!
	//! @param type         デスクリプタに設定するリソースの種類
	//! @param elementNum   要素数
	DescriptorTableImpl::DescriptorTableImpl(VulkanRHI& rhi, DescriptorHeap& heap, const Ref<RootSignature>& signature, s32 slot)
		: m_rhi(rhi)
		, m_signature(signature.cast<RootSignatureImpl>())
		, m_slot(slot)
	{
		if (m_signature == nullptr) return;

		vk::DescriptorPoolCreateInfo info;
		info.poolSizeCount = signature.cast<RootSignatureImpl>()->getDesc().slots[slot].items.size();
		info.pPoolSizes;
		info.maxSets = 1;

		auto& device = rhi.getDevice();

		auto pool = device.createDescriptorPool(info, m_rhi.getAllocationCallbacks());

		vk::DescriptorSetAllocateInfo allocInfo;
		allocInfo.descriptorPool = pool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts;

		auto set = device.allocateDescriptorSets(allocInfo);


		auto& slot2 = m_signature ? m_signature->getDesc().slots.at(m_slot) : m_desc;

		for (auto& item : slot2.items) {

			switch (item.type)
			{
			case BindingType::Texture:

				vk::DescriptorImageInfo imageInfo;
				imageInfo.imageView;
				imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
				imageInfo.sampler = nullptr;


			case BindingType::RWTexture:

				vk::DescriptorImageInfo imageInfo;
				imageInfo.imageView;
				imageInfo.imageLayout = vk::ImageLayout::eGeneral;
				imageInfo.sampler = nullptr;

				break;

			case BindingType::Buffer:
			case BindingType::RWBuffer:

				bool isUAV;

				vk::BufferViewCreateInfo bufferInfo;
				bufferInfo.buffer;
				bufferInfo.offset = 0;
				bufferInfo.range = item.size;
				bufferInfo.format = vk::Format::eR32G32B32A32Sfloat;
				// キャッシュ可能？

				vk::WriteDescriptorSet writeInfo;
				writeInfo.dstSet = set[0];
				writeInfo.dstBinding = item.index;
				writeInfo.dstArrayElement = 0;
				writeInfo.descriptorCount = 1;
				writeInfo.descriptorType = vk::DescriptorType::eStorageBuffer;
				writeInfo.pBufferInfo = &bufferInfo;
				writeInfo.pImageInfo = nullptr;
				writeInfo.pTexelBufferView = nullptr;

				break;
			case BindingType::StructuredBuffer:
			case BindingType::RWStructuredBuffer:
			case BindingType::ByteAddressBuffer:
			case BindingType::RWByteAddressBuffer:
			case BindingType::ConstantBuffer:

				bool isUAV;
				bool isStructuredBuffer;
				bool canHaveRawViews;
				if (item.type == BindingType::RWStructuredBuffer || item.type == BindingType::RWByteAddressBuffer) {
					if (!isUAV) {
						LOG_ERROR("{}はUAVリソースではありません", "");
						return;
					}
				}
				if (item.type == BindingType::StructuredBuffer || item.type == BindingType::RWStructuredBuffer) {
					if (!isStructuredBuffer) {
						LOG_ERROR("{}はStructuredBufferではありません", "");
						return;
					}
				}
				if (item.type == BindingType::ByteAddressBuffer || item.type == BindingType::RWByteAddressBuffer) {
					if (!canHaveRawViews) {
						LOG_ERROR("{}はRawViewを持っていません", "");
						return;
					}
				}

				vk::DescriptorBufferInfo bufferInfo;
				bufferInfo.buffer;
				bufferInfo.offset = 0;
				bufferInfo.range = item.size;
				bufferInfo.format = vk::Format::eR32G32B32A32Sfloat;
				// キャッシュ可能？

				vk::WriteDescriptorSet writeInfo;
				writeInfo.dstSet = set[0];
				writeInfo.dstBinding = item.index;
				writeInfo.dstArrayElement = 0;
				writeInfo.descriptorCount = 1;
				writeInfo.descriptorType = vk::DescriptorType::eStorageBuffer;
				writeInfo.pBufferInfo = &bufferInfo;
				writeInfo.pImageInfo = nullptr;
				writeInfo.pTexelBufferView = nullptr;

				// TODO Transition管理

				break;
			case BindingType::Sampler:

				vk::DescriptorImageInfo imageInfo;
				imageInfo.sampler;



				vk::WriteDescriptorSet writeInfo;
				writeInfo.dstSet = set[0];
				writeInfo.dstBinding = item.index;
				writeInfo.dstArrayElement = 0;
				writeInfo.descriptorCount = 1;
				writeInfo.descriptorType = vk::DescriptorType::eStorageBuffer;
				writeInfo.pBufferInfo = &bufferInfo;
				writeInfo.pImageInfo = nullptr;
				writeInfo.pTexelBufferView = nullptr;

				break;
			}

		}



	}

	DescriptorTableImpl::DescriptorTableImpl(VulkanRHI& rhi, DescriptorHeap& heap, const BindingSlot& desc)
		: m_rhi(rhi)
		, m_desc(desc)
	{
		heap.allocateHandle(m_handle, desc.items.size());

		m_elemetns.resize(desc.items.size());
	}


	//! @brief      名前を取得
	const String& DescriptorTableImpl::getName()const {
		return m_name;
	}


	//! @brief  バッファリソースを設定
	bool DescriptorTableImpl::setResource(s32 index,const Ref<Buffer>& resource) {
		return false;
	}


	//! @brief  テクスチャリソースを設定
	bool DescriptorTableImpl::setResource(s32 index, const Ref<Texture>& resource) {
		return false;
	}

	//! @brief  サンプラーリソースを設定
	bool DescriptorTableImpl::setResource(s32 index, const Ref<Sampler>& resource) {
		return false;
	}

	//! @brief CommandListに記録
	void DescriptorTableImpl::record(vk::CommandBuffer commandBuffer, s32 slot) const {
		s32 num = 1;
		commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_signature->getNative(), slot, m_set, {});
	}

}