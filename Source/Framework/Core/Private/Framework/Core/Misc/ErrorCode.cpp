//***********************************************************
//! @file
//! @brief Blob
//! @author Gajumaru
//***********************************************************
#include <Framework/Core/Misc/ErrorCode.h>
#include <Framework/Core/Platform/WindowsHeaders.h>
#include <Framework/Core/String/StringEncoder.h>

namespace ob::core {

    String ErrorCode::message() const {
#ifdef OS_WINDOWS
        WCHAR buffer[256];
        DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
        auto result = ::FormatMessageW(
            dwFlags,
            nullptr,
            m_value,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            buffer,
            (DWORD)std::size(buffer),
            nullptr
        );

        if (0 == result) {
            String message;
            StringEncoder::Encode(buffer, message);
            return "不明なエラー";
        }
        String message;
        StringEncoder::Encode(buffer, message);
        if (2 <= message.size())message.resize(message.size() - 2);
        return message;
#else
        return "";
#endif
    }

} 