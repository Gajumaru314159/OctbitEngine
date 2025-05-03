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
#include <Framework/Graphics/Builtin/RenderFeature/CameraRenderFeature.h>
#include <Framework/Graphics/Builtin/RenderFeature/LightRenderFeature.h>
#include <Framework/Graphics/Builtin/Renderer/DefferedLightRenderer.h>

namespace ob::graphics {

    class TestRenderPipeline : public RenderPipeline{
    public:
        TestRenderPipeline(RenderView& view);
        FGTexture render(FG& fg);
    private:
        RenderView& m_view;
        MaterialRenderer m_material;
        ImGuiRenderer m_imgui;
        EarlyZRenderer m_earlyZ;
        OpaqueRenderer m_opaque;
        MaskedRenderer m_masked;
        DefferedLightRenderer m_defferedLight;
    };

}