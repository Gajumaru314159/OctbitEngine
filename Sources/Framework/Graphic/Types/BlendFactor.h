//***********************************************************
//! @file
//! @brief		ブレンド係数
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      ブレンド係数
    //! 
    //! @see        BlendState    
    //@―---------------------------------------------------------------------------
    enum class BlendFactor :u32 {
        Zero,                       //!< 0
        One,                        //!< 1
        SrcColor,                   //!< src.col
        OneMinusSrcColor,           //!< 1-src.col
        DstColor,                   //!< dst.col
        OneMinusDstColor,           //!< 1-dst.col
        SrcAlpha,                   //!< src.a
        OneMinusSrcAlpha,           //!< 1-src.a
        DstAlpha,                   //!< dst.a
        OneMinusDstAlpha,           //!< 1-dst.a
        ConstantColor,              //!< 
        OneMinusConstantColor,      //!< 
        ConstantAlpha,              //!< 
        OneMinusConstantAlpha,      //!< 
        SrcAlphaSaturate,           //!<
        Src1Color,                  //!< 
        OneMinusSrc1Color,          //!< 
        Src1Alpha,                  //!< 
        OneMinusSrc1Alpha,          //!< 
    };

}// namespcae ob::graphic