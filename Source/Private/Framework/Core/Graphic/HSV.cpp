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
	//! @brief Vec3 を変換して初期化(アルファは1.0)
	//@―---------------------------------------------------------------------------
	HSV::HSV(const Vec3& another)noexcept {
		OB_NOTIMPLEMENTED();
	}


	//@―---------------------------------------------------------------------------
	//! @brief Vec4 を変換して初期化
	//@―---------------------------------------------------------------------------
	HSV::HSV(const Vec4& another)noexcept {
		OB_NOTIMPLEMENTED();
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