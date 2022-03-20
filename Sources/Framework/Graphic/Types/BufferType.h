//***********************************************************
//! @file
//! @brief		バッファ・タイプ
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  バッファ・タイプ
    //@―---------------------------------------------------------------------------
    enum class BufferType {
        Unknown,
        VertexBuffer,
        IndexBuffer,
        ConstantBuffer,
        RenderTarget,
        UnorderedAccess,
        DepthWrite,
        StencilWrite,
        DepthStencilWrite,
        DepthStencilRead,
        // AllShaderResource,
        ComputeShaderResource,
        PixelShaderResource,
        StreamOut,
        IndirectArgument,
        CopyDest,
        CopySource,
        Present,
    };

}// namespcae ob::graphic