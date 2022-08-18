//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Rendering/Mesh.h>

namespace ob::rendering {





    Mesh::Mesh(BlobView vertices, size_t vertexSize, BlobView indices, size_t indexSize) {
        if (vertexSize <= 0)return;
        if (indexSize <= 0)return;
        if (vertices.empty())return;

        // 頂点 
        {
            graphic::BufferDesc desc;
            desc.bufferType = graphic::BufferType::VertexBuffer;
            desc.usage = graphic::ResourceUsage::Dynamic;
            desc.bufferSize = vertices.size();
            desc.bufferStride = (u32)vertexSize;
            m_vertexBuffer = graphic::Buffer(desc, vertices);
            m_vertexCount = desc.bufferSize / desc.bufferStride;
        }
        // インデックス
        if(!indices.empty()) {
            graphic::BufferDesc desc;
            desc.bufferType = graphic::BufferType::IndexBuffer;
            desc.usage = graphic::ResourceUsage::Dynamic;
            desc.bufferSize = indices.size();
            desc.bufferStride = (u32)indexSize;
            m_indexBuffer = graphic::Buffer(desc, vertices);
            m_indexCount = desc.bufferSize / desc.bufferStride;
        }
    }

}// namespace ob