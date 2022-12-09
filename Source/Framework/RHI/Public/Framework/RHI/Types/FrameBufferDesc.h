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
        Size size;
    };

    struct FrameBufferDescHelper:FrameBufferDesc {
        void setSize(s32 width, s32 height) {
            size = Size(width, height);
        }
        void addAttachment(Texture& texture) {
            attachments.push_back(texture);
        }
    };

}// namespcae ob::rhi