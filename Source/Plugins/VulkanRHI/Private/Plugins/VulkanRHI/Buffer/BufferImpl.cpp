//***********************************************************
//! @file
//! @brief		コマンドキュー
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Buffer/BufferImpl.h>
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/Utility/TypeConverter.h>

namespace ob::rhi::vulkan {

	//! @brief バリデート
	static bool IsInvalid(BufferDesc& desc) {

		if (desc.size == 0) {
			LOG_WARNING("バッファサイズは0より大きくなくてはいけません。サイズを256に設定します。 [name={}]", desc.name);
			desc.size = 256;
		}

		// D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT
		if (desc.type == BufferType::ConstantBuffer && desc.size % 256 != 0) {
			LOG_WARNING("定数バッファは256の倍数で作成する必要があります。サイズを{}から{}に調整します。 [name={}]", desc.name, desc.size, align_up(desc.size, 256));
			desc.size = align_up(desc.size, 256);		}

		return false;
	}

	//! @brief  コンストラクタ
	//! 
	//! @param desc バッファ定義
	BufferImpl::BufferImpl(VulkanRHI& rhi, const BufferDesc& desc)
		: m_rhi(rhi)
		, m_desc(desc)
	{

		if (IsInvalid(m_desc));

		auto& device = rhi.getDevice();

		// リソースの生成
		vk::BufferCreateInfo info;
		info.size = desc.size;
		info.usage = TypeConverter::Convert(desc.type);
		info.sharingMode = vk::SharingMode::eExclusive;

		m_buffer = device.createBuffer(info, m_rhi.getAllocationCallbacks());

		auto requirements = m_buffer.getMemoryRequirements();
		VkMemoryAllocateInfo allocInfo = m_rhi.getAllocationInfo(requirements, vk::MemoryPropertyFlags() | vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

		m_memory = device.allocateMemory(allocInfo, m_rhi.getAllocationCallbacks());
		m_buffer.bindMemory(m_memory, 0);

	}


	//! @brief  コンストラクタ
	//! 
	//! @param desc バッファ定義
	//! @param data 初期化データ
	BufferImpl::BufferImpl(VulkanRHI& rhi, const BufferDesc& desc, const Blob& blob)
		: BufferImpl(rhi,desc)
	{
		if (!isValid())return;
		// TODO
		OB_NOTIMPLEMENTED();
	}


	//! @brief  妥当な状態か
	bool BufferImpl::isValid()const {
		return true;
	}


	//! @brief      名前を取得
	const String& BufferImpl::getName()const {
		return m_desc.name;
	}


	//! @brief  定義を取得
	const BufferDesc& BufferImpl::getDesc()const {
		return m_desc;
	}


	//! @brief      バッファを更新
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	void BufferImpl::update(size_t size, const void* pData, size_t offset) {
		updateDirect(size,pData,offset);
	}


	//! @brief      バッファを更新
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	void BufferImpl::update(const CopyFunc& func) {
		// TODO 間接更新
		updateDirect(func);
	}


	//! @brief      バッファを更新(直接更新)
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	void BufferImpl::updateDirect(size_t size, const void* data, size_t offset) {
		if (data == nullptr) return;

		m_rhi.getBufferUploader().add(BlobView(data, size), m_buffer, offset);


		/*
		HRESULT result;
		byte* ptr = nullptr;
		result = m_resource->Map(0, nullptr, (void**)&ptr);
		if (FAILED(result))
		{
			Utility::OutputFatalLog(result, "ID3D12Resource::Map()");
			return;
		}
		memcpy_s(ptr+offset, (s64)m_desc.bufferSize-offset, pData, size);
		m_resource->Unmap(0, nullptr);
		*/
	}


	//! @brief      バッファを更新(直接更新)
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	void BufferImpl::updateDirect(const CopyFunc& func){
		if (!func) return;

		m_rhi.getBufferUploader().add(func, m_desc.size, m_buffer, 0);

		return;

	}

}