//***********************************************************
//! @file
//! @brief		ディスプレイ定義
//! @author		Gajumaru
//***********************************************************
#pragma once

#include <Framework/Platform/Window.h>
#include <Framework/RHI/Types/TextureFormat.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      ディスプレイ定義
    //@―---------------------------------------------------------------------------
    struct DisplayDesc {
        String              name;                       //!< 名前
        platform::Window    window;                     //!< 描画ウィンドウ
        TextureFormat       format=TextureFormat::RGBA8;//!< フォーマット(RGBA8/RGB16)
        f32                 refleshRate=60.0f;          //!< リフレッシュ・レート
        Size                size = {0,0};               //!< バックバッファのサイズ(0の場合ウィンドウサイズを使用)
        s32                 bufferCount=2;              //!< バックバッファの数
        bool                vsync=true;                 //!< VSyncが有効か
        bool                hdr=false;                  //!< HDRが有効か
    };

}