//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/TextureDesc.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    struct RenderTextureDesc {
        TextureFormat   format = TextureFormat::RGBA8;   //!< テクスチャフォーマット
        Size            size = { 0,0,0 };                //!< サイズ

        struct Clear {
            Color color;
            f32 depth;
            u32 stencil;
        } clear;
    };

}// namespcae ob