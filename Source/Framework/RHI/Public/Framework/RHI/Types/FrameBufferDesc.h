//***********************************************************
//! @file
//! @brief		フレームバッファ定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  フレームバッファ定義
    //@―---------------------------------------------------------------------------
    struct FrameBufferDesc {
        String			            name;
        Ref<RenderPass>             renderPass;
        Array<Ref<RenderTexture>>   attachments;
    };

}