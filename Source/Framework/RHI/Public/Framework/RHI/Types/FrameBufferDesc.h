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
        String			name;			//!< 名前
        Ref<RenderPass> renderPass;
        Array<Ref<RenderTexture>> attachments;
    };

}// namespcae ob::rhi