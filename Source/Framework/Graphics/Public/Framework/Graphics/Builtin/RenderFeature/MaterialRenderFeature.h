//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/FrameGraph/FG.h>

#include <Framework/Graphics/Render/RenderView.h>

#include <Framework/RHI/CommandList.h>

namespace ob::graphics {

	struct Renderable {
		Ref<Mesh>		mesh;
		s32				submesh;
		Ref<Material>	material;
		u8				flags;
		// CastShadow
		// ReveievShadow
	};

	struct GBufferData {
		FGTexture albedo;
		FGTexture normal;
		FGTexture depth;
	};


	//@―---------------------------------------------------------------------------
	//! @brief      マテリアル描画機能
	//@―---------------------------------------------------------------------------
	class MaterialRenderFeature : public RenderFeature {
	public:

		OB_RTTI();

		MaterialRenderFeature();

		void addRenderable(const Ref<Mesh>& mesh, const Ref<Material>& material);

		//@―---------------------------------------------------------------------------
		//! @brief      描画
		//@―---------------------------------------------------------------------------
		bool render(FG& fg, RenderView& view,String pass, FGTexture& targets)const;

		//@―---------------------------------------------------------------------------
		//! @brief      描画
		//@―---------------------------------------------------------------------------
		bool render(StringView pass,rhi::CommandList& cmdList)const;

	private:

		Map<String, Array<Renderable>,std::less<>> m_renderablesMap;

	};

	class MaterialRenderer {
	public:
		MaterialRenderer(RenderView& view) 
			: m_view(view)
		{

		}

		bool render(FG& fg, String pass, FGTexture& targets) {
			if (auto feature = m_view.findFeature<MaterialRenderFeature>()) {
				return feature->render(fg, m_view,pass, targets);
			}
			return false;
		}

	private:
		RenderView& m_view;
	};


	class GBufferRenderer {
	public:
		GBufferRenderer(RenderView& view)
			: m_view(view)
		{

		}

		bool render(FG& fg, FGBlackboard& blackboard) {

			IntRect rect = m_view.getScaledRect();

			auto& gbuffer = blackboard.get<GBufferData>();

			blackboard.get<GBufferData>() = fg.addPass<GBufferData>(
				"Opaque",
				[&](FGBuilder& builder, GBufferData& data) {
					rhi::RenderTextureDesc desc;
					desc.size = m_view.getRenderSize();
					
					data.albedo = builder.write(gbuffer.albedo);
					data.normal = builder.write(gbuffer.normal);
					data.depth = builder.write(gbuffer.depth);

					//{
					//	desc.name = "Albedo";
					//	desc.format = rhi::TextureFormat::RGBA8;
					//	desc.clear.color = Color::Black;
					//	data.albedo = builder.write(builder.create(desc));
					//}
					//{
					//	desc.name = "Normal";
					//	desc.format = rhi::TextureFormat::RGBA8;
					//	desc.clear.color = Color::Normal;
					//	data.normal = builder.write(builder.create(desc));
					//}
					//{
					//	desc.name = "Depth";
					//	desc.format = rhi::TextureFormat::D32;
					//	data.depth = builder.write(builder.create(desc));
					//}
				},
				[=](const GBufferData& data, FGResources& resources, rhi::CommandList& cmdList) {
					if (auto feature = m_view.findFeature<MaterialRenderFeature>()) {

						cmdList.pushMarker("Opaque");

						Viewport vp(rect.left, rect.top, rect.right, rect.bottom, 1, 0);

						auto albedo = resources.get(data.albedo);
						auto normal = resources.get(data.normal);

						cmdList.setRenderTargets(
							{ albedo ,normal},
							resources.get(data.depth)
						);

						cmdList.clearColors();
						cmdList.clearDepthStencil();
						cmdList.setViewport(&vp, 1);
						cmdList.setScissorRect(&rect, 1);

						feature->render("Opaque",cmdList);

						cmdList.popMarker();
					}
				}
			);

			return true;
		}
	private:
		RenderView& m_view;
	};
}