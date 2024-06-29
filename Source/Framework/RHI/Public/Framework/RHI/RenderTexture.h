//***********************************************************
//! @file
//! @brief		描画テクスチャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Constants.h>
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/Types/RenderTextureDesc.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      描画テクスチャ
    //@―---------------------------------------------------------------------------
    class RenderTexture :public Texture {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief          コンストラクタ
        //! 
        //! @param desc     定義
        //! @param name     オブジェクト名
        //@―---------------------------------------------------------------------------
        static Ref<RenderTexture> Create(const RenderTextureDesc& desc);

    public:

        //@―---------------------------------------------------------------------------
        //! @brief      定義取得
        //@―---------------------------------------------------------------------------
        virtual const RenderTextureDesc& descOfRenderTexture()const = 0;

    };

    using RenderTextureArray = FixedVector<Ref<RenderTexture>,RENDER_TARGET_MAX>;

}