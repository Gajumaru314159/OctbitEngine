//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderFeature.h>

#include <Framework/Graphics/FrameGraph/FG.h>

#include <Framework/RHI/CommandList.h>

#include <Framework/Graphics/Render/RenderPipeline.h>
#include <Framework/Graphics/Render/RenderScene.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      描画機能
	//! @details    O3DEでいうところのFeatureProcessor。
	//!				初期状態は非アクティブです。
	//@―---------------------------------------------------------------------------
	class ModelRenderFeature : public RenderFeature {
	public:

		OB_RTTI();

		//@―---------------------------------------------------------------------------
		//! @brief      描画
		//@―---------------------------------------------------------------------------
		void addModel() {
			// Mesh/Material単位でインスタンシング化
			// 
		
		}
		//@―---------------------------------------------------------------------------
		//! @brief      描画
		//@―---------------------------------------------------------------------------
		void removeModel() {}



		//@―---------------------------------------------------------------------------
		//! @brief      描画
		//@―---------------------------------------------------------------------------
		void render(FG& fg, FrameGraphResource target) {

		}


	protected:
		ModelRenderFeature(RenderScene& scene) :RenderFeature(scene) {}
	private:

	};

}