//***********************************************************
//! @file
//! @brief		マテリアル定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Types/PipelineStateDesc.h>

// HashMap<Name, MaterialPass> で使用するためインクルード
#include <Framework/RHI/RootSignature.h>
#include <Framework/RHI/Shader.h>

namespace ob::graphics {

    //@―---------------------------------------------------------------------------
    //! @brief  頂点属性
    //! @see    VertexLayout
    //@―---------------------------------------------------------------------------
    struct InputLayout {

        rhi::Semantic	semantic;	//!< セマンティクス
        rhi::Type		type;		//!< コンポーネント型
        s32			    dimention;	//!< 次元数
        s32			    index;		//!< セマンティクス内インデックス

    public:

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //@―---------------------------------------------------------------------------
        InputLayout() = default;

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //@―---------------------------------------------------------------------------
        InputLayout(rhi::Semantic semantic, rhi::Type type, s32 dimention = 1, s32 index = 0)
            :semantic(semantic), type(type), dimention(dimention), index(index) {}

    };

    //@―---------------------------------------------------------------------------
    //! @brief  マテリアルパス定義
    //@―---------------------------------------------------------------------------
    struct MaterialPass {
        Array<rhi::TextureFormat>	    colors;			    //!< 描画先フォーマット
        Optional<rhi::TextureFormat>	depth;

        Ref<rhi::RootSignature>         rootSignature;
        Ref<rhi::Shader>                vs;
        Ref<rhi::Shader>                ps;

        rhi::BlendDescList		        blends;
        rhi::RasterizerDesc		        rasterizer;
        rhi::DepthStencilDesc	        depthStencil;

        Array<InputLayout>              requiredLayout;
    };

    //@―---------------------------------------------------------------------------
    //! @brief  マテリアル定義
    //@―---------------------------------------------------------------------------
    struct MaterialDesc{
        String          name;
        
        Array<String>   floatProperties;
        Array<String>   colorProperties;
        Array<String>   matrixProperties;
        Array<String>   textureProperties;

        Map<String, MaterialPass,std::less<>> passes;
    };

}