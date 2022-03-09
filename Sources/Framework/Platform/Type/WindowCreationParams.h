//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Type/Size.h>

namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウ生成定義
    //@―---------------------------------------------------------------------------
    struct WindowCreationDesc {
        String      title = TC("NONAME");			// ウィンドウタイトル
        Size		clientSize{ 640, 480 };	        // クライアント領域のピクセルサイズ
        bool		fullscreen = false;				// フルスクリーンモードで作成するかどうか
        bool		resizable = true;				// 可変ウィンドウとして作成するかどうか
    };

}// namespace ob::platform