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

    void RenderView::startup() {

        for (auto& step : m_steps) {
            step->setup(m_textureManager);
        }

    }


    void RenderView::addStep(UPtr<RenderStep> step) {
        m_steps.emplace_back(std::move(step));
    }

    void RenderView::render() {
        for (auto& step : m_steps) {
            step->render(*this);
        }
    }

    void RenderView::resize() {
        // TODO TextureManagerのリサイズ
    }


    auto RenderView::getRenderContext()const->RenderContext& {
        OB_ASSERT_EXPR(m_context);
        return *m_context;
    }
}