﻿//***********************************************************
//! @file
//! @brief		ビューポート
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Vector2.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief      ビューポート
	//! 
	//! @details    全ての値は0～1に正規化さえれた値です。
	//@―---------------------------------------------------------------------------
	struct Viewport {
	public:

		//===============================================================
		//  コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief      デフォルトコンストラクタ( 初期化なし )
		//@―---------------------------------------------------------------------------
		Viewport() noexcept = default;


		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ(要素指定)
		//@―---------------------------------------------------------------------------
		constexpr Viewport(f32 left, f32 top, f32 right, f32 bottom, f32 nearZ = 0.0f, f32 farZ = 1.0f) noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ(サイズ/中心指定)
		//@―---------------------------------------------------------------------------
		constexpr Viewport(const Vec2& center, const Vec2& size = Vec2::Zero) noexcept;


		//===============================================================
		//  オペレータ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief      等価演算子
		//@―---------------------------------------------------------------------------
		constexpr bool operator==(const Viewport& other)const noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief      否等価演算子
		//@―---------------------------------------------------------------------------
		constexpr bool operator!=(const Viewport& other)const noexcept;


		//===============================================================
		//  ゲッター
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief      幅
		//@―---------------------------------------------------------------------------
		constexpr f32 width()const noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief      高さ
		//@―---------------------------------------------------------------------------
		constexpr f32 height()const noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief      深さ
		//@―---------------------------------------------------------------------------
		constexpr f32 depth()const noexcept;


	public:
		f32 left;   //!< 左
		f32 top;    //!< 上
		f32 right;  //!< 右
		f32 bottom; //!< 下
		f32 nearZ;  //!< 近
		f32 farZ;   //!< 遠
	};






	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond

	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ(要素指定)
	//@―---------------------------------------------------------------------------
	constexpr Viewport::Viewport(f32 left, f32 top, f32 right, f32 bottom, f32 nearZ, f32 farZ) noexcept
		: left(left), top(top), right(right), bottom(bottom), nearZ(nearZ), farZ(farZ)
	{
	}


	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ(サイズ/中心指定)
	//@―---------------------------------------------------------------------------
	constexpr Viewport::Viewport(const Vec2& center, const Vec2& size) noexcept
		: left(center.x - size.x * 0.5)
		, top(center.y - size.y * 0.5)
		, right(center.x + size.x * 0.5)
		, bottom(center.y + size.y * 0.5)
		, nearZ(0), farZ(1)
	{

	}

	//@―---------------------------------------------------------------------------
	//! @brief      等価演算子
	//@―---------------------------------------------------------------------------
	constexpr bool Viewport::operator==(const Viewport& other)const noexcept {
		return
			left == other.left &&
			top == other.top &&
			right == other.right &&
			bottom == other.bottom &&
			nearZ == other.nearZ &&
			farZ == other.farZ;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      否等価演算子
	//@―---------------------------------------------------------------------------
	constexpr bool Viewport::operator!=(const Viewport& other)const noexcept {
		return !(*this == other);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      幅
	//@―---------------------------------------------------------------------------
	constexpr f32 Viewport::width()const noexcept {
		return right - left;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      高さ
	//@―---------------------------------------------------------------------------
	constexpr f32 Viewport::height()const noexcept {
		return bottom - top;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      高さ
	//@―---------------------------------------------------------------------------
	constexpr f32 Viewport::depth()const noexcept {
		return farZ - nearZ;
	}

	//! @endcond
}