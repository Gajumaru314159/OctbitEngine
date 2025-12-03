//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/Material/MaterialDesc.h>

namespace ob::graphics {

	enum class VertexLayoutId : s32 {};

	enum class MaterialPropertyType {
		Texture,
		Buffer,
		Matrix,
		Vector,
		Scalar,
		Integer,
	};

	struct MaterialValuePropertyDesc {
		MaterialPropertyType type;
		s32 offset = -1; // バッファ内のバイトオフセット
		s32 index = -1; // 何番目のリソースか (Textureの何番目か/Samplerの何番目か/Bufferの何番目か)
		s32 slot = -1; // DescriptorTableの何番目にバインドされているか
	};

	using MaterialPropertyMap = Map<String, MaterialValuePropertyDesc, std::less<>>;

}