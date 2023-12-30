//***********************************************************
//! @file
//! @brief		UniversalRenderPipeline
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderFeature.h>

namespace ob::graphics {

	using MeshHandle = u32;

	//@―---------------------------------------------------------------------------
	//! @brief      
	//@―---------------------------------------------------------------------------
	class MeshRenderFeature : public RenderFeature {
	public:

		MeshRenderFeature();

		MeshHandle createMesh();
		bool releaseMesh(MeshHandle);
		MeshHandle cloneMesh(MeshHandle);

		Ref<Model> getModel(MeshHandle);


		template<class T>
		static MeshRenderFeature* Of(T& owner) {
			return owner.findService<MeshRenderFeature>();
		}

	private:

		void simulate() override {

		}

	private:

		HashMap<MeshHandle, UPtr<ModelInstance>> m_instances;

		struct MeshSubset {
			s32 indexOffset;
			s32 indexCount;
			s32 materialIndex;
		};

		struct Data {
			Ref<Mesh> mesh;
			Array<MeshSubset> subsets;
			AABB aabb;
		};




	};

}