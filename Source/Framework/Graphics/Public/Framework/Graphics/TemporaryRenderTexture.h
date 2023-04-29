//***********************************************************
//! @file
//! @brief		TemporaryRenderTexture
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/TextureDesc.h>

namespace ob::graphics {

    //@―---------------------------------------------------------------------------
    //! @brief      TemporaryRenderTexture
    //! @details    1フレーム内でのみ使用するレンダーテクスチャ
    //@―---------------------------------------------------------------------------
    struct TemporaryRenderTexture {
        rhi::TextureDesc desc;
    };

}