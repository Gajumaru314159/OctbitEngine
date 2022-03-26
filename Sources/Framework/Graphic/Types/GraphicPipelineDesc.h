//***********************************************************
//! @file
//! @brief		ファイル説明
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
		UNDEFINED,
		BOOL,
		INT,
		INT2,
		INT3,
		INT4,
		FLOAT,
		FLOAT2,
		FLOAT3,
		FLOAT4,
		TEXTURE2D,
		TEXTURE3D,
		MATRIX
	};


	//@―---------------------------------------------------------------------------
	//! @brief  描画に使用するトポロジーの種類
	//@―---------------------------------------------------------------------------
	enum class PrimitiveTopology {
		Point,		//!< 点群
		Line,		//!< ライン・リスト
		Triangle,	//!< トライアングル・リスト
	};



	enum class CullMode {
		Back,	//!< 裏面をカリング
		Front,	//!< 表面をカリング
		None,	//!< カリングなし
	};

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
		// シェーダID
		Shader vs;
		Shader ps;
		Shader gs;
		Shader hs;
		Shader ds;

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





    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob::graphic