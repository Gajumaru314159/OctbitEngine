//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/Graphics/Render/RenderStep.h>
#include <Framework/Graphics/Command/CommandRecorder.h>

namespace ob::graphics {

    RenderView::RenderView(RenderViewDesc& desc) 
    {
        m_textureManager = std::make_unique<TextureManager>(desc.size);
    }

    RenderView::~RenderView() {

    }

    //@―---------------------------------------------------------------------------
    //! @brief      プライオリティ取得
    //@―---------------------------------------------------------------------------
    s32 RenderView::getPriority()const {
        return 0;
    }

    //@―---------------------------------------------------------------------------
    //! @brief      描画
    //@―---------------------------------------------------------------------------
    void RenderView::render(CommandRecorder& recorder) {

        RenderArgs args{ *this };

        for (auto& step : m_steps) {
            step->render(recorder,args);
        }

    }

}