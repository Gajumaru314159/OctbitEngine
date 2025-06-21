//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderFeature.h>

namespace ob::graphics {

	//! @brief      ImGui描画機能
	class ImGuiRenderFeature : public RenderFeature {
	public:

		OB_RTTI();

		ImGuiRenderFeature(RenderScene& scene);
		virtual ~ImGuiRenderFeature();

	};

}