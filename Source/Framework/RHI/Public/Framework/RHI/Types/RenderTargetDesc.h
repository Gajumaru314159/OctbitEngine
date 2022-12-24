﻿//***********************************************************
//! @file
//! @brief		描画ターゲット定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/TextureDesc.h>
#include <Framework/RHI/Constants.h>

namespace ob::rhi {

#pragma region Element

    //@―---------------------------------------------------------------------------
    //! @brief      カラー・テクスチャ定義
    //@―---------------------------------------------------------------------------
    struct ColorTextureDesc {
        TextureFormat   format          = TextureFormat::RGBA8;     //!< テクスチャフォーマット
        Color           clearColor      = Color::Black;             //!< クリア・カラー
    };


    //@―---------------------------------------------------------------------------
    //! @brief      デプス・テクスチャ定義
    //@―---------------------------------------------------------------------------
    struct DepthTextureDesc {
        TextureFormat   format          = TextureFormat::D24S8;     //!< テクスチャフォーマット
        f32             clearDepth      = 0.0f;                     //!< クリア・デプス
        u32             clearStencil    = 0;                        //!< クリア・ステンシル
    };

#pragma endregion

    //@―---------------------------------------------------------------------------
    //! @brief      描画ターゲット定義
    //! 
    //! @details    デプスを使用しない場合はdepthを空にしてください。
    //@―---------------------------------------------------------------------------
    struct RenderTargetDesc {
        Size                       size = { 0,0 };                 //!< サイズ
        Array<ColorTextureDesc>    colors;                         //!< カラー書き込み先
        Array<DepthTextureDesc>    depth;                          //!< デプス書き込み先
    };

}// namespcae ob::rhi