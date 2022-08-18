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

namespace {

	inline constexpr int HSVConvTable[6][3] =
	{
		{ 3, 2, 0 },
		{ 1, 3, 0 },
		{ 0, 3, 2 },
		{ 0, 1, 3 },
		{ 2, 0, 3 },
		{ 3, 0, 1 },
	};

}

namespace ob::core {

	const HSV HSV::White	= {   0,0,1,1 };
	const HSV HSV::Gray		= {   0,0,0.5,1 };
	const HSV HSV::Black	= {   0,0,0,1 };
	const HSV HSV::Clear	= {   0,0,0,0 };
	const HSV HSV::Red		= {   0,1,1,1 };
	const HSV HSV::Yellow	= {  60,1,1,1 };
	const HSV HSV::Green	= { 120,1,1,1 };
	const HSV HSV::Cyan		= { 180,1,1,1 };
	const HSV HSV::Blue		= { 240,1,1,1 };
	const HSV HSV::Magenta	= { 300,1,1,1 };


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
		auto r = color.r;
		auto g = color.g;
		auto b = color.b;

		f32 k = 0.0;

		if (g < b) {
			std::swap(g, b);
			k = -360.0;
		}

		if (r < g) {
			std::swap(r, g);
			k = -720.0 / 6.0 - k;
		}

		const f32 delta = (g - b) * (360.0 / 6.0);
		const f32 chroma = r - Math::Min(g, b);
		
		h = Math::Abs(k + delta / (chroma + 1e-20));
		s = chroma / (r + 1e-20);
		v = r;
		a = color.a;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      Color型に変換
	//@―---------------------------------------------------------------------------
	Color  HSV::toColor()const {
		const f32 hue01 = Math::Fract(h / 360.0);
		const f32 hueF = (hue01 * 6.0);
		const s32 hueI = static_cast<s32>(hueF);
		const f32 fr = (hueF - hueI);
		const f32 vals[4] = {
			v * (1.0f - s),
			v * (1.0f - s * fr),
			v * (1.0f - s * (1.0f - fr)),
			v
		};

		return{ 
			vals[HSVConvTable[hueI][0]],
			vals[HSVConvTable[hueI][1]],
			vals[HSVConvTable[hueI][2]],
			a
		};
	}
	
}// namespace ob