//***********************************************************
//! @file
//! @brief		Box のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Platform/System.h>
#include <Framework/Platform/Window.h>
#include <Framework/Core/Utility/DI.h>
#include <Framework/RHI/All.h>
#include <Plugins/DirectX12RHI/System.h>
#include <Windows.h>

#include <Framework/Graphics/Graphics.h>
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/Graphics/Feature/ImGuiRenderFeature.h>

#include <Framework/Input/System.h>
#include <Framework/Input/InputManager.h>

#include <Framework/Debug/LogInfo.h>
#include <Framework/Debug/Profiler.h>

using namespace ob;
using namespace ob::graphics;


class SampleRenderPipeline : public RenderPipeline {
public:
	SampleRenderPipeline(RenderScene& scene)
		: RenderPipeline(scene)
	{

	}

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
			step->render(args...);
		} else {

		}
	}

};

TEST(Graphis, Create) {

	using namespace ob::rhi;
	using namespace ob::graphics;
	using namespace ob::platform;

	// EngineRootMarkがあるフォルダをカレントパスに変更
	auto path = std::filesystem::current_path();
	while (path.has_parent_path()) {
		auto rootMarkPath = path / "EngineRootMark";
		if (std::filesystem::exists(rootMarkPath)) {
			std::filesystem::current_path(path);
			ob::String t;
			ob::StringEncoder::Encode(path.u16string(), t);
			LOG_INFO("カレントパスを{}に設定", t);
			break;
		}
		path = path.parent_path();
	}
	
	ServiceInjector injector;
	{
		rhi::Config config;
		config.enablePIX = false;
		//config.breakWithWarning = true;
		injector.bind(config);
	}

	rhi::dx12::Register(injector);
	rhi::Register(injector);
	input::Register(injector);
	graphics::Register(injector);

	struct Dependency {
		Dependency(ob::graphics::Graphics&,ob::input::InputModule&){}
	};
	injector.bind<Dependency>();

	ServiceContainer container;
	injector.create<Dependency>(container);


	ob::debug::Profiler profiler;
	ob::debug::LogInfo loginfo;

	// ウィンドウ生成
	platform::WindowDesc windowDesc;
	windowDesc.title = "Graphic Test";
	platform::Window window(windowDesc);

	Ref<Display> display = [&] {
		DisplayDesc desc;
		desc.name = "MainDisplay";
		desc.window = window;
		return Display::Create(desc);
	}();

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
		return scene->createView(desc);
	}();
	view->setRenderTarget(display);



	ImGuiHandle handle;
	scene->findFeature<ImGuiRenderFeature>()->addTask(handle,
		[&] {
			profiler.update();
			loginfo.update();
		}
	);

	MSG msg = {};

	for (s32 i = 0; i < 10000; ++i) {

		if (System::Update() == false)break;

		RHI::Get()->update();
		input::InputModule::Get()->update();

		display->update();

		if (auto graphics = container.get<Graphics>()) {
			graphics->update();
		}

	}

	scene->requestDispose();

	if (auto graphics = container.get<Graphics>()) {
		graphics->update();
	}

}