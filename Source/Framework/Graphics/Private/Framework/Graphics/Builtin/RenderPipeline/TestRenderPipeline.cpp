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
        , m_imgui(view)
        , m_gbuffer(view)
    {

    }
    FGTexture TestRenderPipeline::render(FG& fg) {

        auto target = fg.import(m_view.getRenderTexture());

        // ソート設定
        
        // 描画設定
        // フィルタ設定

        FGBlackboard blackboard;

        {
			blackboard.add<GBufferData>() = fg.addPass<GBufferData>(
				"Prepare",
				[&](FGBuilder& builder, GBufferData& data) {
					rhi::RenderTextureDesc desc;
					desc.size = m_view.getRenderSize();
					{
						data.albedo = target;
					}
					{
						desc.name = "Normal";
						desc.format = rhi::TextureFormat::RGBA8;
						desc.clear.color = Color::Normal;
						data.normal =builder.write(builder.create(desc));
					}
					{
						desc.name = "Depth";
						desc.format = rhi::TextureFormat::D32;
						data.depth = builder.write(builder.create(desc));
					}
				},
				[=](const GBufferData& data, FGResources& resources, rhi::CommandList& cmdList) {
				}
			);
        }

        m_gbuffer.render(fg, blackboard);

        GBufferData& gbuffer = blackboard.get<GBufferData>();
        target = gbuffer.albedo;

        m_imgui.render(fg, target);


        return target;
    }

}