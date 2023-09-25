//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/Graphics/Render/RenderStep.h>
#include <Framework/Graphics/Render/RenderContext.h>

namespace ob::graphics {

    RenderView::RenderView(RenderViewDesc& desc) 
        : m_textureManager(desc.size)
    {
    }

    RenderView::~RenderView() {

    }

    s32 RenderView::getPriority()const {
        return 0;
    }

}