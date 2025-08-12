//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/FrameGraph/FG.h>
#include <Framework/Graphics/Material/Material.h>
#include <Framework/Graphics/Mesh/Mesh.h>
#include <Framework/Graphics/Render/Pass/DeferredPass.h>
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/RHI/CommandList.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/Shader.h>

using namespace ob::rhi;

namespace ob::graphics {

	struct DeferredData {
		Ref<Material> material;
	};

	DeferredPass::DeferredPass() {

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

	DeferredPass::Output DeferredPass::render(FG& fg, RenderView& view, const Input& input)const {

		using namespace ob::rhi;

		auto& data = view.get<DeferredData>();
		if (!data.material) {
			data.material = [&] {

				auto code = File::ReadAllText("Assets/Shader/DeferredLight.hlsl");
				OB_ASSERT(code, "ファイル読み込み失敗");

				MaterialDesc desc;
				desc.name = "DeferredLight";
				desc.textures = { "Main" ,"Normal","Depth" };
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
		}

		return fg.addPass<Output>(
			"DeferredPass",
			[&](FGBuilder& builder, Output& output) {
				output.albedo = builder.read(input.albedo);
				output.normal = builder.read(input.normal);
				output.depth = builder.read(input.depth);

				rhi::RenderTextureDesc desc = fg.getTextureDesc(input.albedo);
				{
					desc.name = "Color";
					desc.format = rhi::TextureFormat::RGBA8;
					desc.clear.color = Color::Black;
					output.color = builder.write(builder.create(desc));
				}
			},
			[&](const Output& output, FGResources& resources, Ref<rhi::CommandList>& cmdList) {

				cmdList->pushMarker("Deffered Lighting");

				auto albedo = resources.getTexture(output.albedo);
				auto normal = resources.getTexture(output.normal);
				auto depth = resources.getTexture(output.depth);
				data.material->setTexture("Main", albedo);
				data.material->setTexture("Normal", normal);
				data.material->setTexture("Depth", depth);

				BeginPassParam param;
				param.colors.emplace_back(resources.getTexture(output.color), RenderPassBeforeAccessType::Clear, RenderPassAfterAccessType::Preserve);

				cmdList->beginRenderPass(param);

				MaterialBlockSet blocks(view);
				data.material->record(cmdList, blocks, m_mesh, 0, "PostProcess");

				cmdList->endRenderPass();

				cmdList->popMarker();

			}
		);

	}

}