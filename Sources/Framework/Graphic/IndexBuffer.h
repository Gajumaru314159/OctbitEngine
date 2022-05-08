﻿//***********************************************************
//! @file
//! @brief		頂点バッファ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Buffer.h>

namespace ob::graphic {

	enum class IndexFormat {
		U16,
		U32,
	};

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class IndexBuffer {
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		IndexBuffer();


		//@―---------------------------------------------------------------------------
		//! @brief			コンストラクタ
		//!
		//! @param count	頂点数
		//! @param format	頂点フォーマット
		//! @param usage	リソースの使用方法
		//@―---------------------------------------------------------------------------
		IndexBuffer(size_t count, IndexFormat format= IndexFormat::U16, ResourceUsage usage = ResourceUsage::Dynamic);


		//@―---------------------------------------------------------------------------
		//! @brief			コンストラクタ
		//!
		//! @param count	頂点数
		//! @param format	頂点フォーマット
		//! @param pData	初期データ
		//! @param usage	リソースの使用方法
		//@―---------------------------------------------------------------------------
		IndexBuffer(size_t count, const void* pData, IndexFormat format= IndexFormat::U16 , ResourceUsage usage = ResourceUsage::Dynamic);


		//@―---------------------------------------------------------------------------
		//! @brief			インデックスバッファのバイトサイズを取得
		//@―---------------------------------------------------------------------------
		size_t size()const;

		//@―---------------------------------------------------------------------------
		//! @brief			インデックス数を取得
		//@―---------------------------------------------------------------------------
		size_t count()const;


		//@―---------------------------------------------------------------------------
		//! @brief			リソースの使用方法を変更
		//@―---------------------------------------------------------------------------
		void setResourceUsage(ResourceUsage usage)const;


		//@―---------------------------------------------------------------------------
		//! @brief			頂点データを更新
		//@―---------------------------------------------------------------------------
		void update(size_t size,const void* pData);


		//@―---------------------------------------------------------------------------
		//! @brief			頂点フォーマットを取得
		//@―---------------------------------------------------------------------------
		IndexFormat getFormat()const;


		//@―---------------------------------------------------------------------------
		//! @brief			ストライブ幅を取得
		//@―---------------------------------------------------------------------------
		size_t getStribe()const;


	private:

		Buffer m_buffer;

	};

}// namespcae ob