//***********************************************************
//! @file
//! @brief		マテリアル定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Constants.h>
#include <Framework/RHI/Shader.h>
#include <Framework/RHI/Types/PipelineStateDesc.h>

namespace ob::graphics {

    //! @brief  頂点属性
    //! @see    VertexLayout
    struct InputLayout {

        rhi::Semantic	    semantic;	//!< セマンティクス
        rhi::ElementType	type;		//!< コンポーネント型
        s32			        dimention;	//!< 次元数
        s32			        index;		//!< セマンティクス内インデックス

    public:

        //! @brief      コンストラクタ
        InputLayout() = default;

        //! @brief      コンストラクタ
        InputLayout(rhi::Semantic semantic, rhi::ElementType type, s32 dimention = 1, s32 index = 0)
            :semantic(semantic), type(type), dimention(dimention), index(index) {}

    };

    struct ShaderSet {
        Ref<rhi::Shader> 		vs;
        Ref<rhi::Shader> 		ps;

        // PipelineState周りの必須情報
        Vector<InputLayout>		        inputLayout;
        rhi::RenderTargetFormatArray    colors;
        Optional<rhi::TextureFormat>	depth;
        rhi::BlendDescList		        blends;
        rhi::RasterizerDesc		        rasterizer;
        rhi::DepthStencilDesc	        depthStencil;
    };

    // TODO 効率的なキーワード管理
    using ShaderKeywordSet = Set<String, std::less<>>;
    using ShaderKeywordMap = Map<String, bool, std::less<>>;


    using ShaderMap = Map<ShaderKeywordSet, ShaderSet>;

    struct MaterialPass {
        String              name;
        ShaderKeywordSet	keywords;	// RenderPassのキーワード RENDER_PASS_EARLY_Z
        s32					lodNum;		// SHADER_QUALITY_0
        s32					lodMax;		// 最低限保証する品質 (遠景で使用している単色シェーダーは低品質にしたとしても使ってはいけないなど)
    };
    using MaterialPassMap = Map<String, MaterialPass,std::less<>>;

    //! @brief  マテリアル定義
    struct MaterialDesc{
        String          name;
        
        Vector<String>  textures;
        Vector<String>  buffers;
        Vector<String>  matrices;
        Vector<String>  colors;
        Vector<String>  vectors;
        Vector<String>  scalars;

        MaterialPassMap	passes;
        ShaderMap 		shaders;	// 対応するものがなければエラーシェーダーにフォールバック
    };

}