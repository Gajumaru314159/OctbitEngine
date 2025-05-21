//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Render/RenderView.h>

namespace ob::graphics {

	//! @brief      コンストラクタ
	RenderView::RenderView(RenderScene& scene, StringView name, StringView pipeline)
		: m_scene(scene)
		, m_name(name)
	{
		scene.addView(this,pipeline);
	}

	//! @brief      デストラクタ
	RenderView::~RenderView() {
		m_scene.removeView(this);
	}


	//! @brief      RenderFeatureを見つける
	RenderFeature* RenderView::findFeature(Type type)const {
		return m_scene.findFeature(type);
	}

}