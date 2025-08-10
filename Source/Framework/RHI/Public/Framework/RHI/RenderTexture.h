//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Constants.h>
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/Types/RenderTextureDesc.h>

namespace ob::rhi {

    //! @brief      描画テクスチャ
    class RenderTexture :public Texture {
    public:

        //! @brief          コンストラクタ
        //! 
        //! @param desc     定義
        static Ref<RenderTexture> Create(const RenderTextureDesc& desc);

        //! @brief      テクスチャフォーマットがサポートされているか
        static bool Supports(TextureFormat format);

    public:

        //! @brief      定義取得
        virtual const RenderTextureDesc& descOfRenderTexture()const = 0;

    };

    using RenderTargetArray = FixedVector<Ref<RenderTexture>,RENDER_TARGET_MAX>;

}