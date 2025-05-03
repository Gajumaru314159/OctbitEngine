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
		s32                             quality;

        // PipelineState周りの必須情報
        Ref<rhi::Shader> 		        vs;
        Ref<rhi::Shader> 		        ps;
        Vector<InputLayout>		        inputLayout;
        rhi::RenderTargetFormatArray    colors;
        Optional<rhi::TextureFormat>	depth;
		rhi::SampleDesc		            sample;
		rhi::BlendDescList		        blend;
        rhi::BlendDescList		        blends;
        rhi::RasterizerDesc		        rasterizer;
        rhi::DepthStencilDesc	        depthStencil;
    };

    struct MaterialPass {
		Vector<ShaderSet>   qualities;
    };

    using MaterialPassMap = Map<String, MaterialPass,std::less<>>;

    //! @brief  マテリアル定義
    struct MaterialDesc{
        String          name;
        
        Vector<String>  textures;
        Vector<String>  buffers;
        Vector<String>  matrices;
        Vector<String>  vectors;
        Vector<String>  scalars;
        Vector<String>  integers;

        MaterialPassMap	passes;
    };

}