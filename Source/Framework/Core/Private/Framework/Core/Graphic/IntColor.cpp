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
    const IntColor IntColor::Normal = { 128,128,255,255 };

    //@―---------------------------------------------------------------------------
    //! @brief			カラーコードから生成
    //! @param code		カラーコード
    //! @param format	カラーコードの並び
    //@―---------------------------------------------------------------------------
    IntColor IntColor::FromCode(u32 code,ColorCodeFormat format) {
        u32 c0 = (code >> 24) & 0xFF;
        u32 c1 = (code >> 16) & 0xFF;
        u32 c2 = (code >> 8) & 0xFF;
        u32 c3 = (code >> 0) & 0xFF;
        switch (format) {
        case ColorCodeFormat::ABGR:
            return IntColor(c3, c2, c1, c0);
        case ColorCodeFormat::RGBA:
            return IntColor(c0, c1, c2, c3);
        case ColorCodeFormat::BGRA:
            return IntColor(c2, c1, c0, c3);
        default:
            return IntColor(c1, c2, c3, c0);
        }
    }

}// namespace ob