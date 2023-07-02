﻿//***********************************************************
//! @file
//! @brief		カメラ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/RenderPipeline.h>
#include <Framework/Core/Utility/Ref.h>

namespace ob::graphics {

	class RenderPipelineAsset {
	public:

		Ref<RenderPipeline> create() {
			return nullptr;
		}
	};


	class RenderPipelineStepAsset {

	};

}