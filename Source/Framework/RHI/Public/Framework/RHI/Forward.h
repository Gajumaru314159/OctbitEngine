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
	struct ShaderCompileDesc;
	struct RootSignatureDesc;
	struct PipelineStateDesc;
	struct BindingSlot;

	// クラス
	class RHI;
	class Buffer;
	class CommandList;
	class DescriptorTable;
	class DescriptorLayout;
	class SwapChain;
	class PipelineState;
	class RenderTexture;
	class RootSignature;
	class Shader;
	class Texture;
	class Sampler;
	class RenderPass;

	class GraphicFileHandle;
	class GraphicFileEvent;
	class GraphicFileQueue;

	// 内部クラス
	class GraphicObjectManager;

}