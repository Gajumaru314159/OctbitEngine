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
#include <Framework/Graphics/Builtin/RenderPipeline/TestRenderPipeline.h>

#include <Framework/Input/System.h>
#include <Framework/Input/InputManager.h>

#include <Framework/Debug/LogInfo.h>
#include <Framework/Debug/Profiler.h>

#include <Framework/RHI/Config.h>

using namespace ob;
using namespace ob::graphics;

TEST(Graphis, Create) {

	using namespace ob::rhi;
	using namespace ob::graphics;
	using namespace ob::platform;

	System::Setup();
	
	ServiceInjector injector;

	rhi::dx12::Register(injector);
	rhi::Register(injector);
	input::Register(injector);
	graphics::Register(injector);

	rhi::Config config;
	config.enablePIX = true;
	injector.bind(config);

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

	// 事前セットアップここまで

	RenderScene scene;
	scene.addFeature<ImGuiRenderFeature>(scene);
	RenderView view(scene,"Test");
	view.setDisplay(display);
	view.setPipeline<TestRenderPipeline>(view);

	ImGuiHandle handle;
	if(auto feature = scene.findFeature<ImGuiRenderFeature>()){
		feature->addTask(handle,
			[&] {
				profiler.update();
				loginfo.update();
			}
		);
	}


	Ref<RootSignature> signature;
	{
		RootSignatureDesc desc(
			{
				RootParameter::Range(DescriptorRangeType::CBV,1,0),		// バッファ
				RootParameter::Range(DescriptorRangeType::SRV,1,0),		// テクスチャ
				//RootParameter::Range(DescriptorRangeType::Sampler,1,0),	// サンプラー
				RootParameter::Range(DescriptorRangeType::CBV,1,1),		// バッファ
				RootParameter::Range(DescriptorRangeType::SRV,1,1),		// テクスチャ
				//RootParameter::Range(DescriptorRangeType::Sampler,1,1),	// サンプラー
			},
			{
				StaticSamplerDesc(SamplerDesc(TextureFillter::Point),0),
			}
			);
		desc.name = "Common";
		signature = RootSignature::Create(desc);
		OB_ASSERT_EXPR(signature);
	}

	for (s32 i = 0; i < 10000; ++i) {

		if (System::Update() == false)break;

		RHI::Get()->update();
		input::InputModule::Get()->update();

		display->update();

		if (auto graphics = container.get<Graphics>()) {
			graphics->update();
		}

	}

}