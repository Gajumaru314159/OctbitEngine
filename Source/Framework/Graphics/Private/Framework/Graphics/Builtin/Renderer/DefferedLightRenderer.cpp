//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Builtin/Renderer/DefferedLightRenderer.h>
#include <Framework/Graphics/Material/Material.h>
#include <Framework/Graphics/Mesh/Mesh.h>

namespace ob::graphics {


	OpaqueRenderer::OpaqueRenderer(RenderView& view)
		: m_view(view)
	{

	}

	bool OpaqueRenderer::render(FG& fg, FGBlackboard& blackboard)const {

		auto rect = m_view.getScaledRect();
		auto viewport = m_view.getViewport();

		auto& gbuffer = blackboard.get<GBufferData>();

		blackboard.get<GBufferData>() = fg.addPass<GBufferData>(
			"Opaque",
			[&](FGBuilder& builder, GBufferData& data) {
				data.albedo = builder.write(gbuffer.albedo);
				data.normal = builder.write(gbuffer.normal);
				data.depth = builder.write(gbuffer.depth);
				data.uv = builder.write(gbuffer.uv);
			},
			[=](const GBufferData& data, FGResources& resources, rhi::CommandList& cmdList) {
				if (auto feature = m_view.findFeature<MaterialRenderFeature>()) {

					cmdList.pushMarker("Opaque");

					cmdList.setRenderTargets(
						{ resources.get(data.albedo) ,resources.get(data.normal) ,resources.get(data.uv) },
						resources.get(data.depth)
					);

					cmdList.clearColors();
					cmdList.clearDepthStencil();
					cmdList.setViewport(&viewport, 1);
					cmdList.setScissorRect(&rect, 1);

					feature->render("Opaque", cmdList);

					cmdList.popMarker();
				}
			}
		);

		return true;
	}

	//--------------------------------

	MaskedRenderer::MaskedRenderer(RenderView& view)
		: m_view(view)
	{

	}

	bool MaskedRenderer::render(FG& fg, FGBlackboard& blackboard)const {

		IntRect rect = m_view.getScaledRect();

		auto& gbuffer = blackboard.get<GBufferData>();

		blackboard.get<GBufferData>() = fg.addPass<GBufferData>(
			"Masked",
			[&](FGBuilder& builder, GBufferData& data) {
				rhi::RenderTextureDesc desc;
				desc.size = m_view.getRenderSize();

				data.albedo = builder.write(gbuffer.albedo);
				data.normal = builder.write(gbuffer.normal);
				data.depth = builder.write(gbuffer.depth);
				data.uv = builder.write(gbuffer.uv);

			},
			[=](const GBufferData& data, FGResources& resources, rhi::CommandList& cmdList) {
				if (auto feature = m_view.findFeature<MaterialRenderFeature>()) {

					cmdList.pushMarker("Masked");

					Viewport vp(rect.left, rect.top, rect.right, rect.bottom, 1, 0);

					cmdList.setRenderTargets(
						{ resources.get(data.albedo) ,resources.get(data.normal) },
						resources.get(data.depth)
					);

					cmdList.setViewport(&vp, 1);
					cmdList.setScissorRect(&rect, 1);

					feature->render("Masked", cmdList);

					cmdList.popMarker();
				}
			}
		);

		return true;
	}

	//----

	DefferedLightRenderer::DefferedLightRenderer(RenderView& view)
		: m_view(view)
	{
		m_material = [&] {

			auto code = File::ReadAllText("Assets/Shader/DeferredLight.hlsl");
			OB_ASSERT(code, "ファイル読み込み失敗");

			MaterialDesc desc;
			desc.name = "DefferedLight";
			desc.colorProperties = { "Color" };
			desc.matrixProperties = { "Matrix" };
			desc.textureProperties = { "Main" ,"Normal","Depth" ,"UV" };

			MaterialPass lighting;
			lighting.colors = { TextureFormat::RGBA8};
			lighting.vs = Shader::CompileVS(code.value());
			lighting.ps = Shader::CompilePS(code.value());
			lighting.blends = { BlendDesc::AlphaBlend };
			lighting.requiredLayout = {
				{Semantic::Position,ElementType::Float,4},
				{Semantic::TexCoord,ElementType::Float,2},
			};

			desc.passes.emplace("PostProcess", lighting);
			return Material::Create(desc);
		}();

		MeshData md;
		md.name = "FullScreen";
		md.positions = {
			Vec3(-1,1,0),
			Vec3(1,1,0),
			Vec3(-1,-1,0),
			Vec3(1,-1,0),
		};
		md.uvs = {
			Vec2(0,0),
			Vec2(1,0),
			Vec2(0,1),
			Vec2(1,1),
		};
		md.indices = {
			0,1,2,
			1,3,2
		};
		auto& submesh = md.submeshes.emplace_back();
		submesh.indexCount = 6;

		m_mesh = Mesh::Create(md);

	}

	bool DefferedLightRenderer::render(FG& fg, FGBlackboard& blackboard,FGTexture& accumulate)const {

		IntRect rect = m_view.getScaledRect();

		auto& gbuffer = blackboard.get<GBufferData>();

		struct Data {
			// in
			FGTexture albedo;
			FGTexture normal;
			FGTexture depth;
			FGTexture uv;
			// out
			FGTexture accumulate;
		};

		auto& data = fg.addPass<Data>(
			"Deffered Lighting",
			[&](FGBuilder& builder, Data& data) {
				rhi::RenderTextureDesc desc;
				desc.size = m_view.getRenderSize();

				data.albedo = builder.read(gbuffer.albedo);
				data.normal = builder.read(gbuffer.normal);
				data.depth = builder.read(gbuffer.depth);
				data.uv = builder.read(gbuffer.uv);
				data.accumulate = builder.write(accumulate);
			},
			[=](const Data& data, FGResources& resources, rhi::CommandList& cmdList) {


				cmdList.pushMarker("Deffered Lighting");


				Viewport vp(rect.left, rect.top, rect.right, rect.bottom, 1, 0);

				auto albedo = resources.get(data.albedo);
				auto normal = resources.get(data.normal);
				auto depth = resources.get(data.depth);
				auto uv = resources.get(data.uv);
				m_material->setTexture("Main", albedo);
				m_material->setTexture("Normal", normal);
				m_material->setTexture("Depth", depth);
				m_material->setTexture("UV", uv);

				cmdList.setRenderTarget(resources.get(data.accumulate));

				cmdList.setViewport(&vp, 1);
				cmdList.setScissorRect(&rect, 1);

				Ref<rhi::CommandList> cmd = &cmdList;
				Matrix mtx;

				m_material->record(cmd, mtx, m_mesh, 0, "PostProcess");

				cmdList.popMarker();

			}
		);

		accumulate = data.accumulate;

		return true;
	}

}