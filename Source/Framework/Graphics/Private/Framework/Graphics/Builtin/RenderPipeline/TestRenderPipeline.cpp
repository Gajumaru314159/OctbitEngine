//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Forward.h>
#include <Framework/Graphics/Builtin/RenderPipeline/TestRenderPipeline.h>
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/Graphics/Material/MaterialSystem.h>
#include <Framework/Graphics/Builtin/RenderFeature/MaterialRenderFeature.h>

namespace ob::graphics {

    enum class RenderSortType {
        None,
        BackToFront,
        FrontToBack,
    };

    struct RenderSortDesc {
        Vec3 cameraPosition;
        Vec3 cameraFront;
        RenderSortType sortType;

    };
    struct RenderDesc {
        // Ref<Material> overrideMaterial

        u32 layerMask;
        u32 renderingLayerMask;
        Range queueIndex;
    };

    TestRenderPipeline::TestRenderPipeline(RenderView& view)
        : m_view(view)
		, m_material(view)
        , m_earlyZ(view)
        , m_opaque(view)
        , m_masked(view)
        , m_defferedLight(view)
        , m_imgui(view)
    {
    }
    FGTexture TestRenderPipeline::render(FG& fg) {

        MaterialBlockSet blocks{
            MaterialSystem::Instance().getGlobalBlock(),
            m_view.findFeature<MaterialRenderFeature>()->getSceneBlock(),
            m_material.getViewBlock()
        };

        // ソート設定
        
        // 描画設定
        // フィルタ設定

		FGDummy dummy;
        m_material.render(fg, dummy);

        FGBlackboard blackboard;

        {
            auto& data = blackboard.add<GBufferData>() = fg.addPass<GBufferData>(
                "Prepare",
                [&](FGBuilder& builder, GBufferData& data) {

                    builder.read(dummy);

                    rhi::RenderTextureDesc desc;
                    desc.size = m_view.getRenderSize();
                    {
                        desc.name = "Albedo";
                        desc.format = rhi::TextureFormat::RGBA8;
                        desc.clear.color = Color::Black;
                        data.albedo = builder.write(builder.create(desc));
                    }
                    {
                        desc.name = "Normal";
                        desc.format = rhi::TextureFormat::RGBA8;
                        desc.clear.color = Color::Normal;
                        data.normal = builder.write(builder.create(desc));
                    }
                    {
                        desc.name = "Depth";
                        desc.format = rhi::TextureFormat::D32;
                        data.depth = builder.write(builder.create(desc));
                    }
                    {
                        desc.name = "UV";
                        desc.format = rhi::TextureFormat::RGBA8;
                        data.uv = builder.write(builder.create(desc));
                    }
                },
                [=](const GBufferData& data, FGResources& resources, Ref<rhi::CommandList>& cmdList) {
                }
            );
        }

        m_earlyZ.render(fg, blackboard);
        m_opaque.render(fg, blackboard);
        m_masked.render(fg, blackboard);

        auto target = fg.import(m_view.getRenderTexture());
        m_defferedLight.render(fg, blackboard, target);

        m_imgui.render(fg, target);

        return target;
    }

}