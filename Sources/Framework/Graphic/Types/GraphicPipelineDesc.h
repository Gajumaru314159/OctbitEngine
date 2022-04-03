//***********************************************************
//! @file
//! @brief		グラフィックパイプライン定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/TextureFormat.h>
#include <Framework/Graphic/Shader.h>

namespace ob::graphic {

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


	//@―---------------------------------------------------------------------------
	//! @brief  描画に使用するトポロジーの種類
	//@―---------------------------------------------------------------------------
	enum class PrimitiveTopology {
		Point,		//!< 点群
		Line,		//!< ライン・リスト
		Triangle,	//!< トライアングル・リスト
	};


	//@―---------------------------------------------------------------------------
	//! @brief  カリング・モード
	//@―---------------------------------------------------------------------------
	enum class CullMode {
		Back,	//!< 裏面をカリング
		Front,	//!< 表面をカリング
		None,	//!< カリングなし
	};


	//@―---------------------------------------------------------------------------
	//! @brief  ブレンド・ファクター
	//@―---------------------------------------------------------------------------
	enum class BlendFactor {
		ONE,
		ZERO,
		SRC_COLOR,
		SRC_INV_COLOR,
		SRC_ALPHA,
		SRC_INV_ALPHA,
		DEST_COLOR,
		DEST_INV_COLOR,
		DEST_ALPHA,
		DEST_INV_ALPHA,
		SRC_ALPHA_SAT
	};


	//@―---------------------------------------------------------------------------
	//! @brief  描画時に使用する合成モード
	//@―---------------------------------------------------------------------------
	struct BlendMode {
		BlendFactor src;
		BlendFactor dest;
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

		VertexShader vs;
		PixelShader ps;

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

}// namespcae ob::graphic