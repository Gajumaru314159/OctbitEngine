//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

#include <Framework/Graphics/Mesh.h>
#include <Framework/Graphics/Render/RenderScene.h>

#include <Framework/Graphics/Render/RenderFeature.h>

namespace ob::model2 {

	struct Texture {

	};


	struct Material {
		HashMap<String, f32>		scalars;
		HashMap<String, Vec4>		vectors;
		HashMap<String, Matrix>		matrices;
		HashMap<String, Color>		colors;
		HashMap<String, Texture>	textures;
	};



	struct SubMesh {
		s64 start;
		s64 count;
	};

	struct Mesh {
		Layout			layout;
		Blob			buffer;
		Array<Submesh>	submeshes;

		Vec3& position(s64 index);
		Vec3& position(s64 index);
		Vec3& position(s64 index);
	};


	struct Model {
		Model(Path);

		Mesh mesh;
		Array<Material> materials;
	};

	
}