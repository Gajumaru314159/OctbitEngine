﻿//***********************************************************
//! @file
//! @brief 乱数生成
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Core/Base/Fwd.hpp>

namespace ob
{

    //-----------------------------------------------------------
    //! @brief 乱数生成器
    //-----------------------------------------------------------
    class OB_API Random
    {
    public:

        Random();			                    // デフォルトコンストラクタ
        Random(u32 seed);	                    // コンストラクタ(シード値指定)

        void SetSeed(u32 seed);					// 乱数のシード値を変更
        u32  GetU32();							// 0から2^32までの整数の乱数を生成する
        s32  Range(s32 minimum, s32 maximum);	// 整数の乱数を生成する
        f32  Range(f32 minimum, f32 maximum);	// 小数の乱数を生成する

    private:

        u32 x, y, z, w;

    };
} // namespace ob