//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/RenderPipeline.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      RenderFeatureを取得
	//@―---------------------------------------------------------------------------
	RenderFeature* RenderPipeline::findFeature(TypeId typId)const { 
		return m_scene.findFeature(typId);
	}

}