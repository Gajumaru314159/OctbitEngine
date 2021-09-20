//***********************************************************
//! @file
//! @brief		カラー(R8G8B8A8)
//! @author		Gajumaru
//***********************************************************
#include <Core/Graphic/IntColor.h>


namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief          Color を変換して初期化
    //@―---------------------------------------------------------------------------
    IntColor::IntColor(const Color& another) noexcept {
        Set(
            static_cast<u8>(another.r * 255.0f),
            static_cast<u8>(another.g * 255.0f),
            static_cast<u8>(another.b * 255.0f),
            static_cast<u8>(another.a * 255.0f));
    }


    //@―---------------------------------------------------------------------------
    //! @brief          Vec3 を変換して初期化(アルファは1.0)
    //@―---------------------------------------------------------------------------
    IntColor::IntColor(const Vec3& another) noexcept {
        Set(
            static_cast<u8>(another.x * 255.0f),
            static_cast<u8>(another.y * 255.0f),
            static_cast<u8>(another.z * 255.0f),
            255);
    }


    //@―---------------------------------------------------------------------------
    //! @brief          Vec4 を変換して初期化
    //@―---------------------------------------------------------------------------
    IntColor::IntColor(const Vec4& another) noexcept {
        Set(
            static_cast<u8>(another.x * 255.0f),
            static_cast<u8>(another.y * 255.0f),
            static_cast<u8>(another.z * 255.0f),
            static_cast<u8>(another.w * 255.0f));
    }


    //@―---------------------------------------------------------------------------
    //! @brief          色をカラーコードに変換
    //! 
    //! @param format   カラーフォーマット(色の並び)
    //@―---------------------------------------------------------------------------
    u32 IntColor::ToCode(ColorCodeFormat format)const noexcept {
        switch (format) {
        case ob::ColorCodeFormat::ARGB:
            return (a << 24) | (r << 16) | (g << 8) | (b);
        case ob::ColorCodeFormat::ABGR:
            return (a << 24) | (b << 16) | (g << 8) | (r);
        case ob::ColorCodeFormat::RGBA:
            return (r << 24) | (g << 16) | (b << 8) | (a);
        case ob::ColorCodeFormat::BGRA:
            return (b << 24) | (g << 16) | (r << 8) | (a);
        default:break;
        }
        return 0xFFFFFFFF;
    }

}// namespace ob