//***********************************************************
//! @file
//! @brief		Graphic前方宣言
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphics {

	struct RenderPipelineDesc;

	class Material;
	class Mesh;

	class Camera;
	class CommandBuffer;
	class RenderContext;

	class IRenderer;


	class RenderScene;
	class RenderPipeline;
	class RenderFeature;
	class RenderView;
	class RenderStepSet;
	class RenderStep;
	class RenderExecutor;
	class TextureManager;

	struct RenderArgs;
	class CommandRecorder;
	class CommandStorage;

	class RenderFeatureInjector;

	class RenderFrameData;

}