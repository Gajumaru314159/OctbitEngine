//***********************************************************
//! @file
//! @brief		UniversalRenderPipeline
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderFeature.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      
	//@―---------------------------------------------------------------------------
	class MaterialRenderFeature : public RenderFeature {
	public:

		MaterialRenderFeature();


		void render() {
		

		
		}


	private:

		void simulate() override {

		}

	private:

		HashMap<String, Array<DrawItem>> m_map;

	};

}