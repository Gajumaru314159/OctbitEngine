//***********************************************************
//! @file
//! @brief		Graphicモジュール設定
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rhi {

    //! @brief      Graphicモジュール設定
    struct RHIConfig {
        s32 frameBufferCount = 2;   //!< ダブルバッファリング

        bool enablePIX = false;
        bool enableDebugLayer = false;
        bool breakWithWarning = false;
        bool enableDirectStorage = true;
        bool enableDirectStorageDebug = false;

        bool enableBindless = false;

    };

}