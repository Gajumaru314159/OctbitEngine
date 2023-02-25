﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {
	
	enum class VertexLayoutId : s32 {};

	enum class PropertyType {
		Int,
		Float,
		Color,
		Matrix,
		Texture
	};

	struct ValuePropertyDesc {
		PropertyType type;
		s32 offset = -1;
	};

	using PropertyMap = Map<String, ValuePropertyDesc, std::less<>>;

}// namespcae ob