//***********************************************************
//! @file
//! @brief		ボックス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Quaternion.h>
#include <Framework/Core/Geometry/Box.h>
#include <Framework/Core/Geometry/Sphere.h>

namespace ob::core {

	//! @brief		Axis Aligned Bounding Box
	struct AABB {
		Vec3 min;
		Vec3 max;
	};

	//! @brief		Oriented Bounding Box
	struct OBB {
		Vec3 pos;
		Vec3 size;
		Quat rotation;
	};
	
	//! @brief		Box と Sphere
	struct Bounds {
		Box		box;
		Sphere	sphere;
	};

}