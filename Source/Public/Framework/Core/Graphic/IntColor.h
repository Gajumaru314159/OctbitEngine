//***********************************************************
//! @file
//! @brief		整数表現カラークラス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Graphic/ColorTypes.h>
#include <Framework/Core/Template/include.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief		整数表現カラークラス
	//! 
	//! @details	各色要素は、0〜255の範囲の u8 型で表現されます。アルファ(a)は透過度を表し、0で完全な透明、255で完全な不透明を表します。
	//@―---------------------------------------------------------------------------
	struct OB_API IntColor {
	public:

		//===============================================================
		//  コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief			デフォルトコンストラクタ(初期化なし)
		//@―---------------------------------------------------------------------------
		IntColor() noexcept = default;


		//@―---------------------------------------------------------------------------
		//! @brief			コンストラクタ(各要素を指定して初期化)
		//! 
		//! @details		各色成分を指定して初期化します。
		//! @param r		赤成分
		//! @param g		緑成分
		//! @param b		青成分
		//! @param a		アルファ
		//@―---------------------------------------------------------------------------
		constexpr IntColor(s32 r, s32 g, s32 b, s32 a = 255) noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief          コンストラクタ(輝度とアルファ値を指定して初期化)
		//! 
		//! @details		色成分を輝度で指定します。
		//! @param grey	    輝度
		//! @param a		アルファ
		//@―---------------------------------------------------------------------------
		constexpr IntColor(s32 grey, s32 a = 255) noexcept;


		//===============================================================
		//  オペレータ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief			加算演算子
		//@―---------------------------------------------------------------------------
		constexpr IntColor operator + (const IntColor& another) const noexcept;

		//@―---------------------------------------------------------------------------
		//! @brief			減算演算子
		//@―---------------------------------------------------------------------------
		constexpr IntColor operator - (const IntColor& another) const noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief			加算代入演算子
		//@―---------------------------------------------------------------------------
		constexpr IntColor& operator += (const IntColor& another) noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief			減算代入演算子
		//@―---------------------------------------------------------------------------
		constexpr IntColor& operator -= (const IntColor& another) noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief 乗算演算子
		//@―---------------------------------------------------------------------------
		constexpr IntColor operator * (const IntColor& another) const noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief          乗算代入演算子
		//@―---------------------------------------------------------------------------
		constexpr IntColor& operator *= (const IntColor& another) noexcept;


		//===============================================================
		//  ゲッター
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief          RGBのうち最小の値を取得
		//@―---------------------------------------------------------------------------
		constexpr u8 minComponent()const noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief          RGBのうち最大の値を取得
		//@―---------------------------------------------------------------------------
		constexpr u8 maxComponent()const noexcept;


		//===============================================================
		//  変換
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief          色をカラーコードに変換
		//! 
		//! @param          format  カラーフォーマット(色の並び)
		//@―---------------------------------------------------------------------------
		constexpr u32 toCode(ColorCodeFormat format = ColorCodeFormat::Default)const noexcept;

	public:

		static const IntColor White;		//!< Color(255,255,255,255)
		static const IntColor Gray;		    //!< Color(128.128.128,255)
		static const IntColor Black;		//!< Color(0  ,0  ,0  ,255)
		static const IntColor Clear;		//!< Color(0  ,0  ,0  ,0  )
		static const IntColor Red;		    //!< Color(255,0  ,0  ,255)
		static const IntColor Yellow;	    //!< Color(255,255,0  ,255)
		static const IntColor Green;		//!< Color(0  ,255,0  ,255)
		static const IntColor Cyan;		    //!< Color(0  ,255,255,255)
		static const IntColor Blue;		    //!< Color(0  ,0  ,255,255)
		static const IntColor Magenta;	    //!< Color(255,0  ,255,255)
		static const IntColor Normal;	    //!< Color(128,255,128,255)

	public:

		u8 r;       //!< 赤成分
		u8 g;       //!< 緑成分
		u8 b;       //!< 青成分
		u8 a;       //!< アルファ成分

	};






	//=======================================================================
	// インライン
	//=======================================================================
	//! @cond

	//@―---------------------------------------------------------------------------
	//! @brief			コンストラクタ
	//! 
	//! @details		各色成分を指定して初期化します。
	//! @param r		赤成分
	//! @param g		緑成分
	//! @param b		青成分
	//! @param a		アルファ
	//@―---------------------------------------------------------------------------
	constexpr IntColor::IntColor(s32 r, s32 g, s32 b, s32 a) noexcept
		: r((u8)std::clamp(r, 0, 255))
		, g((u8)std::clamp(g, 0, 255))
		, b((u8)std::clamp(b, 0, 255))
		, a((u8)std::clamp(a, 0, 255))
	{
	}


	//@―---------------------------------------------------------------------------
	//! @brief			コンストラクタ
	//! 
	//! @details		色成分を輝度で指定します。
	//! @param grey	輝度
	//! @param a		アルファ
	//@―---------------------------------------------------------------------------
	constexpr IntColor::IntColor(s32 gray, s32 a) noexcept
		: IntColor(gray, gray, gray, a)
	{
	}


	//@―---------------------------------------------------------------------------
	//! @brief			加算演算子
	//@―---------------------------------------------------------------------------
	constexpr IntColor IntColor::operator + (const IntColor& another) const noexcept {
		return IntColor(*this) += another;
	}


	//@―---------------------------------------------------------------------------
	//! @brief			減算演算子
	//@―---------------------------------------------------------------------------
	constexpr IntColor IntColor::operator - (const IntColor& another) const noexcept {
		return IntColor(*this) -= another;
	}


	//@―---------------------------------------------------------------------------
	//! @brief			乗算演算子
	//@―---------------------------------------------------------------------------
	constexpr IntColor IntColor::operator * (const IntColor& another) const noexcept {
		return IntColor(*this) *= another;
	}


	//@―---------------------------------------------------------------------------
	//! @brief			加算代入演算子
	//@―---------------------------------------------------------------------------
	constexpr IntColor& IntColor::operator += (const IntColor& another) noexcept {
		return *this = IntColor(r + another.r, g + another.g, b + another.b, a);
	}


	//@―---------------------------------------------------------------------------
	//! @brief			減算代入演算子
	//@―---------------------------------------------------------------------------
	constexpr IntColor& IntColor::operator -= (const IntColor& another) noexcept {
		return *this = IntColor(r - another.r, g - another.g, b - another.b, a);
	}


	//@―---------------------------------------------------------------------------
	//! @brief			乗算代入演算子
	//@―---------------------------------------------------------------------------
	constexpr IntColor& IntColor::operator *= (const IntColor& another) noexcept {
		return *this = IntColor(
			(s32)r * another.r / 255,
			(s32)g * another.g / 255,
			(s32)b * another.b / 255,
			(s32)a * another.a / 255);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      RGBのうち最小の値を取得
	//@―---------------------------------------------------------------------------
	constexpr u8 IntColor::minComponent()const noexcept {
		return std::min({ r, g, b });
	}


	//@―---------------------------------------------------------------------------
	//! @brief      RGBのうち最大の値を取得
	//@―---------------------------------------------------------------------------
	constexpr u8 IntColor::maxComponent()const noexcept {
		return std::min({ r, g, b });
	}


	//@―---------------------------------------------------------------------------
	//! @brief          色をカラーコードに変換
	//! 
	//! @param format   カラーフォーマット(色の並び)
	//@―---------------------------------------------------------------------------
	constexpr u32 IntColor::toCode(ColorCodeFormat format)const noexcept {
		switch (format) {
		case ColorCodeFormat::ARGB:
			return (a << 24) | (r << 16) | (g << 8) | (b);
		case ColorCodeFormat::ABGR:
			return (a << 24) | (b << 16) | (g << 8) | (r);
		case ColorCodeFormat::RGBA:
			return (r << 24) | (g << 16) | (b << 8) | (a);
		case ColorCodeFormat::BGRA:
			return (b << 24) | (g << 16) | (r << 8) | (a);
		default:break;
		}
		return 0xFFFFFFFF;
	}

	//! @endcond
}// namespcae ob


//===============================================================
// フォーマット
//===============================================================
//! @cond
template <> struct fmt::formatter<ob::core::IntColor, ob::core::Char> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
		return ctx.end();
	}

	template<typename FormatContext>
	auto format(ob::core::IntColor value, FormatContext& ctx) -> decltype(ctx.out()) {
		return format_to(ctx.out(), TC("({},{},{},{})"), value.r, value.g, value.b, value.a);
	}
};
//! @endcond