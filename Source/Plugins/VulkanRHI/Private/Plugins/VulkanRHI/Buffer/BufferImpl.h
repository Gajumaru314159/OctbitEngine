//***********************************************************
//! @file
//! @brief		バッファ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/Types/BufferDesc.h>

namespace ob::rhi::vulkan {

	class VulkanRHI;

	//! @brief      バッファ
	class BufferImpl:public rhi::Buffer {
	public:

		//! @brief  コンストラクタ
		//! 
		//! @param desc バッファ定義
		BufferImpl(VulkanRHI& rDevice,const BufferDesc& desc);


		//! @brief  コンストラクタ
		//! 
		//! @param desc バッファ定義
		//! @param data 初期化データ
		BufferImpl(VulkanRHI& rDevice, const BufferDesc& desc, const Blob& blob);


		//! @brief      名前を取得
		const String& getName()const override;


		//! @brief  定義を取得
		const BufferDesc& getDesc()const override;


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


	private:

		VulkanRHI& m_rhi;
		BufferDesc m_desc;

		vk::raii::Buffer m_buffer = nullptr;
		vk::raii::DeviceMemory m_memory = nullptr;

	};


}