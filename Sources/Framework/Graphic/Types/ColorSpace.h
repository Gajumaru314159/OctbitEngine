//***********************************************************
//! @file
//! @brief		カラースペース
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  カラースペース
    //! 
    //! @details    ||DirectX|Vulkan|
    //!             |-|-|-|
    //!             |BT709_G22      ,DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709|VK_COLOR_SPACE_BT709_NONLINEAR_EXT |
    //!             |BT709_G10      ,DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709|VK_COLOR_SPACE_BT709_LINEAR_EXT |
    //!             |BT2020_G2084   ,DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020|VK_COLOR_SPACE_BT2020_LINEAR_EXT |
    //!             |BT2100_G22     ,DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P2020|VK_COLOR_SPACE_HDR10_ST2084_EXT |
    //!             ガンマカーブにはHLG(Hybrid Log-Gamma)とPG(Perceptual Quantizer)が存在します。
    //!             [参考](https://ja.wikipedia.org/wiki/ハイダイナミックレンジビデオ)
    //@―---------------------------------------------------------------------------
    enum class ColorSpace {
        BT709_G22,      //!< SDR(8bit) Gamma=2.2
        BT709_G10,      //!< SDR(8bit) Gamma=1.0
        BT2020_G2084,   //!< HDR(10bit/12bit) SMPTE ST 2084
        BT2020_G22,     //!< HDR(10bit/12ibt) Gamma=2.2

        HDR10= BT2020_G2084,    //!< BT2020_G2084 のエイリアス
    };


}// namespcae ob::graphic