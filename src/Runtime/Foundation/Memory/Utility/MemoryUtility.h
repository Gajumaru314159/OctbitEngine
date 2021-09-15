//***********************************************************
//! @file
//! @brief		アドレスユーティリティ
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob {

    //-----------------------------------------------------------------------------
    //! @brief  入力ポインタからオフセットバイト進んだポインタの取得
    //-----------------------------------------------------------------------------
    template<typename T>
    constexpr T* GetOffsetPtr(const void* pBuffer, s32 offset)noexcept     {
        return reinterpret_cast<T*>(reinterpret_cast<byte*>(const_cast<void*>(pBuffer)) + offset);
    }

    constexpr void* GetOffsetPtr(const void* pBuffer, s32 offset)noexcept     {
        return GetOffsetPtr<byte>(pBuffer, offset);
    }


    //-----------------------------------------------------------------------------
    //! @brief  アライメント分offset下ポインタの取得
    //-----------------------------------------------------------------------------



    //-----------------------------------------------------------------------------
    //! @brief deleteによるnullチェック付き解放
    //-----------------------------------------------------------------------------
    template<class T>
    inline void SafeDelete(T*& pBuffer)     {
        if (pBuffer) {
            delete pBuffer;
            pBuffer = nullptr;
        }
    }


    //-----------------------------------------------------------------------------
    //! @brief delete[]によるnullチェック付き解放
    //-----------------------------------------------------------------------------
    template<class T>
    inline void SafeDeleteArray(T*& pBuffer)     {
        if (pBuffer) {
            delete[] pBuffer;
            pBuffer = nullptr;
        }
    }

    //-----------------------------------------------------------------------------
    //! @brief                  値を一番近いアライメントの倍数に増やして合わせる
    //!
    //! @param[in] val          位置合わせするアライメント値
    //! @param[in] alignment    アライメントの値(2の累乗)
    //! @return                 アラインされた値
    //-----------------------------------------------------------------------------
    template<typename T>
    inline constexpr T Align(T val, size_t alignment) {
        return (T)(((size_t)val + alignment - 1) & (~(alignment - 1)));
    }


    //-----------------------------------------------------------------------------
    //! @brief                  値を一番近いアライメントの倍数に減らして合わせる
    //!
    //! @param[in] val          位置合わせするアライメント値
    //! @param[in] alignment    アライメントの値(2の累乗)
    //! @return                 アラインされた値
    //-----------------------------------------------------------------------------
    template<typename T>
    inline constexpr T AlignDown(T val, size_t alignment) {
        return (T)(((size_t)val) & (~(alignment - 1)));
    }


    //-----------------------------------------------------------------------------
    //! @brief                  値を一番近いアライメントの倍数に合わせる
    //!
    //! @param[in] val          位置合わせするアライメント値
    //! @param[in] alignment    アライメントの値(2の累乗)
    //! @return                 アラインされた値
    //-----------------------------------------------------------------------------
    template<typename T>
    inline constexpr T AlignArbitary(T val, size_t alignment) {
        return (T)((((size_t)val + alignment - 1) / alignment) * alignment);
    }


    //-----------------------------------------------------------------------------
    //! @brief                  値がアライメントの倍数か判定
    //!
    //! @param[in] val          判定する値
    //! @param[in] alignment    アライメントの値(2の累乗)
    //! @retval TRUE            アライメントされている
    //! @retval FALSE           アライメントされていない
    //-----------------------------------------------------------------------------
    template<typename T>
    inline constexpr bool IsAligned(T val, size_t alignment) {
        return !((size_t)val & (alignment - 1));
    }

}// namespcae ob