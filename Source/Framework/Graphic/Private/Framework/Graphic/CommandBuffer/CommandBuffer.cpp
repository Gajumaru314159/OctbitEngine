//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/CommandBuffer.h>
#include <Framework/Graphic/Material.h>

namespace ob::graphic
{

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------


    void CommandBuffer::drawMesh(Span<rhi::Buffer> streams, const rhi::Buffer& indices, const Matrix&, const Material& material, Name shaderPass) {

        //cmd.setVertices(streams,vparam);
        //cmd.setIndices(indices,iparam);
        //material.record(,shaderPass)
    }

}// namespace ob