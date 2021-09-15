//***********************************************************
//! @file
//! @brief		文字列クラス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Core/Base/BaseType.hpp>

#pragma warning ( push )
#pragma warning ( disable : 26439 )
#include <EASTL/string.h>
#include <EASTL/fixed_string.h>
#include <EASTL/string_view.h>
#include <EASTL/string_map.h>
#pragma warning ( pop )

namespace ob
{

    using String = ::eastl::basic_string<Char>;                     //!< 文字列

    template<Size tSize>
    using FixedString = ::eastl::fixed_string<Char,tSize,FALSE>;    //!< 固定長文字列

    using StringView = ::eastl::basic_string_view<Char>;            //!< 参照文字列




    //===============================================================
    // インライン関数
    //===============================================================

    //-----------------------------------------------------------


}// namespcae ob