﻿//***********************************************************
//! @file
//! @brief		サイズ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Math.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  サイズ
    //@―---------------------------------------------------------------------------
    struct OB_API Size {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  デフォルトコンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        Size() = default;


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        constexpr Size(s32 width, s32 height)noexcept
            : Size(width, height, 0) {}


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        constexpr Size(s32 width, s32 height, s32 depth)noexcept
            :width(width), height(height), depth(depth) {}


        //===============================================================
        //  オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief          等価演算子
        //@―---------------------------------------------------------------------------
        constexpr bool operator==(const Size& other)const noexcept {
            return
                width == other.width &&
                height == other.height &&
                depth == other.depth;
        }


        //@―---------------------------------------------------------------------------
        //! @brief          否等価演算子
        //@―---------------------------------------------------------------------------
        constexpr bool operator!=(const Size& other)const noexcept {
            return !(*this==other);
        }


        //===============================================================
        // ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  面積
        //@―---------------------------------------------------------------------------
        constexpr s32 area()const noexcept {
            return width * height;
        }


        //@―---------------------------------------------------------------------------
        //! @brief  体積
        //@―---------------------------------------------------------------------------
        constexpr s32 volume()const noexcept {
            return width * height * depth;
        }


        //===============================================================
        // 操作
        //===============================================================


        //@―---------------------------------------------------------------------------
        //! @brief  体積
        //@―---------------------------------------------------------------------------
        constexpr Size& normalize()noexcept {
            width = Math::Abs(width);
            height = Math::Abs(height);
            depth = Math::Abs(depth);
            return *this;
        }

    public:

        s32 width;  //!< 幅
        s32 height; //!< 高さ
        s32 depth;  //!< 深さ(奥行)

    };

}