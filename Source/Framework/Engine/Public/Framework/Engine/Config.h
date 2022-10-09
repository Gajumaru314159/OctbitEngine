//***********************************************************
//! @file
//! @brief		エンジン設定
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <string>

namespace ob::engine {

    //@―---------------------------------------------------------------------------
    //! @brief  エンジン設定
    //! @see    OctbitInit()
    //@―---------------------------------------------------------------------------
    struct Config {
        bool consoleEnabled = false;    //!< コンソールウィンドウを表示するか

        std::string engineDirectory;    //!<
        std::string assetDirectory;     //!< アセットルートディレクトリ
    };

}// namespcae ob