//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rhi {

    //! @brief      Graphicモジュール設定
    struct RHIConfig {
        s32 frameBufferCount = 2;       //!< ダブルバッファリング
		bool enableBindless = false;    //!< Bindlessレンダリングを使用するか
    };

}