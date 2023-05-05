//***********************************************************
//! @file
//! @brief		レンダーテクスチャ定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/TextureDesc.h>
#include <Framework/RHI/Forward.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  クリアカラー
    //@―---------------------------------------------------------------------------
    struct ClearColor {
        Color color = Color::White;
        f32 depth = 0.0f;
        u32 stencil = 0;
    };

    //@―---------------------------------------------------------------------------
    //! @brief  レンダーテクスチャ定義
    //@―---------------------------------------------------------------------------
    struct RenderTextureDesc {
        String          name;
        TextureFormat   format = TextureFormat::RGBA8;  //!< テクスチャフォーマット
        Size            size = { 0,0,0 };               //!< サイズ
        ClearColor      clear;                          //!< クリアカラー

        Display*        display = nullptr;              //!< リサイズ追従するDisplay [Optional]
    };

}