//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Builtin/Renderer/DefferedLightRenderer.h>
#include <Framework/Graphics/Builtin/RenderFeature/CameraRenderFeature.h>
#include <Framework/Graphics/Material/Material.h>
#include <Framework/Graphics/Mesh/Mesh.h>

using namespace ob::rhi;

namespace ob::graphics {

	EarlyZPass::EarlyZPass() {

	}

	EarlyZPass::Output EarlyZPass::render(FG& fg, RenderView& view, Input input)const {

		return fg.addPass<Output>(
			"EarlyZPass",
			[&](FGBuilder& builder, Output& output) {

				rhi::RenderTextureDesc desc;
				desc.size = view.get<CameraRFData>().output->size();
				{
					desc.name = "Albedo";
					desc.format = rhi::TextureFormat::RGBA8;
					desc.clear.color = Color::Black;
					output.albedo = builder.write(builder.create(desc));
				}
				{
					desc.name = "Normal";
					desc.format = rhi::TextureFormat::RGBA8;
					desc.clear.color = Color::Normal;
					output.normal = builder.write(builder.create(desc));
				}
				{
					desc.name = "Depth";
					desc.format = rhi::TextureFormat::D32;
					output.depth = builder.write(builder.create(desc));
				}
			},
			[&](const Output& output, FGResources& resources, Ref<rhi::CommandList>& cmdList) {
				if (auto feature = view.findFeature<MaterialRenderFeature>()) {

					MaterialBlockSet blocks(view);

					cmdList->pushMarker("EarlyZ");

					BeginPassParam param;
					param.colors.emplace_back(resources.getTexture(output.albedo), RenderPassBeforeAccessType::NoAccess, RenderPassAfterAccessType::NoAccess);
					param.depth = { resources.getTexture(output.depth), RenderPassBeforeAccessType::Clear, RenderPassAfterAccessType::Preserve };

					cmdList->beginRenderPass(param);

					feature->render(cmdList,blocks,"EarlyZ");

					cmdList->endRenderPass();

					cmdList->popMarker();
				}
			}
		);

	}

	//--------------------------------

	OpaquePass::OpaquePass() {

	}

	OpaquePass::Output OpaquePass::render(FG& fg, RenderView& view, Input input)const {

		return fg.addPass<Output>(
			"Opaque",
			[&](FGBuilder& builder, Output& output) {
				output.albedo = builder.write(input.albedo);
				output.normal = builder.write(input.normal);
				output.depth = builder.read(input.depth);
			},
			[&](const Output& output, FGResources& resources, Ref<rhi::CommandList>& cmdList) {
				if (auto feature = view.findFeature<MaterialRenderFeature>()) {

					cmdList->pushMarker("Opaque");

					BeginPassParam param;
					param.colors.emplace_back(resources.getTexture(output.albedo), RenderPassBeforeAccessType::Clear, RenderPassAfterAccessType::Preserve);
					param.colors.emplace_back(resources.getTexture(output.normal), RenderPassBeforeAccessType::Clear, RenderPassAfterAccessType::Preserve);
					param.depth = { resources.getTexture(output.depth), RenderPassBeforeAccessType::Preserve, RenderPassAfterAccessType::Preserve };

					cmdList->beginRenderPass(param);

					MaterialBlockSet blocks(view);
					feature->render(cmdList,blocks,"Opaque");

					cmdList->endRenderPass();

					cmdList->popMarker();
				}
			}
		);

	}

	//--------------------------------

	MaskedPass::MaskedPass() {

	}

	MaskedPass::Output MaskedPass::render(FG& fg, RenderView& view, Input input)const {

		return fg.addPass<Output>(
			"Masked",
			[&](FGBuilder& builder, Output& output) {
				output.albedo = builder.write(input.albedo);
				output.normal = builder.write(input.normal);
				output.depth = builder.read(input.depth);
			},
			[&](const Output& output, FGResources& resources, Ref<rhi::CommandList>& cmdList) {
				if (auto feature = view.findFeature<MaterialRenderFeature>()) {

					cmdList->pushMarker("Masked");

					BeginPassParam param;
					param.colors.emplace_back(resources.getTexture(output.albedo), RenderPassBeforeAccessType::Preserve, RenderPassAfterAccessType::Preserve);
					param.colors.emplace_back(resources.getTexture(output.normal), RenderPassBeforeAccessType::Preserve, RenderPassAfterAccessType::Preserve);
					param.depth = { resources.getTexture(output.depth), RenderPassBeforeAccessType::Preserve, RenderPassAfterAccessType::Preserve };

					cmdList->beginRenderPass(param);

					MaterialBlockSet blocks(view);
					feature->render(cmdList,blocks,"Masked");

					cmdList->endRenderPass();

					cmdList->popMarker();
				}
			}
		);

	}

	//----

	DeferredPass::DeferredPass() {
		m_material = [&] {

			auto code = File::ReadAllText("Assets/Shader/DeferredLight.hlsl");
			OB_ASSERT(code, "ファイル読み込み失敗");

			MaterialDesc desc;
			desc.name = "DeferredLight";
			desc.textures= { "Main" ,"Normal","Depth" };
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


	DeferredPass::Output DeferredPass::render(FG& fg, RenderView& view, Input input)const {

		return fg.addPass<Output>(
			"DeferredPass",
			[&](FGBuilder& builder, Output& output) {
				output.albedo = builder.read(input.albedo);
				output.normal = builder.read(input.normal);
				output.depth = builder.read(input.depth);

				rhi::RenderTextureDesc desc;
				desc.size = view.get<CameraRFData>().output->size();
				{
					desc.name = "Color";
					desc.format = rhi::TextureFormat::RGBA8;
					desc.clear.color = Color::Black;
					output.color= builder.write(builder.create(desc));
				}
			},
			[&](const Output& output, FGResources& resources, Ref<rhi::CommandList>& cmdList) {

				cmdList->pushMarker("Deffered Lighting");

				auto albedo = resources.getTexture(output.albedo);
				auto normal = resources.getTexture(output.normal);
				auto depth = resources.getTexture(output.depth);
				m_material->setTexture("Main", albedo);
				m_material->setTexture("Normal", normal);
				m_material->setTexture("Depth", depth);

				BeginPassParam param;
				param.colors.emplace_back(resources.getTexture(output.color), RenderPassBeforeAccessType::Clear, RenderPassAfterAccessType::Preserve);

				cmdList->beginRenderPass(param);

				MaterialBlockSet blocks(view);
				m_material->record(cmdList, blocks, m_mesh, 0, "PostProcess");

				cmdList->endRenderPass();

				cmdList->popMarker();

			}
		);

	}

}