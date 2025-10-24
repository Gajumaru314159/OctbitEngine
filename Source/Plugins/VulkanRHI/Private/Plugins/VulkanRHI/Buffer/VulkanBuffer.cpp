//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Buffer/VulkanBuffer.h>
#include <Plugins/VulkanRHI/VulkanDevice.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>

namespace ob::rhi {

	//! @brief  コンストラクタ
	VulkanBuffer::VulkanBuffer(VulkanDevice& device, const BufferDesc& desc)
		: m_device(device)
		, m_desc(desc)
	{

		if (!m_desc.isValid()) throw Exception("Invalid BufferDesc");

		auto& vkdevice = device.getDevice();

		// バッファ生成
		vk::BufferCreateInfo info;
		info.size = m_desc.size;
		info.usage = vk::BufferUsageFlagBits::eTransferDst;
		info.sharingMode = vk::SharingMode::eExclusive;

		if (m_desc.flags.has(BufferFlag::ShaderResource)) (info.usage |= vk::BufferUsageFlagBits::eUniformBuffer) |= vk::BufferUsageFlagBits::eStorageBuffer; // StorageBufferはStructuredとByteAddressで必須
		if (m_desc.flags.has(BufferFlag::UnorderedAccess)) info.usage |= vk::BufferUsageFlagBits::eStorageBuffer;
		if (m_desc.flags.has(BufferFlag::CopySource)) info.usage |= vk::BufferUsageFlagBits::eTransferSrc;
		if (m_desc.flags.has(BufferFlag::CopyDest)) info.usage |= vk::BufferUsageFlagBits::eTransferDst;
		if (m_desc.flags.has(BufferFlag::Vertex)) info.usage |= vk::BufferUsageFlagBits::eVertexBuffer;
		if (m_desc.flags.has(BufferFlag::Index)) info.usage |= vk::BufferUsageFlagBits::eIndexBuffer;
		if (m_desc.flags.has(BufferFlag::Constant)) info.usage |= vk::BufferUsageFlagBits::eUniformBuffer;
		if (m_desc.flags.has(BufferFlag::IndirectArgument)) info.usage |= vk::BufferUsageFlagBits::eIndirectBuffer;

		// メモリ確保
		m_shared = std::make_shared<SharedResource>();
		m_shared->buffer = vkdevice.createBuffer(info, m_device.getAllocationCallbacks());		
		VkMemoryAllocateInfo allocInfo = m_device.getAllocationInfo(m_shared->buffer.getMemoryRequirements(), vk::MemoryPropertyFlagBits::eDeviceLocal);
		m_shared->memory = vkdevice.allocateMemory(allocInfo, m_device.getAllocationCallbacks());
		m_shared->buffer.bindMemory(*m_shared->memory, 0);

		// バッファハンドルの生成
		{
			vk::DescriptorBufferInfo descBufInfo[1];
			descBufInfo[0].buffer = *m_shared->buffer;
			descBufInfo[0].offset = 0;
			descBufInfo[0].range = m_desc.size;

			vk::WriteDescriptorSet writeDescSet;
			writeDescSet.dstArrayElement = 0;

			bool withoutView = false;
			
			if (desc.flags & BufferFlag::Constant) {
				writeDescSet.descriptorType = vk::DescriptorType::eUniformBuffer;
				m_viewDesc.type = BufferViewType::ConstantBuffer;
			} else if (desc.flags & BufferFlag::ShaderResource) {
				writeDescSet.descriptorType = vk::DescriptorType::eStorageBuffer;
				m_viewDesc.type = BufferViewType::ByteAddressBuffer;
			} else if (desc.flags & BufferFlag::UnorderedAccess) {
				writeDescSet.descriptorType = vk::DescriptorType::eStorageBuffer;
				m_viewDesc.type = BufferViewType::RWByteAddressBuffer;
			} else {
				// 純粋なVertexBufferなど
				withoutView = true;
			}
			// 初回生成時対応
			// eUniformTexelBuffer
			// eStorageTexelBuffer

			if (!withoutView) {
				writeDescSet.setBufferInfo(descBufInfo);

				m_device.allocateHandle(m_handle, writeDescSet);
			}
		}

		m_device.setName(m_shared->buffer, m_desc.name);
		m_device.setName(m_shared->memory, m_desc.name);

		manage();
	}


	//! @brief  コンストラクタ
	VulkanBuffer::VulkanBuffer(VulkanDevice& device, const BufferDesc& desc, const Blob& blob)
		: VulkanBuffer(device,desc)
	{
		update(blob.size(), blob.data(),0);
	}

	//! @brief  コンストラクタ
	VulkanBuffer::VulkanBuffer(VulkanDevice& device, const BufferViewDesc& desc) 
		: m_device(device)
		, m_viewDesc(desc)
	{

		auto base = desc.base.cast<VulkanBuffer>();

		if (base == nullptr) throw Exception("ベースバッファが指定されていません");

		m_desc = base->m_desc;

		if (!m_desc.isValid()) throw Exception("Invalid BufferDesc");

		m_shared = base->m_shared;

		vk::DescriptorBufferInfo descBufInfo[1];
		descBufInfo[0].buffer = *m_shared->buffer;
		descBufInfo[0].offset = 0;
		descBufInfo[0].range = m_desc.size;

		vk::WriteDescriptorSet writeDescSet;
		writeDescSet.dstArrayElement = 0;
		writeDescSet.setBufferInfo(descBufInfo);
	
		bool withView = false;

		switch (desc.type) {
		case BufferViewType::ConstantBuffer:
			if (m_desc.flags & BufferFlag::Constant) {
				writeDescSet.descriptorType = vk::DescriptorType::eUniformBuffer;
				withView = true;
			} else {
				throw Exception("flagsとtypeに互換性がありません");
			}
			break;
		case BufferViewType::Buffer:
		case BufferViewType::StructuredBuffer:
		case BufferViewType::ByteAddressBuffer:
			if (m_desc.flags & BufferFlag::ShaderResource) {
				writeDescSet.descriptorType = vk::DescriptorType::eStorageBuffer;
				withView = true;
			} else {
				throw Exception("flagsとtypeに互換性がありません");
			}
			break;
		case BufferViewType::RWBuffer:
		case BufferViewType::RWStructuredBuffer:
		case BufferViewType::RWByteAddressBuffer:
			if (m_desc.flags & BufferFlag::UnorderedAccess) {
				writeDescSet.descriptorType = vk::DescriptorType::eStorageBuffer;
				withView = true;
			} else {
				throw Exception("flagsとtypeに互換性がありません");
			}
			break;
		default:break;
		}

		if (withView) {
			m_device.allocateHandle(m_handle, writeDescSet);
		}

		manage();
	}


	//! @brief      名前を取得
	const String& VulkanBuffer::getName()const {
		return m_desc.name;
	}


	//! @brief  定義を取得
	const BufferDesc& VulkanBuffer::getDesc()const {
		return m_desc;
	}


	//! @brief      バッファを更新
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	void VulkanBuffer::update(size_t size, const void* pData, size_t offset) {
		updateDirect(size,pData,offset);
	}


	//! @brief      バッファを更新
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	void VulkanBuffer::update(const CopyFunc& func) {
		// TODO 間接更新
		updateDirect(func);
	}


	//! @brief      バッファを更新(直接更新)
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	void VulkanBuffer::updateDirect(size_t size, const void* data, size_t offset) {
		if (data == nullptr) return;

		m_device.getBufferUploader().add(BlobView(data, size), m_shared->buffer, offset,TypeConverter::Convert(m_desc.state));
	}


	//! @brief      バッファを更新(直接更新)
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	void VulkanBuffer::updateDirect(const CopyFunc& func){
		if (!func) return;

		m_device.getBufferUploader().add(func, m_desc.size, m_shared->buffer, 0, TypeConverter::Convert(m_desc.state));

		return;

	}

}