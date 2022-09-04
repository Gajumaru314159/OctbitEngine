//***********************************************************
//! @file
//! @brief		スワップチェーン定義
//! @author		Gajumaru
//***********************************************************
#pragma once

#include <Framework/Platform/Window.h>
#include <Framework/Graphic/Types/TextureFormat.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  スワップチェーン定義
    //! 
    //! @details フォーマットに指定可能なフォーマットはRGBA8またはRGB16です。
    //@―---------------------------------------------------------------------------
    struct SwapchainDesc {
        platform::Window    window;                     //!< 描画ウィンドウ
        TextureFormat       format=TextureFormat::RGBA8;//!< フォーマット
        f32                 refleshRate=60.0f;          //!< リフレッシュ・レート
        Size                size = {0,0};               //!< バックバッファのサイズ(0の場合ウィンドウサイズを使用)
        s32                 bufferCount=2;              //!< バックバッファの数
        bool                vsync=true;                 //!< VSyncが有効か
        bool                hdr=false;                  //!< HDRが有効か
    };

}// namespcae ob::graphic