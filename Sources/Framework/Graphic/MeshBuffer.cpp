//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/MeshBuffer.h>

namespace ob::graphic {


	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	MeshBuffer::MeshBuffer(const void* pVertexData, size_t vertexStribe, size_t vertexCount, const void* pIndexData, size_t indexStribe, size_t indexCount)
	{
		// 頂点
		{
			BufferDesc desc;
			desc.bufferType = BufferType::VertexBuffer;
			desc.usage = ResourceUsage::Dynamic;
			desc.bufferSize = vertexStribe * vertexCount;
			desc.bufferStride = (u32)vertexStribe;
			m_vertexBuffer = Buffer(desc);
			m_vertexBuffer.updateDirect(desc.bufferSize, pVertexData);
		}
		// インデックス
		{
			BufferDesc desc;
			desc.bufferType = BufferType::IndexBuffer;
			desc.usage = ResourceUsage::Dynamic;
			desc.bufferSize = indexStribe * indexCount;
			desc.bufferStride = (u32)indexStribe;
			m_indexBuffer = Buffer(desc);
			m_indexBuffer.updateDirect(desc.bufferSize, pIndexData);
		}
	}

}// namespace ob