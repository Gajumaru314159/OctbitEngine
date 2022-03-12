//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Foundation/String/String.h>

//! @cond
// Check if fmt/format.h compiles with the X11 index macro defined.
#define index(x, y) no nice things
#include <fmt/format.h>
#undef index
//! @endcond


namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    template<typename TChar,class... Args>
    ob::StringBase<TChar> format(const TChar* fmt, Args&&... args) {
        return fmt::format(fmt, ob::forward<Args>(args)...);
    }





    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob