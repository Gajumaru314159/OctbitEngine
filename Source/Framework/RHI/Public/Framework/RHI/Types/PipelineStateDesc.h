//***********************************************************
//! @file
//! @brief		パイプラインステート定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Constants.h>
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Types/BlendDesc.h>
#include <Framework/RHI/Types/DepthStencilDesc.h>
#include <Framework/RHI/Types/RasterizerDesc.h>
#include <Framework/RHI/Types/Topology.h>
#include <Framework/RHI/Types/TextureFormat.h>
#include <Framework/RHI/Types/VertexLayout.h>

namespace ob::rhi {

	//! @brief		サンプリング定義
	//! 
	//! @details	描画速度に直結するため、アンチエイリアシングの使用を検討してください。
	struct SampleDesc {
		s32 count	=1;		//!< ピクセル当たりのマルチサンプル数
		s32 qualitty=0;		//!< 品質
	};

	//! @brief  レンダーターゲット・フォーマット・配列
	using RenderTargetFormatArray = FixedVector<TextureFormat, RENDER_TARGET_MAX>;

	//! @brief  パイプラインステート定義
	struct PipelineStateDesc {
		
		String					name;							//!< 名前

		RenderTargetFormatArray	colors;						//!< 描画先フォーマット
		Optional<TextureFormat>	depth;

		Ref<RootSignature>		rootSignature;					//!< ルートシグネチャ
		VertexLayout			vertexLayout;					//!< 頂点レイアウト

		Ref<Shader>				vs;								//!< 頂点シェーダ
		Ref<Shader>				gs;								//!< ジオメトリシェーダ
		Ref<Shader>				hs;								//!< ハルシェーダ
		Ref<Shader>				ds;								//!< ドメインシェーダ
		Ref<Shader>				ps;								//!< ピクセルシェーダ

		SampleDesc				sample;							//!< サンプル定義
		BlendDescList			blend;							//!< ブレンド定義
		RasterizerDesc			rasterizer;						//!< ラスタライズ定義
		DepthStencilDesc		depthStencil;					//!< デプス・ステンシル定義

		Topology				topology=Topology::TriangleList;//!< GeometryShaderでのトポロジー
		u32						sampleMask=~u32(0);				//!< マルチレンダーターゲットの何枚目に書き込むか(下位ビットから)
	};

}