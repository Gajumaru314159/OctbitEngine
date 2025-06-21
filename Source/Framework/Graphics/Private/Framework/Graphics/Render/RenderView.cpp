//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Render/RenderView.h>

namespace ob::graphics {

	//! @brief      コンストラクタ
	RenderView::RenderView(RenderScene& scene, const RenderViewData& data)
		: m_scene(scene)
	{
		get<RenderViewData>() = data;
		scene.addView(this);
	}

	//! @brief      コンストラクタ
	RenderView::RenderView(RenderScene& scene, StringView name, s32 pipeline)
		: RenderView(scene, 
			[&] {
				RenderViewData data;
				data.name = name;
				data.pipeline = pipeline;
				return data;
			}()
		)
	{
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