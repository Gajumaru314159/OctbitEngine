//***********************************************************
//! @file
//! @brief		描画テクスチャ
//! @author		Gajumaru
//***********************************************************
#pragma once
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

    };

}