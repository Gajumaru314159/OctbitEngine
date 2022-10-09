﻿//***********************************************************
//! @file
//! @brief		バッファ・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/GraphicObject.h>
#include <Framework/Graphic/Types/BufferDesc.h>
#include <Framework/Core/Misc/Blob.h>

//===============================================================
// クラス宣言
//===============================================================
namespace ob::graphic {

	//@―---------------------------------------------------------------------------
	//! @brief      バッファ・インターフェイス
	//@―---------------------------------------------------------------------------
	class IBuffer :public GraphicObject {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  妥当な状態か
		//@―---------------------------------------------------------------------------
		virtual bool isValid()const = 0;


		//@―---------------------------------------------------------------------------
		//! @brief  定義を取得
		//@―---------------------------------------------------------------------------
		virtual const BufferDesc& getDesc()const = 0;


		//@―---------------------------------------------------------------------------
		//! @brief      バッファを更新
		//! 
		//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
		//@―---------------------------------------------------------------------------
		virtual void update(size_t size, const void* pData, size_t offset) = 0;


		//@―---------------------------------------------------------------------------
		//! @brief      バッファを更新(直接更新)
		//! 
		//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
		//@―---------------------------------------------------------------------------
		virtual void updateDirect(size_t size, const void* pData, size_t offset) = 0;

	};

}// namespace pb::graphic