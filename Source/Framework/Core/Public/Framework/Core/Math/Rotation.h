﻿//***********************************************************
//! @file
//! @brief		オイラー角回転構造体
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Math.h>

namespace ob::core {

	struct Vec3;
	struct Quat;

	//@―---------------------------------------------------------------------------
	//! @brief      オイラー回転を表す構造体
	//! 
	//! @details    回転はZXYの順に行われる。内部の値は度数法で表される。
	//@―---------------------------------------------------------------------------
	struct OB_API Rot {
	public:

		//===============================================================
		//  コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  デフォルトコンストラクタ(初期化なし)
		//@―---------------------------------------------------------------------------
		Rot()noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ(x/y/z指定)
		//@―---------------------------------------------------------------------------
		constexpr Rot(f32 x, f32 y, f32 z)noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ(Vec3指定)
		//@―---------------------------------------------------------------------------
		explicit Rot(const Vec3& vec)noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ(Quat指定)
		//@―---------------------------------------------------------------------------
		explicit Rot(const Quat& quat);


		//===============================================================
		//  オペレータ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief 等価演算子
		//@―---------------------------------------------------------------------------
		constexpr bool operator == (const Rot& rhs)const noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief 等価演算子
		//@―---------------------------------------------------------------------------
		constexpr bool operator != (const Rot& rhs)const noexcept;


		//===============================================================
		//  変換
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  Quatに変換
		//@―---------------------------------------------------------------------------
		Quat toQuat()const;

		//@―---------------------------------------------------------------------------
		//! @brief  Quatに変換
		//@―---------------------------------------------------------------------------
		Vec3 toVec3()const;


		//===============================================================
		//  ゲッター
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  正面ベクトルを取得
		//@―---------------------------------------------------------------------------
		Vec3 front()const;


		//@―---------------------------------------------------------------------------
		//! @brief  背面ベクトルを取得
		//@―---------------------------------------------------------------------------
		Vec3 back()const;


		//@―---------------------------------------------------------------------------
		//! @brief  上ベクトルを取得
		//@―---------------------------------------------------------------------------
		Vec3 up()const;


		//@―---------------------------------------------------------------------------
		//! @brief  下ベクトルを取得
		//@―---------------------------------------------------------------------------
		Vec3 down()const;


		//@―---------------------------------------------------------------------------
		//! @brief  左ベクトルを取得
		//@―---------------------------------------------------------------------------
		Vec3 left()const;


		//@―---------------------------------------------------------------------------
		//! @brief  右ベクトルを取得
		//@―---------------------------------------------------------------------------
		Vec3 right()const;


		//===============================================================
		//  変換
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief      正規化
		//! 
		//! @details    角回転量を[-180,180]に収める
		//@―---------------------------------------------------------------------------
		Rot& normalize();


		//===============================================================
		//  判定
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  NaNを含むか
		//@―---------------------------------------------------------------------------
		bool isNaN();

	public:

		static const Rot Identity;  //!< 無回転

	public:

		f32 x;      //!< X軸回転量(Deg)
		f32 y;      //!< Y軸回転量(Deg)
		f32 z;      //!< Z軸回転量(Deg)

	};






	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ(x/y/z指定)
	//@―---------------------------------------------------------------------------
	constexpr Rot::Rot(f32 _x, f32 _y, f32 _z)noexcept
		: x(_x), y(_y), z(_z)
	{
	}


	//===============================================================
	//  オペレータ
	//===============================================================

	//@―---------------------------------------------------------------------------
	//! @brief 等価演算子
	//@―---------------------------------------------------------------------------
	constexpr bool Rot::operator == (const Rot& rhs)const noexcept {
		return
			Math::IsNearEquals(x, rhs.x) &&
			Math::IsNearEquals(y, rhs.y) &&
			Math::IsNearEquals(z, rhs.z);
	}


	//@―---------------------------------------------------------------------------
	//! @brief 等価演算子
	//@―---------------------------------------------------------------------------
	constexpr bool Rot::operator != (const Rot& rhs)const noexcept {
		return !(*this == rhs);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  正規化
	//@―---------------------------------------------------------------------------
	inline Rot& Rot::normalize() {
		x = Math::Wrap(x, -180, 180);
		y = Math::Wrap(y, -180, 180);
		z = Math::Wrap(z, -180, 180);
		return *this;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  NaNを含むか
	//@―---------------------------------------------------------------------------
	inline bool Rot::isNaN() {
		return
			Math::IsNaN(x) ||
			Math::IsNaN(y) ||
			Math::IsNaN(z);
	}

	//! @endcond
}


//===============================================================
// フォーマット
//===============================================================
//! @cond
template <> struct fmt::formatter<ob::core::Rot, ob::core::Char> : fmt::formatter<ob::core::f32, ob::core::Char> {
	using base = fmt::formatter<ob::core::f32, ob::core::Char>;
	bool isRad = false;
	template<typename ParseContext>
	constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {		
		auto itr = ctx.begin();
		if (itr == ctx.end())return ctx.end();
		auto mode = *ctx.begin();
		if (mode == TC('r') || mode == TC('R')) {
			isRad = true;
			itr++;
		}else if (mode == TC('d') || mode == TC('D')) {
			isRad = false;
			itr++;
		}
		ctx.advance_to(itr);
		return base::parse(ctx);
	}
	template<typename FormatContext>
	auto format(const ob::core::Rot& value, FormatContext& ctx) -> decltype(ctx.out()) {
		auto x = value.x;
		auto y = value.y;
		auto z = value.z;
		if (isRad) {
			x = ob::core::Math::Degrees(x);
			y = ob::core::Math::Degrees(y);
			z = ob::core::Math::Degrees(z);
		}
		ctx.advance_to(format_to(ctx.out(), TC("(")));
		ctx.advance_to(base::format(x, ctx));
		ctx.advance_to(format_to(ctx.out(), TC(",")));
		ctx.advance_to(base::format(y, ctx));
		ctx.advance_to(format_to(ctx.out(), TC(",")));
		ctx.advance_to(base::format(z, ctx));
		ctx.advance_to(format_to(ctx.out(), TC(")")));
		return ctx.out();
	}
};

//! @endcond