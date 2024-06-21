//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/Graphics/Render/RenderPipeline.h>
#include <Framework/Graphics/Builtin/RenderFeature/ImGuiRenderFeature.h>
#include <Framework/Graphics/Builtin/RenderFeature/MaterialRenderFeature.h>

namespace ob::graphics {

    class TestRenderPipeline : public RenderPipeline{
    public:
        TestRenderPipeline(RenderView& view);
        void render(FG& fg);
    private:
        RenderView& m_view;
        ImGuiRenderer m_imgui;
        MaterialRenderer m_material;
    };

}