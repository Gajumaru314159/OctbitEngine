//***********************************************************
//! @file
//! @brief		スワップチェーン生成情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Texture/TextureFormat.h"

namespace ob {
    namespace graphic {

        //@―---------------------------------------------------------------------------
        //! @brief  スワップチェーン生成情報
        //@―---------------------------------------------------------------------------
        struct SwapchainDesc{
            // ウィンドウ
            // リフレッシュレート
            // ウィンドウモード
            // メインウィンドウか
            // HDR有効か
            // モニターハンドル
            s32             width;
            s32             height;
            TextureFormat   format;
            s32             sampleCount;
            s32             sampleQuarity;
            s32             bufferCount;
        };

    }// namespace graphic
}// namespcae ob