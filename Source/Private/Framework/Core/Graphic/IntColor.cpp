//***********************************************************
//! @file
//! @brief		カラー(R8G8B8A8)
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Graphic/IntColor.h>


namespace ob::core {

    const IntColor IntColor::White = { 255,255,255,255 };
    const IntColor IntColor::Gray = { 128,128,128,255 };
    const IntColor IntColor::Black = { 0,0,0,255 };
    const IntColor IntColor::Clear = { 0,0,0,0 };
    const IntColor IntColor::Red = { 255,0,0,255 };
    const IntColor IntColor::Yellow = { 255,255,0,255 };
    const IntColor IntColor::Green = { 0,255,0,255 };
    const IntColor IntColor::Cyan = { 0,255,255,255 };
    const IntColor IntColor::Blue = { 0,0,255,255 };
    const IntColor IntColor::Magenta = { 255,0,255,255 };
    const IntColor IntColor::Normal = { 128,255,128,255 };

    //@―---------------------------------------------------------------------------
    //! @brief          色をカラーコードに変換
    //! 
    //! @param format   カラーフォーマット(色の並び)
    //@―---------------------------------------------------------------------------
    u32 IntColor::toCode(ColorCodeFormat format)const noexcept {
        switch (format) {
        case ColorCodeFormat::ARGB:
            return (a << 24) | (r << 16) | (g << 8) | (b);
        case ColorCodeFormat::ABGR:
            return (a << 24) | (b << 16) | (g << 8) | (r);
        case ColorCodeFormat::RGBA:
            return (r << 24) | (g << 16) | (b << 8) | (a);
        case ColorCodeFormat::BGRA:
            return (b << 24) | (g << 16) | (r << 8) | (a);
        default:break;
        }
        return 0xFFFFFFFF;
    }

}// namespace ob