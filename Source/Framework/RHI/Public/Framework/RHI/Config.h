//***********************************************************
//! @file
//! @brief		Graphicモジュール設定
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      Graphicモジュール設定
    //@―---------------------------------------------------------------------------
    struct Config {
        s32 frameBufferCount = 2;   //!< ダブルバッファリング

        bool enablePIX = false;
    };

    void Register(ServiceInjector&);

}// namespace pb::rhi