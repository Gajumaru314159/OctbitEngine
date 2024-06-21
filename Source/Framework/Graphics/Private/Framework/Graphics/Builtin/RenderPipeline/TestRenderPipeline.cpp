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
    FGTexture TestRenderPipeline::render(FG& fg) {

        auto rt = fg.import(m_view.getRenderTexture());


        rt = m_material.render(fg, "Opaque", rt);

        rt = m_imgui.render(fg, rt);

        return rt;
    }

}