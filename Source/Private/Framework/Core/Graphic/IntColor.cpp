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

}// namespace ob