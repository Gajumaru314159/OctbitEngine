﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

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
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        Size() = default;


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        Size(s32 width, s32 height)noexcept
            : Size(width,height,0) {}


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        Size(s32 width, s32 height,s32 depth)noexcept
            :width(width), height(height),depth(depth) {}


        //===============================================================
        // ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  面積
        //@―---------------------------------------------------------------------------
        s32 area()const noexcept {
            return width * height;
        }


        //@―---------------------------------------------------------------------------
        //! @brief  体積
        //@―---------------------------------------------------------------------------
        s32 volume()const noexcept {
            return width * height * depth;
        }


        //===============================================================
        // 操作
        //===============================================================


        //@―---------------------------------------------------------------------------
        //! @brief  体積
        //@―---------------------------------------------------------------------------
        Size& normalize()noexcept {
            width = abs(width);
            height = abs(height);
            depth = abs(depth);
        }

    public:

        s32 width;  //!< 幅
        s32 height; //!< 高さ
        s32 depth;  //!< 深さ(奥行)

    };

}// namespcae ob