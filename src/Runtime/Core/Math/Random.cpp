﻿//***********************************************************
//! @file
//! @brief 乱数生成
//! @author Gajumaru
//***********************************************************
#include <Core/Math/Random.h>


namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief デフォルトコンストラクタ
    //@―---------------------------------------------------------------------------
    Random::Random() {
        x = 123456789;
        y = 362436069;
        z = 521288629;
        w = 88675123;

        // TODO Timeクラスができたら現在の時間から乱数のシード生成
        SetSeed(0);
    }


    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ(シード値指定)
    //@―---------------------------------------------------------------------------
    Random::Random(u32 seed) :Random() {
        SetSeed(seed);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 乱数のシード値を変更
    //@―---------------------------------------------------------------------------
    void Random::SetSeed(u32 seed) {
        do {
            seed = seed * 1812433253 + 1; seed ^= seed << 13; seed ^= seed >> 17;
            x = 123464980 ^ seed;
            seed = seed * 1812433253 + 1; seed ^= seed << 13; seed ^= seed >> 17;
            y = 3447902351 ^ seed;
            seed = seed * 1812433253 + 1; seed ^= seed << 13; seed ^= seed >> 17;
            z = 2859490775 ^ seed;
            seed = seed * 1812433253 + 1; seed ^= seed << 13; seed ^= seed >> 17;
            w = 47621719 ^ seed;
        } while (x == 0 && y == 0 && z == 0 && w == 0);
    }


    //@―---------------------------------------------------------------------------
    //! @brief			乱数のシード値を変更
    //! 
    //! @param seed		シード値
    //@―---------------------------------------------------------------------------
    u32 Random::GetU32() {
        u32 t;
        t = x ^ (x << 11);
        x = y; y = z; z = w;
        w ^= t ^ (t >> 8) ^ (w >> 19);
        return w;
    }


    //@―---------------------------------------------------------------------------
    //! @brief			整数の乱数を生成する
    //! 
    //! @details		minimum以上maximum以下の乱数を生成する
    //! @param minimum	乱数の最小値
    //! @param maximum	乱数の最大値
    //@―---------------------------------------------------------------------------
    s32 Random::Range(s32 minimum, s32 maximum) {
        if (maximum < minimum)return minimum;
        return GetU32() % (maximum - minimum + 1) + minimum;
    }


    //@―---------------------------------------------------------------------------
    //! @brief			小数の乱数を生成する
    //! 
    //! @details		minimum以上maximum以下の乱数を生成する
    //! @param minimum	乱数の最小値
    //! @param maximum	乱数の最大値
    //@―---------------------------------------------------------------------------
    f32 Random::Range(f32 minimum, f32 maximum) {
        GetU32();
        f32 result = ((x + 0.5f) / 4294967296.0f + w) / 4294967296.0f;
        return minimum + result * (maximum - minimum);
    }

} // namespace ob