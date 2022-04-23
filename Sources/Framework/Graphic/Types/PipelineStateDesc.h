﻿//***********************************************************
//! @file
//! @brief		グラフィックパイプライン定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/RootSignature.h>
#include <Framework/Graphic/Shader.h>
#include <Framework/Graphic/RenderTexture.h>
#include <Framework/Graphic/Types/BlendDesc.h>
#include <Framework/Graphic/Types/DepthStencilDesc.h>
#include <Framework/Graphic/Types/RasterizerDesc.h>
#include <Framework/Graphic/Types/Topology.h>

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
	
	enum class VertexInputRate {
		Vertex,
		Instance,
	};

	struct VertexInputBinding {
		u16 stream;					//!< 
		u16 stride;					//!< 頂点データの配置間隔
		VertexInputRate inputRate;	//!<
	};
	*/




	//@―---------------------------------------------------------------------------
	//! @brief		型
	//! 
	//! @see		VertexAttribute
	//! @see		VertexLayout
	//@―---------------------------------------------------------------------------
	enum class Type : u32 {
		Int8,
		Int16,
		Int32,
		UInt8,
		UInt16,
		UInt32,
		Float,
	};


	//@―---------------------------------------------------------------------------
	//! @brief		セマンティクス
	//! 
	//! @details	Positionは頂点内で1度しか使用できません。
	//! @see		VertexAttribute
	//! @see		VertexLayout
	//@―---------------------------------------------------------------------------
	enum class Semantic : u32 {
		Position,		//!< 頂点
		Normal,			//!< 法線
		Binormal,		//!< 従法線
		Tangent,		//!< 接線
		Color,			//!< 色
		TexCoord,		//!< テクスチャ座標(UV座標)
		BlendIndices,	//!< ブレンド・インデックス
		BlendWeights,	//!< ブレンドの重み
		PointSize,		//!< ポイントサイズ
	};


	//@―---------------------------------------------------------------------------
	//! @brief  頂点属性
	//! @see    VertexLayout
	//@―---------------------------------------------------------------------------
	struct VertexAttribute {
		Semantic	semantic;	//!< セマンティクス
		Type		type;		//!< コンポーネント型
		s32			dimention;	//!< 次元数
		s32			offset;		//!< 頂点構造体内でのオフセット
	};


	//@―---------------------------------------------------------------------------
	//! @brief  頂点レイアウト
	//@―---------------------------------------------------------------------------
	struct VertexLayout {
		vector<VertexAttribute> attributes;	//!< 属性リスト
	};


	//@―---------------------------------------------------------------------------
	//! @brief		サンプリング定義
	//! 
	//! @details	描画速度に直結するため、アンチエイリアシングの使用を検討してください。
	//@―---------------------------------------------------------------------------
	struct SampleDesc {
		s32 count=1;		//!< ピクセル当たりのマルチサンプル数
		s32 qualitty=0;		//!< 品質
	};


	//@―---------------------------------------------------------------------------
	//! @brief  パイプラインステート定義
	//@―---------------------------------------------------------------------------
	struct PipelineStateDesc {
		RootSignature		rootSignature;	//!< ルートシグネチャ

		VertexShader		vs;				//!< 頂点シェーダ
		PixelShader			ps;				//!< ピクセルシェーダ
		//GeometryShader gs;				//!< ジオメトリシェーダ
		//HullShader hs;					//!< ハルシェーダ
		//DomainShader ds;					//!< ドメインシェーダ

		SampleDesc			sample;			//!< サンプル定義
		BlendDesc			blend;			//!< ブレンド定義
		RasterizerDesc		rasterizer;		//!< ラスタライズ定義
		DepthStencilDesc	depthStencil;	//!< デプス・ステンシル定義
		VertexLayout		vertexLayout;	//!< 頂点レイアウト
		Topology			topology;		//!< トポロジー

		RenderTexture		target;			//!< 描画先
		u32					sampleMask=0b11111111;	//!< マルチレンダーターゲットの何枚目に書き込むか(下位ビットから)
	};



}// namespcae ob::graphic