//***********************************************************
//! @file
//! @brief		ブレンド・ステート
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/BlendFactor.h>
#include <Framework/Graphic/Types/BlendOp.h>
#include <Framework/Graphic/Types/LogicOp.h>

namespace ob::graphic {

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

        static const BlendState None;
        static const BlendState AlphaBlend;
        static const BlendState Addition;
        static const BlendState AdditionAlpha;
        static const BlendState Subtraction;
    };

}// namespcae ob::graphic