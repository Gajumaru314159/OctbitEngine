//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <cstdio>
#include <cstring>
#include <utility>

namespace ob::core::CAPI {

    //! @brief sscanf の安全ラッパー（MSVCのセキュリティ警告対策）
    template<class... Args>
    inline int SScanf(const char* buffer, const char* format, Args&&... args) {
#if defined(_MSC_VER)
        return ::sscanf_s(buffer, format, std::forward<Args>(args)...);
#else
        return std::sscanf(buffer, format, std::forward<Args>(args)...);
#endif
    }


    //! @brief fopen の安全ラッパー（MSVCのセキュリティ警告対策）
    inline FILE* FOpen(const char* filename, const char* mode) {
#if defined(_MSC_VER)
        FILE* handle = nullptr;
        if (::fopen_s(&handle, filename, mode) != 0) {
            return nullptr;
        }
        return handle;
#else
        return std::fopen(filename, mode);
#endif
    }


    //! @brief strerror の安全ラッパー
    inline bool StrError(int errnum, char* buffer, size_t bufferSize) {
        if (buffer == nullptr || bufferSize == 0) return false;
#if defined(_MSC_VER)
        return ::strerror_s(buffer, bufferSize, errnum) == 0;
#else
        const char* msg = std::strerror(errnum);
        if (msg == nullptr) {
            buffer[0] = '\0';
            return false;
        }
        std::strncpy(buffer, msg, bufferSize);
        buffer[bufferSize - 1] = '\0';
        return true;
#endif
    }

}
