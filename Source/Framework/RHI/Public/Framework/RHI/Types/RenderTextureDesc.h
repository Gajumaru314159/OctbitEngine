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
            Color color;
            f32 depth;
            u32 stencil;
        } clear;
    };

}// namespcae ob