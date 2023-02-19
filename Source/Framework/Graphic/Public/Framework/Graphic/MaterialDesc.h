//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/PipelineState.h>
#include <Framework/Engine/Name.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  マテリアルパス定義
    //@―---------------------------------------------------------------------------
    struct MaterialPass {
        engine::Name            renderTag;     // 異なるマテリアルで共通 事前にRenderPassを設定する必要あり

        Ref<rhi::RootSignature> rootSignature;
        Ref<rhi::Shader>        vs;
        Ref<rhi::Shader>        ps;

        rhi::BlendDescList		blends;
        rhi::RasterizerDesc		rasterizer;
        rhi::DepthStencilDesc	depthStencil;

        rhi::VertexLayout       layout;
    };

    //@―---------------------------------------------------------------------------
    //! @brief  マテリアル定義
    //@―---------------------------------------------------------------------------
    struct MaterialDesc{
        
        HashMap<engine::Name, MaterialPass> passes;
        
        Array<String> floatProperties;
        Array<String> colorProperties;
        Array<String> matrixProperties;
        Array<String> textureProperties;
    };


    struct MaterialUser {
        rhi::VertexLayout       vertexLayout;
    };





    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob