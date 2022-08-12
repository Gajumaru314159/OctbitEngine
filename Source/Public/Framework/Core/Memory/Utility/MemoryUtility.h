//***********************************************************
//! @file
//! @brief		アドレスユーティリティ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CoreTypes.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  入力ポインタからオフセットバイト進んだポインタを取得
    //@―---------------------------------------------------------------------------
    template<typename T>
    constexpr T* GetOffsetPtr(const void* pBuffer, s32 offset)noexcept {
        return reinterpret_cast<T*>(reinterpret_cast<byte*>(const_cast<void*>(pBuffer)) + offset);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  入力ポインタからオフセットバイト進んだポインタを取得
    //@―---------------------------------------------------------------------------
    constexpr void* GetOffsetPtr(const void* pBuffer, s32 offset)noexcept {
        return GetOffsetPtr<byte>(pBuffer, offset);
    }


    //@―---------------------------------------------------------------------------
    //! @brief nullチェック付きretain呼び出し
    //@―---------------------------------------------------------------------------
    template<class T>
    inline void SafeRetain(T* ptr)noexcept(ptr->retain()) {
        if (ptr) {
            ptr->retain();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief nullチェック付き解放release呼び出し
    //@―---------------------------------------------------------------------------
    template<class T>
    inline void SafeRelease(T*& pBuffer)noexcept(ptr->release()) {
        if (ptr) {
            ptr->release();
            ptr = nullptr;
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief deleteによるnullチェック付き解放
    //@―---------------------------------------------------------------------------
    template<class T>
    inline void SafeDelete(T*& pBuffer) {
        if (pBuffer) {
            delete pBuffer;
            pBuffer = nullptr;
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief delete[]によるnullチェック付き解放
    //@―---------------------------------------------------------------------------
    template<class T>
    inline void SafeDeleteArray(T*& pBuffer) {
        if (pBuffer) {
            delete[] pBuffer;
            pBuffer = nullptr;
        }
    }

}// namespcae ob