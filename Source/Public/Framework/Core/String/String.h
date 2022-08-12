//***********************************************************
//! @file
//! @brief		文字列
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CoreTypes.h>
#include <string>
#include <string_view>

namespace ob::core {

    //! @cond
    
    template <class TChar>
    using StringBase = std::basic_string<TChar>;

    template <class TChar>
    using StringViewBase = std::basic_string_view<TChar>;

    using WString = StringBase<wchar_t>;
    using WStringView = StringViewBase<wchar_t>;

    using String = StringBase<Char>;
    using StringView = StringViewBase<Char>;

    //! @endcond

}// namespcae ob::core