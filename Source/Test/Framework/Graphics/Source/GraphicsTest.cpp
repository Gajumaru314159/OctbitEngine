//***********************************************************
//! @file
//! @brief		Box のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Platform/Window.h>
#include <Framework/Core/Utility/DI.h>
#include <Framework/RHI/All.h>
#include <Plugins/DirectX12RHI/System.h>
#include <Windows.h>

#include <Framework/Graphics/Graphics.h>
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Feature/ImGuiRenderFeature.h>

using namespace ob;
using namespace ob::graphics;


class SampleRenderPipeline : public RenderPipeline {
public:

	//@―---------------------------------------------------------------------------
	//! @brief      描画
	//@―---------------------------------------------------------------------------
	void render(FG& fg, const Array<Ref<RenderView>>& views) {

		// RenderFeature更新
		// 例：Shadow/GI

		for (auto& view : views) {

			FGTexture::Desc desc;
			FGTexture target{ view->getRenderTarget() };

			auto resource = fg.import("Target",desc,std::move(target));

			render<ImGuiStep>(view,fg, resource);
		}

	}

	template<class T,class... Args>
	void render(const Ref<RenderView>& view,Args&&... args) {
		if (auto step = view->findStep<T>()) {
			step->render(std::forward(args)...);
		} else {

		}
	}

};

TEST(Graphis, Create) {

	using namespace ob::rhi;
	using namespace ob::graphics;

	ServiceInjector injector;
	rhi::dx12::Register(injector);
	rhi::Register(injector);
	graphics::Register(injector);

	ServiceContainer container;
	injector.create<Graphics>(container);



	Ref<RenderScene> scene = [&]{
		RenderSceneDesc desc;
		desc.name = "Test";
		desc.features.add<ImGuiRenderFeature>();
		desc.pipelines.add<SampleRenderPipeline>();
		return RenderScene::Create(desc);
	}();

	Ref<RenderView> view = [&] {
		RenderViewDesc desc;
		desc.name = "MainCamera";
		desc.size = { 1280,720 };
		desc.type = RenderViewType::Camera;
		desc.window = platform::Window::Main();
		return scene->createView(desc);
	}();
	view->getRenderTarget(window);


	for (s32 i = 0; i < 100; ++i) {

		if (auto graphics = container.get<Graphics>()) {
			graphics->update();
		}

	}

}