﻿//***********************************************************
//! @file
//! @brief		ウィンドウ・ステート
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウスタイル
    //@―---------------------------------------------------------------------------
    enum class WindowStyle :u8 {
        Fixed,      // サイズ変更不可
        Sizable,    // サイズ変更可能
        Frameless,  // 枠なしウィンドウ
    };

    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウ状態
    //@―---------------------------------------------------------------------------
    enum class WindowState {
        FullScreen  = get_bit(0),//!< フルスクリーン
        Minimized   = get_bit(1),//!< 最小化されているか
        Maximized   = get_bit(2),//!< 最小化されているか
        Focused     = get_bit(3),//!< フォーカス中 
        Moved       = get_bit(4),//!< 移動中
        Resized     = get_bit(5),//!< リサイズ中
    };

}// namespcae ob::platform