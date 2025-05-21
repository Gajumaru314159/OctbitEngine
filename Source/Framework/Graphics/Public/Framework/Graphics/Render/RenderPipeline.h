//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/Render/RenderPassBuilder.h>

namespace ob::graphics {

	struct RenderPipeline {
		String						name;
		RenderPassBuilder			builer;
		Vector<RenderView*>			views;
	};

}