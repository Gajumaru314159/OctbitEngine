//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      描画ステップ
	//! @detailas   カメラごとに持ちます。
	//@―---------------------------------------------------------------------------
	class RenderStep {
	public:

		RenderStep();
		virtual ~RenderStep();

		virtual void setup(TextureManager&) {}
		virtual void render(RenderView&) {};

	};

}