//***********************************************************
//! @file
//! @brief f32型算術演算
//! @author Gajumaru
//***********************************************************
#pragma once
#include <math.h>
#include <limits>
#include <gsl/gsl>
#include <Framework/Core/CorePrivate.h>

namespace ob::core {


    //@―---------------------------------------------------------------------------
    //! @brief f32型算術演算
    //@―---------------------------------------------------------------------------
    class Math {
    public:

        //===============================================================
        //	三角関数系
        //===============================================================

        //! @brief fの正弦を返す(Radians)
        static f32 Sin(f32 radians) { return sinf(radians); }

        //! @brief fの正弦を返す(Degrees)
        static f32 SinD(f32 degrees) { return sinf(Math::Degrees(degrees)); }

        //! @brief fの余弦を返す(Radians)
        static f32 Cos(f32 radians) { return cosf(radians); }

        //! @brief fの余弦を返す(Degrees)
        static f32 CosD(f32 degrees) { return cosf(Math::Degrees(degrees)); }

        //! @brief fの正接を返す(Radians)
        static f32 Tan(f32 radians) { return tanf(radians); }

        //! @brief fの正接を返す(Degrees)
        static f32 TanD(f32 degrees) { return tanf(Math::Degrees(degrees)); }

        //! @brief 高速余弦正弦関数
        //! 
        //! @details -PI<radians<PIの時高い精度で結果が得られる。
        static void SinCos(f32 radians, f32& outSin, f32& outCos)noexcept;


        //! @brief fのアークサインを返す(Radians)
        static f32 Asin(f32 f) { return asinf(f); }

        //! @brief fのアークサインを返す(Degrees)
        static f32 AsinD(f32 f) { return Math::Radians(asinf(f)); }

        //! @brief fのアークコサインを返す(Radians)
        static f32 Acos(f32 f) { return acosf(f); }

        //! @brief fのアークコサインを返す(Degrees)
        static f32 AcosD(f32 f) { return Math::Radians(acosf(f)); }

        //! @brief fのアークタンジェントを返す(Radians)
        static f32 Atan(f32 f) { return atanf(f); }

        //! @brief fのアークタンジェントを返す(Degrees)
        static f32 AtanD(f32 f) { return Math::Radians(atanf(f)); }

        //! @brief Tanがy/xになる角度返す(Radians)
        static f32 Atan2(f32 y, f32 x) { return atan2f(y, x); }

        //! @brief Tanがy/xになる角度返す(Degrees)
        static f32 Atan2D(f32 y, f32 x) { return Math::Radians(atan2f(y, x)); }


        //! @brief 度数法をラジアンに変換する
        static constexpr f32 Degrees(f32 degrees)noexcept { return degrees * PI / 180.0f; }

        //! @brief ラジアンを度数法に変換する
        static constexpr f32 Radians(f32 radians)noexcept { return radians * 180.0f / PI; }


        //===============================================================
        //	四捨五入系
        //===============================================================

        //! @brief f以上の最小の整数を返す
        static f32 Ceil(f32 f) { return ceilf(f); }

        //! @brief f以下の最大の整数を返す
        static f32 Floor(f32 f) { return floorf(f); }

        //! @brief fを四捨五入した値を返す
        static f32 Round(f32 f) { return roundf(f); }


        //===============================================================
        //	補間系
        //===============================================================

        //! @brief aとbをtで線形補間する
        static constexpr f32 Lerp(f32 a, f32 b, f32 t)noexcept { return a * (1 - t) + b * t; }

        //! @brief aとbの角度をtで線形補間する
        static f32 LerpAngle(f32 a, f32 b, f32 t) { return fmodf(a + fmodf(b - a, TWO_PI) * t, TWO_PI); }

        //! @brief [a,b] の範囲内で補間する値valueを生成する線形パラメータtを計算する
        static constexpr f32 InverseLerp(f32 a, f32 b, f32 f)noexcept { return (b - a) / (f - a); }

        //! @brief tを[a,b]から[c,d]に変換する
        static constexpr f32 Map(f32 a, f32 b, f32 c, f32 d, f32 t)noexcept { return IsNearEquals(b - a, 0.0f) ? c : ((t - a) / (b - a) * (d - c) + c); }


        //===============================================================
        //	Min / Max
        //===============================================================

        //! @brief 2つの値から小さい値を返す
        template<typename T>
        static constexpr f32 Min(T a, T b)noexcept { return ((a) < (b)) ? (a) : (b); }

        //! @brief 3つの値から一番小さい値を返す
        template<typename T>
        static constexpr f32 Min(T a, T b, T c)noexcept { return Min(Min(a, b), c); }

        //! @brief 4つの値から一番小さい値を返す
        template<typename T>
        static constexpr f32 Min(T a, T b, T c, T d)noexcept { return Min(Min(a, b), Min(c, d)); }

        //! @brief 配列の中から一番小さい値を返す
        template<typename T>
        static constexpr T Min(gsl::span<const T> values)noexcept;

        //! @brief 2つの値から大きい値を返す
        template<typename T>
        static constexpr T Max(T a, T b)noexcept { return ((a) < (b)) ? (b) : (a); }

        //! @brief 3つの値から一番大きい値を返す
        template<typename T>
        static constexpr T Max(T a, T b, T c)noexcept { return Max(Max(a, b), c); }

        //! @brief 4つの値から一番大きい値を返す
        template<typename T>
        static constexpr T Max(T a, T b, T c, T d)noexcept { return Max(Max(a, b), Max(c, d)); }

        //! @brief 配列の中から一番大きい値を返す
        template<typename T>
        static constexpr T Max(gsl::span<const T> values)noexcept;


        //===============================================================
        //	判定
        //===============================================================

        //! @brief 2つの浮動小数点値を比較し近似している場合はtrueを返す
        static constexpr bool IsNearEquals(f32 a, f32 b, f32 tolerance = Math::TOLERANCE)noexcept { return Abs(a - b) < tolerance; }

        //! @brief 値が0に近い場合はtrueを返す
        static constexpr bool IsNearZero(f32 a, f32 tolerance = Math::TOLERANCE)noexcept { return IsNearEquals(a, 0.0f, tolerance); }

        //! @brief 値が無効な数字か判定する
        static bool IsNaN(f32 value);

        //! @brief 値が無限か判定する
        static bool IsInf(f32 value);


        //===============================================================
        //	その他
        //===============================================================

        //! @brief fの絶対値を返す
        template<typename T>
        static constexpr T Abs(T f)noexcept { return f < 0 ? -f : f; }

        //! @brief fをminimumとmaximumの間に収まるように返す
        template<typename T>
        static constexpr T Clamp(T f, T minimum, T maximum)noexcept { return std::clamp(f, maximum, minimum); }

        //! @brief fを0と1の間に収まるように返す
        template<typename T>
        static constexpr T Clamp01(T f)noexcept { return std::clamp<T>(f,0,1); }

        //! @brief toとfromの間の角度をラジアンで返す
        static f32 BetweenAngle(f32 to, f32 from) { return abs(fmodf(from - to, TWO_PI)); }

        //! @brief fのexpを返す
        static f32 Exp(f32 f) { return expf(f); }

        //! @brief fの小数部を取得する
        static f32 Fract(f32 f) { return f - floorf(f); }

        //! @brief fの対数を返す
        static f32 Log(f32 f) { return logf(f); }

        //! @brief 底を10としたfの対数を返す
        static f32 Log10(f32 f) { return log10f(f); }

        //! @brief 0と1の間を往復する値を返す
        static f32 PingPong(f32 f) { return abs(f * 0.5f - (f32)ceil(f * 0.5) - 0.5f); }

        //! @brief fのp乗を返す
        static f32 Pow(f32 f, f32 p) { return powf(f, p); }

        //! @brief fの符号を返す
        template<typename T,std::enable_if_t<std::is_signed_v<T>>>
        static constexpr T Sign(T f)noexcept { if (f == 0)return 0; return 0 < f ? 1 : -1; }

        //! @brief fの二乗を返す
        template<typename T>
        static constexpr T Square(T f)noexcept { return f * f; }

        //! @brief fの根(ルート)を返す
        static f32 Sqrt(f32 f) { return sqrtf(f); }

        //! @brief fの根(ルート)を返す(高速版)
        static constexpr f32 SqrtFast(f32 f)noexcept {
            f32 xHalf = 0.5f * f;
            s32   tmp = 0x5F3759DF - (*(s32*)&f >> 1);
            f32 xRes = *(f32*)&tmp;

            xRes *= (1.5f - (xHalf * xRes * xRes));		// 1回目
            xRes *= (1.5f - (xHalf * xRes * xRes));		// 2回目
            return xRes * f;
        }

        //! @brief 割ったあまり
        static f32 Mod(f32 f, f32 mod)noexcept;

        //! @brief fの根(ルート)を返す(高速版)
        static f32 Wrap(f32 f, f32 minval, f32 maxVal)noexcept;

    public:

        static constexpr f32 PI             = 3.1415926535897932384626433832795f;   //!< 円周率
        static constexpr f32 TWO_PI         = Math::PI * 2.0f;                      //!< 円周率の2倍
        static constexpr f32 HALF_PI        = Math::PI * 0.5f;                      //!< 円周率の半分
        static constexpr f32 INV_PI         = 1.0f / Math::PI;                      //!< 円周率の逆数
        static constexpr f32 INV_TWO_PI     = 1.0f / Math::TWO_PI;                  //!< 円周率の2倍の逆数
        static constexpr f32 INV_HALF_PI    = 1.0f / Math::HALF_PI;                 //!< 円周率の半分の逆数
        static constexpr f32 EPSILON        = FLT_EPSILON;                          //!< ごくわずかな浮動小数点の値
        static constexpr f32 NAPIER         = 2.71828182845904523536f;              //!< ネイピア数
        static constexpr f32 GOLDEN_RATIO   = 1.6180339887498948482045868343656381f;//!< 黄金比
        static constexpr f32 INF            = std::numeric_limits<f32>::infinity(); //!< 無限
        static constexpr f32 NaN            = std::numeric_limits<f32>::quiet_NaN();//!< 無効値
        static constexpr f32 TOLERANCE      = 0.00001f;                             //!< 誤差許容値

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    // @brief 高速余弦正弦関数
    inline void Math::SinCos(f32 radians, f32& outSin, f32& outCos)noexcept {
        radians = Math::Wrap(radians, -PI, PI);

        static f32 waru[8] = { -1.0f / (3 * 4 * 5 * 6 * 7 * 8),-1.0f / (2 * 3 * 4 * 5 * 6 * 7),1.0f / (3 * 4 * 5 * 6),1.0f / (2 * 3 * 4 * 5),-1.0f / (3 * 4),-1.0f / (2 * 3),1.0f,1.0f };
        f32 c = 1.0f / (3 * 4 * 5 * 6 * 7 * 8 * 9 * 10);
        f32 s = 1.0f / (2 * 3 * 4 * 5 * 6 * 7 * 8 * 9);
        f32* p = waru;
        f32 z;

        s32 i;
        radians *= (1.0f / 32.0);
        z = radians * radians;
        do {
            c = c * z + (*p);
            p++;
            s = s * z + (*p);
            p++;
        } while (p < waru + 8);
        c = c * z;
        s = s * radians;
        for (i = 0; i < 5; i++) {
            s = s * (2.0f - c);
            c = c * (4.0f - c);
        }
        outSin = s;
        outCos = 1.0f - c / 2.0f;
    }


    // @brief 配列の中から一番大きい値を返す
    template<typename T>
    constexpr T Math::Max(gsl::span<const T> values)noexcept {
        f32 ans = values.front();
        for (f32 value : values) {
            ans = Math::Max(ans, value);
        }
        return ans;
    }


    // @brief 配列の中から一番小さい値を返す
    template<typename T>
    constexpr T Math::Min(gsl::span<const T> values)noexcept {
        f32 ans = values.front();
        for (f32 value : values) {
            ans = Math::Min(ans, value);
        }
        return ans;
    }

    inline f32 Math::Mod(f32 f, f32 mod)noexcept {
        return fmod(mod + fmod(f, mod), mod);
    }

    inline f32 Math::Wrap(f32 f, f32 minVal, f32 maxVal)noexcept {
        return minVal + Mod(f - minVal, maxVal - minVal);
    }


    //! @endcond
}// namespace ob