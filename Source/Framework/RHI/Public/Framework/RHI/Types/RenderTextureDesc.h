//***********************************************************
//! @file
//! @brief		RenderTexture 定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/TextureDesc.h>
#include <Framework/RHI/Forward.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  RenderTexture 定義
    //@―---------------------------------------------------------------------------
    struct RenderTextureDesc {
        String          name;
        TextureFormat   format = TextureFormat::RGBA8;  //!< テクスチャフォーマット
        Size            size = { 0,0,0 };               //!< サイズ
        Display*        display = nullptr;              //!< リサイズ追従するDisplay [Optional]

        struct Clear {
            Color color = Color::White;
            f32 depth = 0.0f;
            u32 stencil = 0;
        } clear;
    };

}// namespcae ob