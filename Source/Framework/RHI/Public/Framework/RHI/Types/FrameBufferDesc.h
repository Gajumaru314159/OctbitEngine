//***********************************************************
//! @file
//! @brief		デスクリプタ定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RenderPass.h>
#include <Framework/RHI/Texture.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  ディスクリプタ・タイプ
    //@―---------------------------------------------------------------------------
    struct FrameBufferDesc {
        RenderPass renderPass;
        Array<Texture> attachments;
    };

}// namespcae ob::rhi