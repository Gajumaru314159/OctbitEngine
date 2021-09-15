//***********************************************************
//! @file
//! @brief f32型算術演算
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Core/Base/Fwd.hpp>

#include <math.h>

namespace ob
{


	//-----------------------------------------------------------
	//! @brief f32型算術演算
	//-----------------------------------------------------------
	class OB_API Mathf
	{
	public:

		//===============================================================
		//	三角関数系
		//===============================================================

		//! @brief fの正弦を返す(Radians)
		static f32 Sin(f32 radians) { return sinf(radians); }

		//! @brief fの正弦を返す(Degrees)
		static f32 SinD(f32 degrees) { return sinf(Mathf::Degrees(degrees)); }

		//! @brief fの余弦を返す(Radians)
		static f32 Cos(f32 radians) { return cosf(radians); }

		//! @brief fの余弦を返す(Degrees)
		static f32 CosD(f32 degrees) { return cosf(Mathf::Degrees(degrees)); }

		//! @brief fの正接を返す(Radians)
		static f32 Tan(f32 radians) { return tanf(radians); }

		//! @brief fの正接を返す(Degrees)
		static f32 TanD(f32 degrees) { return tanf(Mathf::Degrees(degrees)); }

		//! @brief 高速余弦正弦関数
		//! 
		//! @details -PI<radians<PIの時高い精度で結果が得られる。
		static f32 SinCos(f32 radians, f32& outSin, f32& outCos);


		//! @brief fのアークサインを返す(Radians)
		static f32 Asin(f32 f) { return asinf(f); }

		//! @brief fのアークサインを返す(Degrees)
		static f32 AsinD(f32 f) { return Mathf::Radians(asinf(f)); }

		//! @brief fのアークコサインを返す(Radians)
		static f32 Acos(f32 f) { return acosf(f); }

		//! @brief fのアークコサインを返す(Degrees)
		static f32 AcosD(f32 f) { return Mathf::Radians(acosf(f)); }

		//! @brief fのアークタンジェントを返す(Radians)
		static f32 Atan(f32 f) { return atanf(f); }

		//! @brief fのアークタンジェントを返す(Degrees)
		static f32 AtanD(f32 f) { return Mathf::Radians(atanf(f)); }

		//! @brief Tanがy/xになる角度返す(Radians)
		static f32 Atan2(f32 y, f32 x) { return atan2f(y, x); }

		//! @brief Tanがy/xになる角度返す(Degrees)
		static f32 Atan2D(f32 y, f32 x) { return Mathf::Radians(atan2f(y, x)); }


		//! @brief ディグリーをラジアンに変換する
		static f32 Degrees(f32 degrees) { return degrees * PI / 180.0f; }

		//! @brief ラジアンをディグリーに変換する
		static f32 Radians(f32 radians) { return radians * 180.0f / PI; }

		
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
		static f32 Lerp(f32 a, f32 b, f32 t) { return a * (1 - t) + b * t; }

		//! @brief aとbの角度をtで線形補間する
		static f32 LerpAngle(f32 a, f32 b, f32 t) { return fmodf(a + fmodf(b - a, TWO_PI) * t, TWO_PI); }

		//! @brief [a,b] の範囲内で補間する値valueを生成する線形パラメータtを計算する
		static f32 InverseLerp(f32 a, f32 b, f32 f) { return (b - a) / (f - a); }


		//===============================================================
		//	Min / Max
		//===============================================================

		//! @brief 2つの値から小さい値を返す
		static f32 Min(f32 a, f32 b) { return ((a) < (b)) ? (a) : (b); }

		//! @brief 3つの値から一番小さい値を返す
		static f32 Min(f32 a, f32 b, f32 c);

		//! @brief 4つの値から一番小さい値を返す
		static f32 Min(f32 a, f32 b, f32 c, f32 d);

		//! @brief 配列の中から一番小さい値を返す
		static f32 Min(const f32* values, s32 size);

		//! @brief 2つの値から大きい値を返す
		static f32 Max(f32 a, f32 b) { return ((a) < (b)) ? (b) : (a); }

		//! @brief 3つの値から一番大きい値を返す
		static f32 Max(f32 a, f32 b, f32 c);

		//! @brief 4つの値から一番大きい値を返す
		static f32 Max(f32 a, f32 b, f32 c, f32 d);

		//! @brief 配列の中から一番大きい値を返す
		static f32 Max(const f32* values, s32 size);


		//===============================================================
		//	判定
		//===============================================================

		//! @brief 2つの浮動小数点値を比較し近似している場合はtrueを返す
		static b32 Approximately(f32 a, f32 b, f32 tolerance = Mathf::TOLERANCE) { return Abs(a - b) < tolerance; }

		//! @brief 値が無効な数字か判定する
		static b32 IsNaN(f32 value);

		//! @brief 値が無限か判定する
		static b32 IsInf(f32 value);


		//===============================================================
		//	その他
		//===============================================================

		//! @brief fの絶対値を返す
		static f32 Abs(f32 f) { return abs(f); }

		//! @brief fをminimumとmaximumの間に収まるように返す
		static f32 Clamp(f32 f, f32 minimum, f32 maximum) { return OB_CLAMP(f, maximum, minimum); }

		//! @brief fを0と1の間に収まるように返す
		static f32 Clamp01(f32 f) { return Mathf::Clamp(f, 0, 1); }

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
		static f32 Sign(f32 f) { if (f == 0.0f)return 0; return 0.0f<f?1.0f:-1.0f; }

		//! @brief fの二乗を返す
		static f32 Square(f32 f) { return f*f; }

		//! @brief fの根(ルート)を返す
		static f32 Sqrt(f32 f) { return sqrtf(f); }

		//! @brief fの根(ルート)を返す(高速版)
		static f32 SqrtFast(f32 f)
		{
			f32 xHalf = 0.5f * f;
			s32   tmp = 0x5F3759DF - (*(s32*)&f >> 1);
			f32 xRes = *(f32*)&tmp;

			xRes *= (1.5f - (xHalf * xRes * xRes));			// 1回目
			xRes *= ( 1.5f - ( xHalf * xRes * xRes ) );		// 2回目
			return xRes * f;
		}

	public:

		static const f32 PI;			//!< 円周率
		static const f32 TWO_PI;		//!< 円周率の2倍
		static const f32 HALF_PI;		//!< 円周率の半分
		static const f32 INV_PI;		//!< 円周率の逆数
		static const f32 INV_TWO_PI;	//!< 円周率の2倍の逆数
		static const f32 INV_HALF_PI;	//!< 円周率の半分の逆数
		static const f32 EPSILON;		//!< ごくわずかな浮動小数点の値
		static const f32 NAPIER;		//!< ネイピア数
		static const f32 GOLDEN_RATIO;	//!< 黄金比
		static const f32 INF;			//!< 無限
		static const f32 NaN;			//!< 無効値
		static const f32 TOLERANCE;		//!< 誤差許容値

	};


#define DEG(degrees) (Mathf::Degrees(degrees))
#define RAD(radians) (Mathf::Radians(radians))




	//===============================================================
	// インライン関数
	//===============================================================

	// @brief 高速余弦正弦関数
	inline f32 Mathf::SinCos(f32 radians, f32& outSin, f32& outCos)
	{
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

	// @brief 3つの値から一番大きい値を返す
	inline f32 Mathf::Max(f32 a, f32 b, f32 c)
	{
		f32 result = a;
		if (result < b)result = b;
		if (result < c)result = c;
		return result;
	}


	// @brief 4つの値から一番大きい値を返す
	inline f32 Mathf::Max(f32 a, f32 b, f32 c, f32 d)
	{
		f32 result = a;
		if (result<b)result = b;
		if (result<c)result = c;
		if (result<d)result = d;
		return result;
	}


	// @brief 配列の中から一番大きい値を返す
	inline f32 Mathf::Max(const f32* values, s32 size)
	{
		OB_REQUIRE(0 < size);
		OB_REQUIRE(values != nullptr);
		f32 ans = values[0];
		for (s32 i = 0; i < size; i++)ans = Mathf::Max(ans, values[i]);
		return ans;
	}


	// @brief 3つの値から一番小さい値を返す
	inline f32 Mathf::Min(f32 a, f32 b, f32 c)
	{
		f32 result = a;
		if (b < result)result = b;
		if (c < result)result = c;
		return result;
	}


	// @brief 4つの値から一番小さい値を返す
	inline f32 Mathf::Min(f32 a, f32 b, f32 c, f32 d)
	{
		f32 result = a;
		if (b < result)result = b;
		if (c < result)result = c;
		if (d < result)result = d;
		return result;
	}


	// @brief 配列の中から一番小さい値を返す
	inline f32 Mathf::Min(const f32* values, s32 size)
	{
		OB_REQUIRE(0 < size);
		OB_REQUIRE(values != nullptr);
		f32 ans = values[0];
		for (s32 i = 0; i < size; i++)ans = Mathf::Min(ans, values[i]);
		return ans;
	}


}// namespace ob