//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphics {

	enum class MaterialRootSignatureSlot {
		DynamicGlobal ,
		TextureGlobal,
		//SamplerGlobal,
		//BufferGlobal,
		DynamicLocal,
		TextureLocal,
		//BufferLocal,
		//SamplerLocal,
	};



	
	enum class VertexLayoutId : s32 {};

	enum class PropertyType {
		Int,
		Float,
		Color,
		Matrix,
		Texture,
		Buffer
	};

	struct ValuePropertyDesc {
		PropertyType type;
		s32 offset = -1;
	};

	using PropertyMap = Map<String, ValuePropertyDesc, std::less<>>;

}