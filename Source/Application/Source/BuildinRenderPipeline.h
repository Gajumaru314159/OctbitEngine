#pragma once
#include <Framework/Graphics/Render/RenderPipeline.h>
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/Graphics/FrameGraph/FG.h>

#include <Framework/RHI/CommandList.h>

using namespace ob;
using namespace ob::rhi;
using namespace ob::graphics;


//@―---------------------------------------------------------------------------
//! @brief      描画機能
//! @details    O3DEでいうところのFeatureProcessor。
//!				初期状態は非アクティブです。
//@―---------------------------------------------------------------------------
class TestRenderFeature : public RenderFeature {
public:

	OB_RTTI();


	//@―---------------------------------------------------------------------------
	//! @brief      描画
	//@―---------------------------------------------------------------------------
	void render(FG& fg, FrameGraphResource target,RenderViewId id) {

		struct ImGuiData {
			FrameGraphResource frameBuffer;
			FrameGraphResource target;
		};

		fg.addCallbackPass<ImGuiData>(
			"Test",
			[=](FrameGraph::Builder& builder, ImGuiData& data) {

				FGFrameBuffer::Desc desc;
				desc.name = TC("");
				desc.attachments = {
					// texture
				};
				desc.renderPass;


				builder.create<FGFrameBuffer>("",desc);
				data.target = builder.write(target);
			},
			[](const ImGuiData& data, FrameGraphPassResources& resources, void* ctx) {
				auto& cmd = *static_cast<rhi::CommandList*>(ctx);

				resources.get<FGTexture>(data.target).instance;

				cmd.beginRenderPass();
			}
		);
	}


protected:
	TestRenderFeature(RenderScene& scene) :RenderFeature(scene) {}
};




//@―---------------------------------------------------------------------------
//! @brief      描画機能
//! @details    O3DEでいうところのFeatureProcessor。
//!				初期状態は非アクティブです。
//@―---------------------------------------------------------------------------
class CameraRenderFeature : public RenderFeature {
public:

	OB_RTTI();

	void createView() {



	}

protected:
	CameraRenderFeature(RenderScene& scene) :RenderFeature(scene) {}
};

class BuiltinRenderPipeline : public RenderPipeline {
public:

	BuiltinRenderPipeline(RenderScene& scene)
		: RenderPipeline(scene)
	{
		
	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画
	//@―---------------------------------------------------------------------------
	void render(FG& fg, const Array<Ref<RenderView>>& views) override {

		for (auto& view : views) {

			auto viewId = view->getId();

			FGTexture::Desc desc;

			desc.name = TC("Target");
			// desc.size = 

			FrameGraphResource finalTarget = fg.import(TC("Target"),desc,resource);

			if (auto feature = findFeature<TestRenderFeature>()) {
				feature->render(fg, finalTarget, viewId);
			}

			view->getId();

		}

		// FrameGraphのPass毎に並列化する
		// そのために処理をコールバックに積む
		if (auto feature = findFeature<TestRenderFeature>()) {
			feature->render(fg,target);
		}

	};
private:

};