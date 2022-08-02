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

	//@―---------------------------------------------------------------------------
	//! @brief			RGBAカラーから設定
	//! 
	//! @param color    色
	//@―---------------------------------------------------------------------------
	void HSV::set(const Color& color) noexcept {
		// TODO HSV::set
	}


	//@―---------------------------------------------------------------------------
	//! @brief      Color型に変換
	//@―---------------------------------------------------------------------------
	Color  HSV::toColor()const {
		// TODO HSV::toColor
		return Color::White;
	}
	
}// namespace ob