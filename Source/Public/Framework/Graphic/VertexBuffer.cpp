//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/VertexBuffer.h>
#include <Framework/Graphic/Private/Device.h>

namespace ob::graphic {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	VertexBuffer::VertexBuffer(){}


	//@―---------------------------------------------------------------------------
	//! @brief			コンストラクタ
	//!
	//! @param size		頂点バッファのサイズ(byte)
	//! @param usage	リソースの使用方法
	//@―---------------------------------------------------------------------------
	VertexBuffer::VertexBuffer(size_t stribe, size_t count, ResourceUsage usage) {
		BufferDesc desc;
		desc.bufferType = BufferType::VertexBuffer;
		desc.usage			= usage;
		desc.bufferSize		= stribe * count;
		desc.bufferStride	= (u32)stribe;

		m_buffer=Buffer(desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief			コンストラクタ
	//!
	//! @param size		頂点バッファのサイズ(byte)
	//! @param size		頂点データ
	//! @param usage	リソースの使用方法
	//@―---------------------------------------------------------------------------
	VertexBuffer::VertexBuffer(size_t stribe, size_t count, void* pData, ResourceUsage usage)
		:VertexBuffer(stribe,count,usage)
	{
		update(stribe*count, pData);
	}


	//@―---------------------------------------------------------------------------
	//! @brief			頂点バッファのバイトサイズを取得
	//@―---------------------------------------------------------------------------
	size_t VertexBuffer::size()const {
		return m_buffer.size();
	}


	//@―---------------------------------------------------------------------------
	//! @brief			リソースの使用方法を変更
	//@―---------------------------------------------------------------------------
	void VertexBuffer::setResourceUsage(ResourceUsage usage)const {
		// TODO ResourceUsageの変更
		OB_NOTIMPLEMENTED();
	}


	//@―---------------------------------------------------------------------------
	//! @brief			頂点データを更新
	//@―---------------------------------------------------------------------------
	void VertexBuffer::update(size_t size, void* pData) {
		m_buffer.update(size, pData,0);
	}

}// namespace ob