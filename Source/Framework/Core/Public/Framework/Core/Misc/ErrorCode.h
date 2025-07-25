//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CoreTypes.h>
#include <Framework/Core/String/String.h>

namespace ob::core {

    //! @brief エラーコード
    class ErrorCode {
    public:
        ErrorCode() : m_value(0) {}
        ErrorCode(u32 value) : m_value(value) {}

        u32 value() const { return m_value; }
        String message() const;

    private:
        u32 m_value;
    };

}

//===============================================================
// フォーマット
//===============================================================
//! @cond
template <> struct std::formatter<ob::core::ErrorCode, ob::core::Char> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }

    template<typename FormatContext>
    auto format(ob::core::ErrorCode value, FormatContext& ctx) const {
        return format_to(ctx.out(), "0x{:08X} {}", value.value(), value.message().c_str());
    }
};