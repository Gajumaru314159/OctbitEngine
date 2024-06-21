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
        , m_material(view)
    {

    }
    void TestRenderPipeline::render(FG& fg) {

        auto resource = fg.import(m_view.getRenderTexture());


        resource = m_material.render(fg, "Opaque", resource);

        m_imgui.render(fg, resource);

    }

}