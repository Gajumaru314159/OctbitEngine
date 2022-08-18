//***********************************************************
//! @file
//! @brief		カラー
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Graphic/Color.h>
#include <Framework/Core/Graphic/HSV.h>
#include <Framework/Core/Graphic/IntColor.h>

#include <Framework/Core/Math/Math.h>
#include <Framework/Core/Math/Vectors.h>

namespace ob::core {

	const HSV HSV::White = { 0,0,1,1 };
	const HSV HSV::Gray = { 0,0,0.5,1 };
	const HSV HSV::Black = { 0,0,0,1 };
	const HSV HSV::Clear = { 0,0,0,0 };
	const HSV HSV::Red = { 0,1,1,1 };
	const HSV HSV::Yellow = { 0.25,1,1,1 };
	const HSV HSV::Green = { 0.5,1,1,1 };
	const HSV HSV::Cyan = { 0.5,1,1,1 };
	const HSV HSV::Blue = { 0.75,1,1,1 };
	const HSV HSV::Magenta = { 1,1,1,1 };// TODO


	//@―---------------------------------------------------------------------------
	//! @brief Vec3 を変換して初期化(アルファは1.0)
	//@―---------------------------------------------------------------------------
	HSV::HSV(const Vec3& another)noexcept {
		h = another.x;
		s = another.y;
		v = another.z;
		a = 1.0f;
	}


	//@―---------------------------------------------------------------------------
	//! @brief Vec4 を変換して初期化
	//@―---------------------------------------------------------------------------
	HSV::HSV(const Vec4& another)noexcept {
		h = another.x;
		s = another.y;
		v = another.z;
		a = another.w;
	}


	//@―---------------------------------------------------------------------------
	//! @brief コンストラクタ(Colorで初期化)
	//! 
	//! @param color    RGBA色
	//@―---------------------------------------------------------------------------
	HSV::HSV(const Color& color) noexcept {
		OB_NOTIMPLEMENTED();
	}


	//@―---------------------------------------------------------------------------
	//! @brief      Color型に変換
	//@―---------------------------------------------------------------------------
	Color  HSV::toColor()const {
		OB_NOTIMPLEMENTED();
		// TODO HSV::toColor
		return Color::White;
	}
	
}// namespace ob