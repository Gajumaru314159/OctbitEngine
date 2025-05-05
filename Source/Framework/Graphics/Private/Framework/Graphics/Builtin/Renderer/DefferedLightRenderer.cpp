//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Builtin/Renderer/DefferedLightRenderer.h>
#include <Framework/Graphics/Material/Material.h>
#include <Framework/Graphics/Material/MaterialBlock.h>
#include <Framework/Graphics/Mesh/Mesh.h>

namespace ob::graphics {

	EarlyZRenderer::EarlyZRenderer(RenderView& view, MaterialRenderer& material)
		: m_view(view)
		, m_materialRenderer(material)
	{

	}

	bool EarlyZRenderer::render(FG& fg, FGBlackboard& blackboard)const {

		auto& gbuffer = blackboard.get<GBufferData>();

		blackboard.get<GBufferData>() = fg.addPass<GBufferData>(
			"EarlyZ",
			[&](FGBuilder& builder, GBufferData& data) {
				data.albedo = builder.write(gbuffer.albedo);
				data.normal = gbuffer.normal;
				data.depth = builder.write(gbuffer.depth);
				data.uv = gbuffer.uv;
			},
			[=](const GBufferData& data, FGResources& resources, Ref<rhi::CommandList>& cmdList) {
				if (auto feature = m_view.findFeature<MaterialRenderFeature>()) {

					MaterialBlockSet blocks(m_materialRenderer);

					cmdList->pushMarker("EarlyZ");

					RenderPassDesc renderPass;
					renderPass.colors.emplace_back(resources.get(data.albedo), RenderPassBeforeAccessType::NoAccess, RenderPassAfterAccessType::NoAccess);
					renderPass.depth = { resources.get(data.depth), RenderPassBeforeAccessType::Clear, RenderPassAfterAccessType::Preserve };

					cmdList->beginRenderPass(renderPass);

					feature->render(cmdList,blocks,"EarlyZ");

					cmdList->endRenderPass();

					cmdList->popMarker();
				}
			}
		);

		return true;
	}

	//--------------------------------

	OpaqueRenderer::OpaqueRenderer(RenderView& view, MaterialRenderer& material)
		: m_view(view)
		, m_materialRenderer(material)
	{

	}

	bool OpaqueRenderer::render(FG& fg, FGBlackboard& blackboard)const {

		auto& gbuffer = blackboard.get<GBufferData>();

		blackboard.get<GBufferData>() = fg.addPass<GBufferData>(
			"Opaque",
			[&](FGBuilder& builder, GBufferData& data) {
				data.albedo = builder.write(gbuffer.albedo);
				data.normal = builder.write(gbuffer.normal);
				data.depth = builder.write(gbuffer.depth);
				data.uv = builder.write(gbuffer.uv);
			},
			[=](const GBufferData& data, FGResources& resources, Ref<rhi::CommandList>& cmdList) {
				if (auto feature = m_view.findFeature<MaterialRenderFeature>()) {

					cmdList->pushMarker("Opaque");

					RenderPassDesc renderPass;
					renderPass.colors.emplace_back(resources.get(data.albedo), RenderPassBeforeAccessType::Clear, RenderPassAfterAccessType::Preserve);
					renderPass.colors.emplace_back(resources.get(data.normal), RenderPassBeforeAccessType::Clear, RenderPassAfterAccessType::Preserve);
					renderPass.colors.emplace_back(resources.get(data.uv), RenderPassBeforeAccessType::Clear, RenderPassAfterAccessType::Preserve);
					renderPass.depth = { resources.get(data.depth), RenderPassBeforeAccessType::Preserve, RenderPassAfterAccessType::Preserve };

					cmdList->beginRenderPass(renderPass);

					MaterialBlockSet blocks(m_materialRenderer);
					feature->render(cmdList,blocks,"Opaque");

					cmdList->endRenderPass();

					cmdList->popMarker();
				}
			}
		);

		return true;
	}

	//--------------------------------

	MaskedRenderer::MaskedRenderer(RenderView& view, MaterialRenderer& material)
		: m_view(view)
		, m_materialRenderer(material)
	{

	}

	bool MaskedRenderer::render(FG& fg, FGBlackboard& blackboard)const {

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
			[=](const GBufferData& data, FGResources& resources, Ref<rhi::CommandList>& cmdList) {
				if (auto feature = m_view.findFeature<MaterialRenderFeature>()) {

					cmdList->pushMarker("Masked");

					RenderPassDesc renderPass;
					renderPass.colors.emplace_back(resources.get(data.albedo), RenderPassBeforeAccessType::Preserve, RenderPassAfterAccessType::Preserve);
					renderPass.colors.emplace_back(resources.get(data.normal), RenderPassBeforeAccessType::Preserve, RenderPassAfterAccessType::Preserve);
					renderPass.depth = { resources.get(data.depth), RenderPassBeforeAccessType::Preserve, RenderPassAfterAccessType::Preserve };

					cmdList->beginRenderPass(renderPass);

					MaterialBlockSet blocks(m_materialRenderer);
					feature->render(cmdList,blocks,"Masked");

					cmdList->endRenderPass();

					cmdList->popMarker();
				}
			}
		);

		return true;
	}

	//----

	DefferedLightRenderer::DefferedLightRenderer(RenderView& view, MaterialRenderer& material)
		: m_view(view)
		, m_materialRenderer(material)
	{
		m_material = [&] {

			auto code = File::ReadAllText("Assets/Shader/DeferredLight.hlsl");
			OB_ASSERT(code, "ファイル読み込み失敗");

			MaterialDesc desc;
			desc.name = "DeferredLight";
			desc.textures= { "Main" ,"Normal","Depth" ,"UV" };
			desc.integers = { "GBuffer" };

			MaterialPass& pass = desc.passes["PostProcess"];

			auto& shaders = pass.qualities.emplace_back();

			shaders.colors = { TextureFormat::RGBA8 };
			shaders.vs = Shader::CompileVS(code.value());
			shaders.ps = Shader::CompilePS(code.value());
			shaders.inputLayout = {
				{Semantic::Position,ElementType::Float,4},
			};

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
			[=](const Data& data, FGResources& resources, Ref<rhi::CommandList>& cmdList) {

				cmdList->pushMarker("Deffered Lighting");

				auto albedo = resources.get(data.albedo);
				auto normal = resources.get(data.normal);
				auto depth = resources.get(data.depth);
				auto uv = resources.get(data.uv);
				m_material->setTexture("Main", albedo);
				m_material->setTexture("Normal", normal);
				m_material->setTexture("Depth", depth);
				m_material->setTexture("UV", uv);

				RenderPassDesc renderPass;
				renderPass.colors.emplace_back(resources.get(data.accumulate), RenderPassBeforeAccessType::Clear, RenderPassAfterAccessType::Preserve);

				cmdList->beginRenderPass(renderPass);

				MaterialBlockSet blocks(m_materialRenderer);
				m_material->record(cmdList, blocks, m_mesh, 0, "PostProcess");

				cmdList->endRenderPass();

				cmdList->popMarker();

			}
		);

		accumulate = data.accumulate;

		return true;
	}

	void DefferedLightRenderer::setDebugMode(s32 mode) {
		m_material->setInteger("GBuffer", mode);
	}
}