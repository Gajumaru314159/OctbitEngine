//***********************************************************
//! @file
//! @brief		RHI前方宣言
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rhi {

	// 列挙型
	enum class PresetTexture;

	// 構造体
	struct SetDescriptorTableParam;
	struct DrawParam;
	struct DrawIndexedParam;
	struct RenderPassDesc;

	// クラス
	class RHI;
	class Buffer;
	class CommandList;
	class DescriptorTable;
	class Display;
	class FrameBuffer;
	class PipelineState;
	class RenderPass;
	class RenderTexture;
	class ResourceBarrier;
	class RootSignature;
	class Shader;
	class VertexShader;
	class PixelShader;
	class GeometryShader;
	class HullShader;
	class DomainShader;
	class Texture;

	// 内部クラス
	class GraphicObjectManager;

}