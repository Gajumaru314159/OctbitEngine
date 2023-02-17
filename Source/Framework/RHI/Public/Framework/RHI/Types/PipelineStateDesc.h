//***********************************************************
//! @file
//! @brief		グラフィックパイプライン定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Constants.h>
#include <Framework/RHI/RootSignature.h>
#include <Framework/RHI/Shader.h>
#include <Framework/RHI/RenderPass.h>
#include <Framework/RHI/Types/BlendDesc.h>
#include <Framework/RHI/Types/DepthStencilDesc.h>
#include <Framework/RHI/Types/RasterizerDesc.h>
#include <Framework/RHI/Types/Topology.h>

namespace ob::rhi {

#pragma region Enum

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

#pragma endregion

#pragma region Sub Structure

	//@―---------------------------------------------------------------------------
	//! @brief  頂点属性
	//! @see    VertexLayout
	//@―---------------------------------------------------------------------------
	struct VertexAttribute {

		Semantic	semantic;	//!< セマンティクス
		s32			offset;		//!< 頂点構造体内でのオフセット
		Type		type;		//!< コンポーネント型
		s32			dimention;	//!< 次元数
		s32			index;		//!< セマンティクス内インデックス

	public:

		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ
		//@―---------------------------------------------------------------------------
		VertexAttribute() = default;

		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ
		//@―---------------------------------------------------------------------------
		VertexAttribute(Semantic semantic, s32 offset,Type type,s32 dimention=1,s32 index = 0)
			:semantic(semantic), offset(offset), type(type), dimention(dimention),index(index) {}

	};


	//@―---------------------------------------------------------------------------
	//! @brief  頂点レイアウト
	//@―---------------------------------------------------------------------------
	struct VertexLayout {
		Array<VertexAttribute> attributes;	//!< 属性リスト
	};


	//@―---------------------------------------------------------------------------
	//! @brief		サンプリング定義
	//! 
	//! @details	描画速度に直結するため、アンチエイリアシングの使用を検討してください。
	//@―---------------------------------------------------------------------------
	struct SampleDesc {
		s32 count	=1;		//!< ピクセル当たりのマルチサンプル数
		s32 qualitty=0;		//!< 品質
	};

#pragma endregion

	//@―---------------------------------------------------------------------------
	//! @brief  パイプラインステート定義
	//@―---------------------------------------------------------------------------
	struct PipelineStateDesc {
		
		String				name;							//!< 名前

		Ref<RenderPass>		renderPass;						//!< レンダーパス
		s32					subpass;						//!< サブパスインデックス

		Ref<RootSignature>	rootSignature;					//!< ルートシグネチャ
		VertexLayout		vertexLayout;					//!< 頂点レイアウト

		Ref<Shader>			vs;								//!< 頂点シェーダ
		//Ref<GeometryShader>	gs;								//!< ジオメトリシェーダ
		//Ref<HullShader>		hs;								//!< ハルシェーダ
		//Ref<DomainShader>	ds;								//!< ドメインシェーダ
		Ref<Shader>			ps;								//!< ピクセルシェーダ

		SampleDesc			sample;							//!< サンプル定義
		BlendDescList		blend;							//!< ブレンド定義
		RasterizerDesc		rasterizer;						//!< ラスタライズ定義
		DepthStencilDesc	depthStencil;					//!< デプス・ステンシル定義

		Topology			topology=Topology::TriangleList;//!< GeometoryShaderでのトポロジー
		u32					sampleMask=-1;					//!< マルチレンダーターゲットの何枚目に書き込むか(下位ビットから)
	};

}// namespcae ob::rhi