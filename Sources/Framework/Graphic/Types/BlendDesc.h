//***********************************************************
//! @file
//! @brief		ブレンド定義
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
    //! @brief      色要素   
    //@―---------------------------------------------------------------------------
    enum class ColorCompoent :u8 {
        Red     = get_bit(0),               //!< 赤
        Green   = get_bit(1),               //!< 緑
        Blue    = get_bit(2),               //!< 青
        Alpha   = get_bit(3),               //!< アルファ
        RGB     = Red | Green | Blue,       //!< カラー
        All = RGB |Alpha,                   //!< 全て
        R = Red,                            //!< Redのエイリアス
        G = Green,                          //!< Greenのエイリアス
        B = Blue,                           //!< Blueのエイリアス
        A = Alpha,                          //!< Alphaのエイリアス
    };
    //@―---------------------------------------------------------------------------
    //! @brief      色マスク
    //@―---------------------------------------------------------------------------
    using ColorMask = bit_flags<ColorCompoent>;


    //@―---------------------------------------------------------------------------
    //! @brief      論理演算
    //! 
    //! @details    現在使用されていません。
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
    //! @brief      ブレンド定義
    //! 
    //! @details    レンダーターゲットのブレンド方法を定義
    //@―---------------------------------------------------------------------------
    struct BlendDesc{
        bool        blendEnable;        //!< ブレンドの有効設定
        BlendFactor srcColorFactor;     //!< ソースカラー係数
        BlendFactor dstColorFactor;     //!< デストカラー係数
        BlendOp     colorOp;            //!< カラーブレンド方法
        BlendFactor srcAlphaFactor;     //!< ソースアルファ係数
        BlendFactor dstAlphaFactor;     //!< デストアルファ係数
        BlendOp     alphaOp;            //!< アルファブレンド方法
        ColorMask   mask;               //!< 書き込みマスク

    public:

        //@―---------------------------------------------------------------------------
        //! @brief      デフォルトコンストラクタ
        //@―---------------------------------------------------------------------------
        BlendDesc() {
            *this = AlphaBlend;
        }

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //@―---------------------------------------------------------------------------
        BlendDesc(
            bool        blendEnable,
            BlendFactor srcColorFactor,
            BlendFactor dstColorFactor,
            BlendOp     colorOp,
            BlendFactor srcAlphaFactor=BlendFactor::One,
            BlendFactor dstAlphaFactor=BlendFactor::Zero,
            BlendOp     alphaOp=BlendOp::Add,
            ColorMask   mask= ColorCompoent::All)
            : blendEnable(blendEnable)
            , srcColorFactor(srcColorFactor)
            , dstColorFactor(dstColorFactor)
            , colorOp(colorOp)
            , srcAlphaFactor(srcAlphaFactor)
            , dstAlphaFactor(dstAlphaFactor)
            , alphaOp(alphaOp)
            , mask(mask) 
        {}

    public:

        static const BlendDesc None;           //!< ブレンドなし(置き換え)
        static const BlendDesc AlphaBlend;     //!< アルファブレンド
        static const BlendDesc Addition;       //!< 加算合成
        static const BlendDesc AdditionAlpha;  //!< アルファあり加算合成
        static const BlendDesc Subtraction;    //!< 減算合成

    };

}// namespcae ob::graphic