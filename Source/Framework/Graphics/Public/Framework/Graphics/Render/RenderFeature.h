//***********************************************************
//! @file
//! @brief		描画パイプライン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Utility/Ref.h>
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/Camera.h>
#include <Framework/Graphics/Render/RenderStep.h>
#include <Framework/Graphics/Render/RenderStepList.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      描画機能
	//! @details    O3DEでいうところのFeatureProcessor。
	//!				初期状態は非アクティブです。
	//@―---------------------------------------------------------------------------
	class RenderFeature {
	public:

		RenderFeature();
		virtual ~RenderFeature();

		//@―---------------------------------------------------------------------------
		//! @brief      必要なRenderStepをRenderStepListに追加する
		//@―---------------------------------------------------------------------------
		virtual void setupView(RenderView&) {}

		virtual void activate() {}
		virtual void deactivate() {}

		virtual void simulate() {}
		virtual void render() {}

	};

}