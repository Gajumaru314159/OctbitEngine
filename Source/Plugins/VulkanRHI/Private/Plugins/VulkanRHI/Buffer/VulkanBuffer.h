//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/Types/BufferDesc.h>
#include <Plugins/VulkanRHI/Descriptor/VulkanDescriptorHandle.h>

namespace ob::rhi {

	class VulkanRHI;

	//! @brief      バッファ
	class VulkanBuffer:public rhi::Buffer {
	public:

		//! @brief  コンストラクタ
		//! 
		//! @param desc バッファ定義
		VulkanBuffer(VulkanRHI& rDevice,const BufferDesc& desc);


		//! @brief  コンストラクタ
		//! 
		//! @param desc バッファ定義
		//! @param data 初期化データ
		VulkanBuffer(VulkanRHI& rDevice, const BufferDesc& desc, const Blob& blob);


		//! @brief  コンストラクタ
		VulkanBuffer(VulkanRHI& rDevice, const BufferViewDesc& desc);


		//! @brief      名前を取得
		const String& getName()const override;


		//! @brief  定義を取得
		const BufferDesc& getDesc()const override;


		//! @brief      BindlessHandleを取得
		BindlessHandle getHandle()const override;


		//! @brief      バッファを更新
		//! 
		//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
		void update(size_t size, const void* pData, size_t offset) override;


		//! @brief      バッファを更新
		//! 
		//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
		void update(const CopyFunc& func) override;


		//! @brief      バッファを更新(直接更新)
		void updateDirect(size_t size, const void* pData, size_t offset) override;


		//! @brief      バッファを更新(直接更新)
		void updateDirect(const CopyFunc& func) override;

	public:

		//! @brief      バッファを取得
		vk::Buffer getNative()const { return m_shared->buffer; }


	private:

		VulkanRHI& m_rhi;
		BufferDesc m_desc;
		BufferViewDesc m_viewDesc;

		struct SharedResource {
			vk::raii::Buffer buffer = nullptr;
			vk::raii::DeviceMemory memory = nullptr;
		};

		SPtr<SharedResource> m_shared;
		VulkanDescriptorHandle m_handle;
	};



	//!@ condn

	//! @brief      BindlessHandleを取得
	inline BindlessHandle VulkanBuffer::getHandle()const {
		BindlessHandle handle;
		if (m_handle.empty()) return handle;
		handle.type = BindingType::ByteAddressBuffer;
		handle.index = m_handle.getBindlessIndex();
		return handle;
	}


	//! @endcond


}