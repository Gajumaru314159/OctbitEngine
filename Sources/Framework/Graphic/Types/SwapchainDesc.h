//***********************************************************
//! @file
//! @brief		スワップチェーン定義
//! @author		Gajumaru
//***********************************************************
#pragma once

#include <Framework/Platform/Window/Window.h>
#include <Framework/Graphic/Types/TextureFormat.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  スワップチェーン定義
    //@―---------------------------------------------------------------------------
    struct SwapchainDesc {
        platform::Window*   window;         //!< 描画ウィンドウ
        TextureFormat       format;         //!< フォーマット
        f32                 refleshRate;    //!< リフレッシュ・レート
        Size                size;           //!< バックバッファのサイズ
        s32                 backBufferNum;  //!< バックバッファの数
        bool                vsync;          //!< VSyncが有効か
        bool                hdr;            //!< HDRが有効か
    };

}// namespcae ob::graphic