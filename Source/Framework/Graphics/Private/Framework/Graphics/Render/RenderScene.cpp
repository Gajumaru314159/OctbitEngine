//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Render/RenderPipeline.h>

namespace ob::graphics {

	Ref<RenderScene> RenderScene::Create(const RenderSceneDesc& desc) {
		return new RenderScene(desc);
	}

	RenderScene::RenderScene(const RenderSceneDesc& desc)
		: m_desc(desc)
	{

	}

	void RenderScene::upadte() {
		if (!m_renderPipeline)return;

	}


}