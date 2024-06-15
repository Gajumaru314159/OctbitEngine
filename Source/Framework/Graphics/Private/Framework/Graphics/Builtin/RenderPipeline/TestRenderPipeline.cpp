//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/Graphics/Builtin/RenderPipeline/TestRenderPipeline.h>
#include <Framework/Graphics/Render/RenderView.h>

namespace ob::graphics {


    TestRenderPipeline::TestRenderPipeline(RenderView& view)
        : m_view(view)
        , m_imgui(view)
    {

    }
    void TestRenderPipeline::render(FG& fg) {

        FGTexture::Desc desc;
        FGTexture target{ m_view.getRenderTexture() };

        auto resource = fg.import("Target", desc, std::move(target));

        m_imgui.render(fg, resource);

    }

}