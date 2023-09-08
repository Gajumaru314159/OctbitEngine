//***********************************************************
//! @file
//! @brief		ボックス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Geometry/Box.h>
#include <Framework/Core/Geometry/Sphere.h>

namespace ob::core {

	struct AABB {
		Vec3 min;
		Vec3 max;
	};

	struct OBB {
		Vec3 pos;
		Vec3 size;
		Quat rotation;
	};

}