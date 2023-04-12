//***********************************************************
//! @file
//! @brief		範囲
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Math.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief      範囲
	//@―---------------------------------------------------------------------------
	struct OB_API Range {
	public:

		//===============================================================
		//  コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief デフォルトコンストラクタ
		//@―---------------------------------------------------------------------------
		Range()noexcept
			:Range(std::numeric_limits<f32>::lowest(), std::numeric_limits<f32>::max())
		{
		}


		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ(範囲指定)
		//! 
		//! @param min  最小値
		//! @param max  最大値
		//@―---------------------------------------------------------------------------
		constexpr Range(f32 min, f32 max)noexcept
			:min(min), max(max)
		{}


		//@―---------------------------------------------------------------------------
		//! @brief 等価演算子
		//@―---------------------------------------------------------------------------
		constexpr bool operator == (const Range & rhs)const noexcept {
			return Math::IsNearEquals(min, rhs.min) && Math::IsNearEquals(max, rhs.max);
		}


		//@―---------------------------------------------------------------------------
		//! @brief 否等価演算子
		//@―---------------------------------------------------------------------------
		constexpr bool operator != (const Range& rhs)const noexcept {
			return !(*this==rhs);
		}


		//@―---------------------------------------------------------------------------
		//! @brief 論理積
		//@―---------------------------------------------------------------------------
		constexpr Range operator & (const Range & rhs)noexcept {
			return Range(*this)&=rhs;
		}


		//@―---------------------------------------------------------------------------
		//! @brief 論理積
		//@―---------------------------------------------------------------------------
		constexpr Range& operator &= (const Range& rhs)noexcept {
			min = Math::Max(min, rhs.min);
			max = Math::Min(max, rhs.max);
			return *this;
		}


		//@―---------------------------------------------------------------------------
		//! @brief		[min,max]がvalueを含むか
		//@―---------------------------------------------------------------------------
		constexpr bool contains(f32 value)const noexcept
		{
			return min < value&& value < max;
		}


		//@―---------------------------------------------------------------------------
		//! @brief		(min,max)がvalueを含むか
		//@―---------------------------------------------------------------------------
		constexpr bool containsOO(f32 value)const noexcept
		{
			return min <= value && value <= max;
		}


		//@―---------------------------------------------------------------------------
		//! @brief		[min,max)がvalueを含むか
		//@―---------------------------------------------------------------------------
		constexpr bool containsCO(f32 value)const noexcept
		{
			return min < value&& value <= max;
		}


		//@―---------------------------------------------------------------------------
		//! @brief		(min,max]がvalueを含むか
		//@―---------------------------------------------------------------------------
		constexpr bool containsOC(f32 value)const noexcept
		{
			return min <= value&& value < max;
		}

	public:

		f32 min;	//!< 最小値
		f32 max;	//!< 最大値

	};

}


//===============================================================
// フォーマット
//===============================================================
//! @cond
template <> struct fmt::formatter<ob::core::Range, ob::core::Char> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
		return ctx.end();
	}

	template<typename FormatContext>
	auto format(ob::core::Range value, FormatContext& ctx) -> decltype(ctx.out()) {
		return format_to(ctx.out(), TC("({:.5},{:.5})"), value.min,value.max);
	}
};
//! @endcond