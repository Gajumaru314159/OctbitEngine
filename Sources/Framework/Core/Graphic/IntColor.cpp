//***********************************************************
//! @file
//! @brief		カラー(R8G8B8A8)
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Graphic/IntColor.h>


namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief          色をカラーコードに変換
    //! 
    //! @param format   カラーフォーマット(色の並び)
    //@―---------------------------------------------------------------------------
    u32 IntColor::toCode(ColorCodeFormat format)const noexcept {
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