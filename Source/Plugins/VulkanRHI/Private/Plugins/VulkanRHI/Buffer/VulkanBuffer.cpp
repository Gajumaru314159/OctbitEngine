//***********************************************************
//! @file
//! @brief		コマンドキュー
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Buffer/VulkanBuffer.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>

namespace ob::rhi::vulkan {

	//! @brief  コンストラクタ
	//! 
	//! @param desc バッファ定義
	VulkanBuffer::VulkanBuffer(VulkanRHI& rhi, const BufferDesc& desc)
		: m_rhi(rhi)
		, m_desc(desc)
	{

		if (!m_desc.isValid()) throw Exception("Invalid BufferDesc");

		auto& device = rhi.getDevice();

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

		m_buffer = device.createBuffer(info, m_rhi.getAllocationCallbacks());		

		// メモリ確保
		VkMemoryAllocateInfo allocInfo = m_rhi.getAllocationInfo(m_buffer.getMemoryRequirements(), vk::MemoryPropertyFlagBits::eDeviceLocal);

		m_memory = device.allocateMemory(allocInfo, m_rhi.getAllocationCallbacks());

		// バインド
		m_buffer.bindMemory(m_memory, 0);


		m_rhi.setName(m_buffer, m_desc.name);
		m_rhi.setName(m_memory, m_desc.name);

		manage();
	}


	//! @brief  コンストラクタ
	//! 
	//! @param desc バッファ定義
	//! @param data 初期化データ
	VulkanBuffer::VulkanBuffer(VulkanRHI& rhi, const BufferDesc& desc, const Blob& blob)
		: VulkanBuffer(rhi,desc)
	{
		update(blob.size(), blob.data(),0);

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

		m_rhi.getBufferUploader().add(BlobView(data, size), m_buffer, offset,TypeConverter::Convert(m_desc.state));
	}


	//! @brief      バッファを更新(直接更新)
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	void VulkanBuffer::updateDirect(const CopyFunc& func){
		if (!func) return;

		m_rhi.getBufferUploader().add(func, m_desc.size, m_buffer, 0, TypeConverter::Convert(m_desc.state));

		return;

	}

}