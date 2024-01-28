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

	// 内部クラス
	class GraphicObjectManager;

}