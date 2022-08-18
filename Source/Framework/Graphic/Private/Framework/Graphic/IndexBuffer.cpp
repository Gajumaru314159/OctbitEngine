//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/IndexBuffer.h>
#include <Framework/Graphic/Device.h>

namespace ob::graphic {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	IndexBuffer::IndexBuffer(){}


	//@―---------------------------------------------------------------------------
	//! @brief			コンストラクタ
	//!
	//! @param count	頂点数
	//! @param format	頂点フォーマット
	//! @param usage	リソースの使用方法
	//@―---------------------------------------------------------------------------
	IndexBuffer::IndexBuffer(size_t count, IndexFormat format, ResourceUsage usage) {
		BufferDesc desc;
		desc.bufferType = BufferType::VertexBuffer;
		desc.usage = usage;
		if (format == IndexFormat::U16)desc.bufferStride = 2;
		if (format == IndexFormat::U32)desc.bufferStride = 4;
		desc.bufferSize = count * desc.bufferStride;

		m_buffer = Buffer(desc);
	}


	//@―---------------------------------------------------------------------------
	//! @brief			コンストラクタ
	//!
	//! @param count	頂点数
	//! @param format	頂点フォーマット
	//! @param pData	初期データ
	//! @param usage	リソースの使用方法
	//@―---------------------------------------------------------------------------
	IndexBuffer::IndexBuffer(size_t count, const void* pData, IndexFormat format, ResourceUsage usage)
		: IndexBuffer(count,format,usage)
	{
		if(m_buffer)m_buffer.update(m_buffer.size(), pData, 0);
	}


	//@―---------------------------------------------------------------------------
	//! @brief			インデックスバッファのバイトサイズを取得
	//@―---------------------------------------------------------------------------
	size_t IndexBuffer::size()const {
		return m_buffer.size();
	}

	//@―---------------------------------------------------------------------------
	//! @brief			インデックス数を取得
	//@―---------------------------------------------------------------------------
	size_t IndexBuffer::count()const {
		return m_buffer.size()/m_buffer.getDesc().bufferStride;
	}


	//@―---------------------------------------------------------------------------
	//! @brief			リソースの使用方法を変更
	//@―---------------------------------------------------------------------------
	void IndexBuffer::setResourceUsage(ResourceUsage usage)const {
		OB_NOTIMPLEMENTED();
	}


	//@―---------------------------------------------------------------------------
	//! @brief			頂点データを更新
	//@―---------------------------------------------------------------------------
	void IndexBuffer::update(size_t size,const void* pData) {
		m_buffer.update(size, pData, 0);
	}

	//@―---------------------------------------------------------------------------
	//! @brief			頂点フォーマットを取得
	//@―---------------------------------------------------------------------------
	IndexFormat IndexBuffer::getFormat()const {
		if (m_buffer.getDesc().bufferStride == 2)return IndexFormat::U16;
		return IndexFormat::U32;
	}


	//@―---------------------------------------------------------------------------
	//! @brief			ストライブ幅を取得
	//@―---------------------------------------------------------------------------
	size_t IndexBuffer::getStribe()const {
		return m_buffer.getDesc().bufferStride;
	}

}// namespace ob