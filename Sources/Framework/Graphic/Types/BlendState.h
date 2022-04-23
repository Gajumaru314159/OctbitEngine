//***********************************************************
//! @file
//! @brief		ブレンド・ステート
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


    //@―---------------------------------------------------------------------------
    //! @brief      ブレンド・オペレータ
    //! 
    //! @details    ピクセルシェーダが出力するRGBまたはAのレンダーターゲットへのブレンド方法です。
    //! @see        BlendState    
    //@―---------------------------------------------------------------------------
    enum class BlendOp :u32 {
        Add,        //!< A+B
        Sub,        //!< A-B
        RevSub,     //!< B-A
        Min,        //!< min(A,B)
        Max,        //!< max(A,B)
    };


    //@―---------------------------------------------------------------------------
    //! @brief      論理演算
    //! 
    //! @see        BlendState    
    //@―---------------------------------------------------------------------------
    enum class LogicOp :u32 {
        Clear,			//!< 0
        Set,			//!< 1
        Copy,			//!< src
        CopyInverted,	//!< ~src
        Noop,			//!< 何もしない
        Invert,			//!< ~dst
        And,			//!< dst & src
        Nand,			//!< ~(dst & src)
        Or,				//!< dst | src
        Nor,			//!< ~(dst | src)
        Xor,			//!< dst ^ src
        Equivalent,		//!< ~(dst ^ src)
        AndReverse,		//!< src & ~dst
        AndInverted,	//!< ~src & dst
        OrReverse,		//!< dst | ~dst
        OrInverted,		//!< ~src | dst
    };


    //@―---------------------------------------------------------------------------
    //! @brief      ブレンド・ステート
    //! 
    //! @details    レンダーターゲットのブレンド方法を定義
    //@―---------------------------------------------------------------------------
    struct BlendState{
        bool        blendEnable;        //!< ブレンドの有効設定
        BlendFactor srcColorFactor;     //!< ソースカラー係数
        BlendFactor dstColorFactor;     //!< デストカラー係数
        BlendOp     colorOp;            //!< カラーブレンド方法
        BlendFactor srcAlphaFactor;     //!< ソースアルファ係数
        BlendFactor dstAlphaFactor;     //!< デストアルファ係数
        BlendOp     alphaOp;            //!< アルファブレンド方法
        bool        logicEnable;        //!< 論理演算の有効設定
        LogicOp     logicOp;            //!< 論理演算方法
        u32         mask;               //!< 書き込みマスク

        static const BlendState None;           //!< ブレンドなし(置き換え)
        static const BlendState AlphaBlend;     //!< アルファブレンド
        static const BlendState Addition;       //!< 加算合成
        static const BlendState AdditionAlpha;  //!< アルファあり加算合成
        static const BlendState Subtraction;    //!< 減算合成
    };

}// namespcae ob::graphic