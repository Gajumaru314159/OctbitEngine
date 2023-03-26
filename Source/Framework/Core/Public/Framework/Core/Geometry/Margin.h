﻿//***********************************************************
//! @file
//! @brief		マージン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Vector2.h>
#include <Framework/Core/Geometry/Rect.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  マージン
	//@―---------------------------------------------------------------------------
	struct OB_API Margin {
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  デフォルトコンストラクタ(初期化なし)
		//@―---------------------------------------------------------------------------
		inline Margin()noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ(要素指定)
		//@―---------------------------------------------------------------------------
		constexpr Margin(f32 left, f32 top, f32 right, f32 bottom)noexcept;


		//===============================================================
		//  オペレータ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief          等価演算子
		//@―---------------------------------------------------------------------------
		constexpr bool operator==(const Margin& another)const noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief          否等価演算子
		//@―---------------------------------------------------------------------------
		constexpr bool operator!=(const Margin& another)const noexcept;


		//===============================================================
		// ゲッター
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief      左上
		//@―---------------------------------------------------------------------------
		constexpr Vec2 tl()const noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief      右上
		//@―---------------------------------------------------------------------------
		constexpr Vec2 tr()const noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief      左下
		//@―---------------------------------------------------------------------------
		constexpr Vec2 bl()const noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief      右下
		//@―---------------------------------------------------------------------------
		constexpr Vec2 br()const noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief      左右の余白の合計
		//@―---------------------------------------------------------------------------
		constexpr f32 horizontal()const noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief      上下の余白の合計
		//@―---------------------------------------------------------------------------
		constexpr f32 vertical()const noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief  親サイズからサイズを計算
		//@―---------------------------------------------------------------------------
		constexpr Vec2 size(const Vec2& parent)const noexcept;


		//@―---------------------------------------------------------------------------
		//! @brief  親矩形から Rect を計算
		//@―---------------------------------------------------------------------------
		constexpr Rect rect(const Rect& parent)const noexcept;


	public:

		f32 left;   //!< 左座標
		f32 top;    //!< 上座標
		f32 right;  //!< 右座標
		f32 bottom; //!< 上座標

	};






	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond

	//@―---------------------------------------------------------------------------
	//! @brief  デフォルトコンストラクタ(初期化なし)
	//@―---------------------------------------------------------------------------
	inline Margin::Margin() noexcept
	{
	}


	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ(要素指定)
	//@―---------------------------------------------------------------------------
	constexpr Margin::Margin(f32 left, f32 top, f32 right, f32 bottom)noexcept
		: left(left)
		, top(top)
		, right(right)
		, bottom(bottom)
	{
	}


	//@―---------------------------------------------------------------------------
	//! @brief          等価演算子
	//@―---------------------------------------------------------------------------
	constexpr bool Margin::operator==(const Margin& another)const noexcept {
		return
			Math::IsNearEquals(left, another.left) &&
			Math::IsNearEquals(top, another.top) &&
			Math::IsNearEquals(right, another.right) &&
			Math::IsNearEquals(bottom, another.bottom);
	}


	//@―---------------------------------------------------------------------------
	//! @brief          否等価演算子
	//@―---------------------------------------------------------------------------
	constexpr bool Margin::operator!=(const Margin& another)const noexcept {
		return !(*this == another);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      左上
	//@―---------------------------------------------------------------------------
	constexpr Vec2 Margin::tl()const noexcept {
		return Vec2(left, top);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      右上
	//@―---------------------------------------------------------------------------
	constexpr Vec2 Margin::tr()const noexcept {
		return Vec2(right, top);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      左下
	//@―---------------------------------------------------------------------------
	constexpr Vec2 Margin::bl()const noexcept {
		return Vec2(left, bottom);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      右下
	//@―---------------------------------------------------------------------------
	constexpr Vec2 Margin::br()const noexcept {
		return Vec2(right, bottom);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      左右の余白の合計
	//@―---------------------------------------------------------------------------
	constexpr f32 Margin::horizontal()const noexcept {
		return left + right;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      上下の余白の合計
	//@―---------------------------------------------------------------------------
	constexpr f32 Margin::vertical()const noexcept {
		return top + bottom;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  親サイズからサイズを計算
	//@―---------------------------------------------------------------------------
	constexpr Vec2 Margin::size(const Vec2& parent)const noexcept {
		return Vec2(parent.x - left - right, parent.y - top - bottom);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  親矩形から Rect を計算
	//@―---------------------------------------------------------------------------
	constexpr Rect Margin::rect(const Rect& parent)const noexcept {
		return Rect(
			size(parent.size()),
			parent.center() + Vec2(left + right, top + bottom) * 0.5f
		);
	}


	//! @endcond
}// namespcae ob