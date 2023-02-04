//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/PipelineState.h>


namespace ob::graphic {

    struct MaterialPass {
        Name renderTag;     // 異なるマテリアルで共通 事前にRenderPassを設定する必要あり
        //  RenderPass/Subpass

        // リソース
        // Shader vs;
    };

    struct MaterialPropertyDesc {
        String name;
        u32 offset;
    };

    //@―---------------------------------------------------------------------------
    //! @brief  マテリアル定義
    //@―---------------------------------------------------------------------------
    struct MaterialDesc{
        
        HashMap<Name, MaterialPass> passes;
        
        Array<MaterialPropertyDesc> floatProperties;
        Array<MaterialPropertyDesc> colorProperties;
        Array<MaterialPropertyDesc> matrixProperties;
        Array<MaterialPropertyDesc> textureProperties;
    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob