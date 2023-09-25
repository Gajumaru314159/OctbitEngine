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

		MeshHandle acquireMesh();
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

	};

}