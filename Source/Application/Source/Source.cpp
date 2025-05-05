#include <Framework/Core/Misc/TimeSpan.h>
#include <Framework/Core/Network/Network.h>
#include <Framework/Core/Network/TCPServer.h>
#include <Framework/Core/Reflection/TypeInfoManager.h>
#include <Framework/Core/String/FixedString.h>
#include <Framework/Core/Thread/ThreadPool.h>
#include <Framework/Core/Utility/DI.h>
#include <Framework/Debug/FrameGraphDebugger.h>
#include <Framework/Debug/LogInfo.h>
#include <Framework/Debug/Outliner.h>
#include <Framework/Debug/Profiler.h>
#include <Framework/Debug/ReflectionExplorer.h>
#include <Framework/Editor/Editor.h>
#include <Framework/Engine/All.h>
#include <Framework/Engine/Component/FlyCameraComponent.h>
#include <Framework/Engine/Component/MeshComponent.h>
#include <Framework/Engine/Component/ReflectionTestComponent.h>
#include <Framework/Engine/Reflection.h>
#include <Framework/Graphics/All.h>
#include <Framework/Graphics/Builtin/RenderPipeline/TestRenderPipeline.h>
#include <Framework/Graphics/Material/Material.h>
#include <Framework/Input/All.h>
#include <Framework/Platform/Arguments.h>
#include <Framework/Platform/System.h>
#include <Framework/Platform/Window.h>
#include <Framework/RHI/All.h>
#include <Plugins/DirectX12RHI/DirectX12RHIConfig.h>
#include <Plugins/DirectX12RHI/System.h>
#include <Framework/Graphics/Material/MaterialSystem.h>

//-----------------------------------------------------------------
using namespace ob;
using namespace ob::debug;
using namespace ob::rhi;
using namespace ob::engine;
using namespace ob::graphics;
using namespace ob::platform;
using namespace ob::rhi;
using namespace ob::input;

int TestDirectX12() {

	TypeRegister::Link<ReflectionTestComponent>();
	Network::Startup();

	ob::editor::Editor editor;

	ThreadPool threadPool;
	TypeInfoManager typeInfoManager;
	Logger log;
	Profiler profiler;
	LogInfo loginfo;
	FrameGraphDebugger fgdebugger;
	ReflectionExplorer reflectionExplorer;
	Outliner outliner;

	Logger::EventHandle hLog;
	log.addEvent(hLog,
		[&](const Log& log){
			ob::editor::LogNotice notice;
			notice.level = log.level;
			notice.message = log.message;
			editor.send(notice);
		}
	);

	System::Setup();

	std::filesystem::current_path("Projects/SampleProject");



	// ディスプレイ生成
	Ref<SwapChain> swapChain = [&] {
		SwapChainDesc desc;
		desc.name = "MainDisplay";
		desc.window = platform::Window::Main();
		return SwapChain::Create(desc);
	}();

	{
		bool isEditor = false;
		Arguments args;
		for (auto& arg : args) {
			if (arg == "--editor") {
				isEditor = true;
			}
		}
		if (isEditor == false) {
			Window::Main().show();
		}
	}

	MaterialPropertiesSetDesc props;
	props.merge(CameraRenderFeature::GetProperties());
	props.merge(LightRenderFeature::GetProperties());
	MaterialSystemDesc materialSystemDesc;
	materialSystemDesc.properties = props;

	MaterialSystem materialSystem(materialSystemDesc);



	// 描画オブジェクト生成
	RenderScene scene;
	RenderView view(scene, "Test");
	scene.addFeature<ImGuiRenderFeature>(scene);
	auto& materialRT = scene.addFeature<MaterialRenderFeature>();
	scene.addFeature<CameraRenderFeature>(materialRT);
	scene.addFeature<LightRenderFeature>(materialRT);
	view.setDisplay(swapChain);
	view.setPipeline<TestRenderPipeline>(view);

	// シーン生成
	auto world = World::Create("MainWorld");
	auto scene2 = Scene::Create("SubScene");
	auto entity1 = Entity::Create("RootEntity");
	entity1->setActive(true);
	entity1->addComponent<ReflectionTestComponent>();
	entity1->addComponent<MeshComponent>()->setModel("Assets/Model/Ukulele.obj");
	scene2->addEntity(entity1);
	auto entity2 = Entity::Create("RootEntity");
	entity2->setActive(true);
	entity2->addComponent<ReflectionTestComponent>();
	entity2->addComponent<MeshComponent>()->setModel("Assets/Model/sky.obj");
	entity2->findComponent<TransformComponent>()->setLocalScale({ 10 ,10,10});
	scene2->addEntity(entity2);

	world->getRootScene().addSubScene(*scene2);


	auto camera = Entity::Create("FlyCamera");
	if (auto transform = camera->addComponent<TransformComponent>()) {
		transform->setWorldPosition({ 0,0,-10 });
	}
	auto flyCamera = camera->addComponent<FlyCameraComponent>();
	scene2->addEntity(camera);


	// デバッグ描画タスク追加
	ImGuiHandle handle;
	ImGuiHandle handle2;
	ImGuiHandle handle3;

	s32 debugMode = 0;

	ImGuiRenderFeature::AddTask(
		scene, handle,
		[&] {
			// profiler.draw();
			loginfo.draw();
			fgdebugger.draw();
			// ImGui::ShowDemoWindow();
		}
	);
	ImGuiRenderFeature::AddTask(
		scene, handle3,
		[&] {
			//reflectionExplorer.draw();
			outliner.draw(*world);

			if (ImGui::Begin("Debug")) {
				if (auto pipeline = view.getPipeline<TestRenderPipeline>()) {
					if (ImGui::SliderInt("Mode", &debugMode, 0, 3)) {
						pipeline->setDebugMode(debugMode);
					}
				}

			}

			ImGui::End();
		}
	);

	auto viewMtx =
		Matrix::Perspective(60, swapChain->getDesc().size, 0.01f, 10000.0f) *
		Matrix::TRS(Vec3(0,0,-10), Rot::Identity, Vec3::One).inverse();
	graphics::Material::SetGlobalMatrix("MatrixTest", viewMtx);

	auto now = DateTime::Now();
	while (true) {

		if (System::Update() == false)break;

		// 行列更新
		auto t = TimeSpan(now, DateTime::Now()).totalSecondsF();
		auto mtx = Matrix::TRS(Vec3::Zero, Quat(0, t * 30.0f, 70), Vec3::One);

		flyCamera->update();

		Engine::Get()->update();
		swapChain->update();
		Graphics::Get()->update();

		fgdebugger.update();
		editor.update();
	}

	handle.remove();
	handle2.remove();
	handle3.remove();

	delete scene2;
	delete world;

	Engine::Get()->update();
	swapChain->update();
	Graphics::Get()->update();


	Network::Shutdown();
	return 0;
}


rhi::RHIConfig config;
rhi::DirectX12RHIConfig dx12Config;

void OctbitInit(ServiceInjector& injector) {

	RegisterDirectX12RHIService(injector);
	RegisterInputService(injector);
	RegisterGraphicsService(injector);

	config.enableBindless = true;
	//config.enableDebugLayer = true;
	//config.breakWithWarning = true;

	dx12Config.enableDirectStorage = true;
	dx12Config.enableDebugLayer = true;
	dx12Config.breakWithWarning = true;
	dx12Config.enablePIX = true;

	injector.bind(config);
	injector.bind(dx12Config);

}

int OctbitMain() {
	TestDirectX12();
	return 0;
}
