//***********************************************************
//! @file
//! @brief		Graphic前方宣言
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphics {

	struct RenderPipelineDesc;
	struct RenderSceneDesc;

	class Material;
	class Mesh;

	class Camera;

	class IRenderer;


	class RenderView;
	class RenderScene;
	class RenderPipeline;
	class RenderFeature;

	using RenderScenePtr = SPtr<RenderScene>;

	class RenderFeatureInjector;

	class FG;

}