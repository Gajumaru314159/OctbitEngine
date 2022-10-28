//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Rendering/CommandBuffer.h>
#include <Framework/Rendering/Material.h>

namespace ob::rendering
{

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------


    void CommandBuffer::drawMesh(Span<graphic::Buffer> streams, const graphic::Buffer& indices, const Matrix&, const Material& material, Name shaderPass) {

        //cmd.setVertices(streams,vparam);
        //cmd.setIndices(indices,iparam);
        //material.record(,shaderPass)
    }

}// namespace ob