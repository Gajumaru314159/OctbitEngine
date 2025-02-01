//***********************************************************
//! @file
//! @brief		描画テクスチャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Constants.h>
#include <Framework/RHI/ITexture.h>
#include <Framework/RHI/Types/RenderTextureDesc.h>

namespace ob::rhi {

    //! @brief      描画テクスチャ
    class IRenderTexture :public ITexture {
    public:

        //! @brief      定義取得
        virtual const RenderTextureDesc& descOfRenderTexture()const = 0;

    };

}