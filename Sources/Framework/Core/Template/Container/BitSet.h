//***********************************************************
//! @file
//! @brief		ビット・セット
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <bitset>

namespace ob::core {

    //! @brief ビットセット
    template<size_t N>
    using BitSet = std::bitset<N>;

}// namespcae ob