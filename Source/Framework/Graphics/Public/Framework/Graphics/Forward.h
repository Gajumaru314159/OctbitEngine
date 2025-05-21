//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>

namespace ob::graphics {

	struct MaterialBlockDesc;

	class FG;

	struct RenderSceneDesc;

	class RenderFeature;
	class RenderPipeline;
	class RenderScene;
	class RenderView;
	class RenderPass;
	class RenderPassBuilder;

	class Mesh;
	class Material;

	OB_EVENT_NOTIFIER(RenderFeatureEvent, RenderFeature&);
	OB_EVENT_NOTIFIER(RenderPipelineEvent, RenderPipeline&);
	OB_EVENT_NOTIFIER(RenderSceneEvent, RenderScene&);
	OB_EVENT_NOTIFIER(RenderViewEvent, RenderView&);

}