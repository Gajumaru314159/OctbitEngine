//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::core {

    //! @brief  コンパイル時定数
    template <typename T, T N> struct ConstValue { static constexpr T value = N; };

}