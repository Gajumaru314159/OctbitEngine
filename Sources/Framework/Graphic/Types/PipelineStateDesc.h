//***********************************************************
//! @file
//! @brief		グラフィックパイプライン定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/TextureFormat.h>
#include <Framework/Graphic/Shader.h>

#include <Framework/Graphic/Types/BlendState.h>
#include <Framework/Graphic/Types/DepthStencilState.h>
#include <Framework/Graphic/Types/RasterizerState.h>

namespace ob::graphic {
	/*


	//@―---------------------------------------------------------------------------
	//! @brief  シェーダプログラムに使用できる変数の型
	//@―---------------------------------------------------------------------------
	enum class ShaderParamType {
		Unknown,		//!< 不明
		Bool,			//!< bool
		Int,			//!< int
		Int2,			//!< int2
		Int3,			//!< int3
		Int4,			//!< int4
		Float,			//!< float
		Float2,			//!< float2
		Float3,			//!< float3
		Float4,			//!< float4
		Texture2D,		//!< Texture2D
		Texture3D,		//!< Texture3D
		Matrix			//!< matrix4x4
	};



	//struct InputLayoutElement {
	//	u16				stream;
	//	u16				offset;
	//	InputFormat		format;
	//	InputSemantic	semantic;
	//	u8				semanttinIndex;
	//};

	enum class VertexInputRate {
		Vertex,
		Instance,
	};

	struct VertexInputBinding {
		u16 stream;					//!< 
		u16 stride;					//!< 頂点データの配置間隔
		VertexInputRate inputRate;	//!<
	};

	struct PipelineStateDesc {
		RootSignature rootSignature;

		VertexShader vs;
		PixelShader ps;
		//GeometryShader ps;
		//HullShader ps;
		//DomainShader ps;

		// StreamOutput
		// SampleMask
		// NodeMask
		// CacchedPSO
		// Flags

		BlendState			blendState;
		DepthStencilState	depthStencilState;
		RasterizerState		rasterizerState;
		//SampleDesc
		

		VertexLayout vertexLayout;

		vector<TextureFormat> renderTargetFormats;
		TextureFormat depthStencilFormat;
		vector<VertexInputBinding> vertexInputBinding;





			bool useWireframe = false;
		CullMode cullMode = CullMode::Back;
		bool useMultisample = true;
		bool zTest = true;
		bool zWrite = true;
		bool useStencil = true;


		// その他

		Topology primitiveTopologyType = Topology::TriangleList;
		u32 controlPointNum;//!< テッセレーションで使用するコントロールポイントの数
	};



	//@―---------------------------------------------------------------------------
	//! @brief  グラフィックパイプラインの定義
	//@―---------------------------------------------------------------------------
	struct GraphicPipelineDesc {
	public:

		GraphicPipelineDesc() {
			for (s32 i = 0; i < 8; i++) {
				textureFormats[i] = TextureFormat::RGBA32;
				blendMode[i].src = BlendFactor::ONE;
				blendMode[i].dest = BlendFactor::ZERO;
			}
		}

	public:


		RootSignature rootSignature;
		RenderPass renderPass;
		u32 subpass;
		//NodeMask
		// PipelineStateFlags

		VertexShader vs;
		PixelShader ps;
		//GeometryShader ps;
		//HullShader ps;
		//DomainShader ps;

		// 入力レイアウト
		// 入力アセンブリ定義	PrimitiveTopology




		bool useWireframe = false;
		CullMode cullMode = CullMode::Back;
		bool useMultisample = true;
		bool zTest = true;
		bool zWrite = true;
		bool useStencil = true;


		// その他

		PrimitiveTopology primitiveTopologyType = PrimitiveTopology::Triangle;

		// レンダ―ターゲット
		s32 numRenderTargets = 1;
		TextureFormat textureFormats[8];

		BlendMode blendMode[8] = {};
	};



	*/

	struct PipelineStateDesc {

	};


}// namespcae ob::graphic