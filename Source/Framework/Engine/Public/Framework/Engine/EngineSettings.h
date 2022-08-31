//***********************************************************
//! @file
//! @brief		エンジン起動設定
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <string>

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief  エンジン起動設定
    //! @see    OctbitInit()
    //@―---------------------------------------------------------------------------
    struct EngineSettings {
        bool consoleEnabled;

        std::string engineDirectory;
        std::string assetDirectory;
    };

}// namespcae ob