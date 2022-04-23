//***********************************************************
//! @file
//! @brief		リソース状態
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      リソース使用法
    //! 
    //! @details    D3D12_RESOURCE_STATES 参照
    //@―---------------------------------------------------------------------------
    enum class ResourceState {
        Present,
        VertexBuffer,
        IndexBuffer,
        ConstantBuffer,
        RenderTarget,
        UnorderedAccess,
        DepthWrite,
        StencilWrite,
        DepthStencilWrite,
        DepthStencilRead,
        
        AllShaderResource,
        ComputeShaderResource,
        PixelShadeResource,
        
        StreamOut,              
        IndirectArgument,
        CoptyDest,
        CopySource,
        ResolveDest,
        ResolveSource,
        RaytracingAccelerationStructure,
    };

}// namespcae ob::graphic