//***********************************************************
//! @file
//! @brief		リソース・タイプ
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  リソース・タイプ
    //@―---------------------------------------------------------------------------
    enum class ResourceType{
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