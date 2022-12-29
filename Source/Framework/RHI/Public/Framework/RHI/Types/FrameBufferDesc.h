//***********************************************************
//! @file
//! @brief		デスクリプタ定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RenderPass.h>
#include <Framework/RHI/RenderTexture.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  ディスクリプタ・タイプ
    //@―---------------------------------------------------------------------------
    struct FrameBufferDesc {
        Ref<RenderPass> renderPass;
        Array<Ref<RenderTexture>> attachments;
    };

}// namespcae ob::rhi