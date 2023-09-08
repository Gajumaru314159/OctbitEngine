//***********************************************************
//! @file
//! @brief		描画パイプライン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>

namespace ob::graphics {

	struct RenderPipelineDesc {
		String						name;
		Array<UPtr<RenderFeature>>	features;
	};

}