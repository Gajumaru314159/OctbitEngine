//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/CommandBuffer.h>
#include <Framework/Graphic/Material.h>
#include <Framework/Graphic/Mesh.h>

namespace ob::graphic
{

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------

    void CommandBuffer::clear() {

    }

    // scale/offset/TextureArrayIndex
    void CommandBuffer::blit(const Texture& source, Texture& dest, const Material& mat) {

    }

    // void copyTexture(src,dest);// フォーマット/サイズが違う場合はレンダー   領域

    void CommandBuffer::drawMesh(const Mesh& mesh, const Matrix&, const Material&, Name shaderPass) {

        //cmd.setVertices(streams,vparam);
        //cmd.setIndices(indices,iparam);
        //material.record(,shaderPass)
    }
    void CommandBuffer::drawMeshInstanced(const Mesh& mesh, Span<Matrix> matrices, const Material& material, Name shaderPass) {

        m_cmdList->setVertexBuffer(mesh.getVertexBuffer());
        m_cmdList->setIndexBuffer(mesh.getIndexBuffer());

        // TODO matrix
        material.record(m_cmdList, shaderPass);

        rhi::DrawIndexedParam dp{};
        rhi::InstancedParam ip{};
        dp.indexCount = mesh.getIndexCount();
        ip.instanceCount = matrices.size();
        
        m_cmdList->drawIndexed(dp);
    }

    void CommandBuffer::drawMesh(Span<Ref<rhi::Buffer>> streams, const Ref<rhi::Buffer>& indices, const Matrix& matrix, const Material& material, Name shaderPass) {

        m_cmdList->setVertexBuffers(streams);
        m_cmdList->setIndexBuffer(indices);

        // TODO matrix
        material.record(m_cmdList, shaderPass);

    }


    void CommandBuffer::getTemporaryRT() {

    }

}// namespace ob