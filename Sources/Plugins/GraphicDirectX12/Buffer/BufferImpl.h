﻿//***********************************************************
//! @file
//! @brief		バッファ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/IBuffer.h>
#include <Framework/Graphic/Types/BufferDesc.h>

namespace ob::graphic::dx12 {

	//@―---------------------------------------------------------------------------
	//! @brief      バッファ
	//@―---------------------------------------------------------------------------
	class BufferImpl:public IBuffer {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//! 
		//! @param desc バッファ定義
		//@―---------------------------------------------------------------------------
		BufferImpl(class DeviceImpl& rDevice,const BufferDesc& desc);


		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//! 
		//! @param desc バッファ定義
		//! @param data 初期化データ
		//@―---------------------------------------------------------------------------
		BufferImpl(class DeviceImpl& rDevice, const BufferDesc& desc, const Blob& blob);


		//@―---------------------------------------------------------------------------
		//! @brief  妥当な状態か
		//@―---------------------------------------------------------------------------
		bool isValid()const override;


		//@―---------------------------------------------------------------------------
		//! @brief  定義を取得
		//@―---------------------------------------------------------------------------
		const BufferDesc& getDesc()const override;


		//@―---------------------------------------------------------------------------
		//! @brief      バッファを更新
		//! 
		//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
		//@―---------------------------------------------------------------------------
		void update(size_t size, const void* pData, size_t offset) override;


		//@―---------------------------------------------------------------------------
		//! @brief      バッファを更新(直接更新)
		//! 
		//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
		//@―---------------------------------------------------------------------------
		void updateDirect(size_t size, const void* pData, size_t offset) override;


	public:

		//@―---------------------------------------------------------------------------
		//! @brief      ネイティブオブジェクトを取得
		//@―---------------------------------------------------------------------------
		ID3D12Resource* getNative()const {return m_buffer.Get();}


	private:

		const BufferDesc m_desc;

		ComPtr<ID3D12Resource> m_buffer;

	};


}// namespace pb::graphic::dx12