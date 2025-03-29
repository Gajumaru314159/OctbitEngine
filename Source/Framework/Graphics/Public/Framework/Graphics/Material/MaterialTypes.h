//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Types/SubPass.h>
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/Material/MaterialDesc.h>

namespace ob::graphics {   

	enum class MaterialPropertyType {
		Int,
		Float,
		Color,
		Matrix,
		Texture,
		Buffer
	};

	struct MaterialValuePropertyDesc {
		MaterialPropertyType type;
		s32 offset = -1;
	};

	using MaterialPropertyMap = Map<String, MaterialValuePropertyDesc, std::less<>>;

}