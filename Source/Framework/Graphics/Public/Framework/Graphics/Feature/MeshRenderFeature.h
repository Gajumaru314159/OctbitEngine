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

	private:


	};

}