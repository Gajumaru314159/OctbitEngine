//***********************************************************
//! @file
//! @brief		RHI前方宣言
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Utility/Ref.h>

namespace ob::rhi {

	// 列挙型
	enum class PresetTexture;

	// 構造体
	struct SetDescriptorTableParam;
	struct DrawParam;
	struct DrawIndexedParam;
	struct GraphicFileQueueDesc;

	// クラス
	class RHI;
	class Buffer;
	class CommandList;
	class DescriptorTable;
	class Display;
	class PipelineState;
	class RenderTexture;
	class ResourceBarrier;
	class RootSignature;
	class Shader;
	class Texture;
	class Sampler;

	class GraphicFileHandle;
	class GraphicFileEvent;
	class GraphicFileQueue;

	// 内部クラス
	class GraphicObjectManager;

}