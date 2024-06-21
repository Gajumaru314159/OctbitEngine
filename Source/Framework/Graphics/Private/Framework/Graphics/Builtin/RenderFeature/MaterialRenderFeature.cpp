//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Builtin/RenderFeature/MaterialRenderFeature.h>
#include <Framework/Graphics/FrameGraph/FG.h>
#include <Framework/RHI/CommandList.h>
#include <Framework/Graphics/Material/Material.h>
#include <Framework/Graphics/Mesh/Mesh.h>

namespace ob::graphics {


	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ
	//@―---------------------------------------------------------------------------
	MaterialRenderFeature::MaterialRenderFeature() {

	}
	void MaterialRenderFeature::addRenderable(const Ref<Mesh>& mesh, const Ref<Material>& material) {
		for (auto [i,submesh] : Indexed(mesh->getSubMeshes())) {
			Renderable renderable;
			renderable.mesh = mesh;
			renderable.submesh = i;
			renderable.material = material;

			for (auto [name,pass] : material->getDesc().passes) {
				m_renderablesMap[name].emplace_back(renderable);
			}
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画
	//@―---------------------------------------------------------------------------
	bool MaterialRenderFeature::render(FG& fg, RenderView& view,String pass, FGTexture& targets)const {

		auto itr = m_renderablesMap.find(pass);
		if (itr == m_renderablesMap.end())return false;

		auto& renderables = itr->second;

		struct Data {
			Array<FGTexture> targets;
			FGTexture depth;
			IntRect rect;
		};


		auto& data = fg.addPass<Data>(
			pass,
			[&](FGBuilder& builder, Data& data) {

				FGTextureInstance::Desc depthDesc;
				depthDesc.format = TextureFormat::D32;
				depthDesc.name = "Depth";
				depthDesc.size = view.getRenderSize();

				data.targets.push_back(builder.write(targets));
				
				data.depth = builder.write(builder.create(depthDesc));
				data.rect = view.getScaledRect();
			},
			// 仮
			[=](const Data& data, FGResources& resources, rhi::CommandList& cmdList) {

				using namespace ob::rhi;

				s32 targetNum = data.targets.size();
				StaticArray<Ref<rhi::RenderTexture>, 8> targets;
				for (auto& [i, resource] : Indexed(data.targets)) {
					targets[i] = resources.get(data.targets[i]);
				}

				cmdList.pushMarker(pass);

				Viewport vp(data.rect.left, data.rect.top, data.rect.right, data.rect.bottom, 1, 0);
				auto texture = resources.get(data.targets.front());
				auto depth = resources.get(data.depth);
				cmdList.setRenderTarget(texture,depth);
				cmdList.clearColors(-1);
				cmdList.clearDepthStencil();
				cmdList.setViewport(&vp, 1);
				cmdList.setScissorRect(&data.rect, 1);

				for (auto& renderable : renderables) {
					Matrix matrix;
					Ref<rhi::CommandList> cmdList2 = &cmdList;
					renderable.material->record(cmdList2, matrix, renderable.mesh, renderable.submesh, pass);
				}

				cmdList.popMarker();
			}
		);

		targets = data.targets.front();
		return true;
	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画
	//@―---------------------------------------------------------------------------
	bool MaterialRenderFeature::render(StringView pass, rhi::CommandList& cmdList)const {

		auto itr = m_renderablesMap.find(pass);
		if (itr == m_renderablesMap.end())return false;

		auto& renderables = itr->second;

		for (auto& renderable : renderables) {
			Matrix matrix;
			Ref<rhi::CommandList> cmdList2 = &cmdList;
			renderable.material->record(cmdList2, matrix, renderable.mesh, renderable.submesh, pass);
		}

		cmdList.popMarker();

		return true;
	}

}