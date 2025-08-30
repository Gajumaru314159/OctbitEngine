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
#include <Framework/Graphics/Material/Material.h>
#include <Framework/Graphics/Material/MaterialSystem.h>
#include <Framework/Graphics/Render/Feature/CameraRenderFeature.h>
#include <Framework/Graphics/Render/Feature/LightRenderFeature.h>
#include <Framework/Graphics/Render/Pipeline/ImGuiRenderPipeline.h>
#include <Framework/Graphics/Render/Pipeline/UniversalRenderPipeline.h>
#include <Framework/Input/All.h>
#include <Framework/Platform/Arguments.h>
#include <Framework/Platform/System.h>
#include <Framework/Platform/Window.h>
#include <Framework/RHI/All.h>
#include <Plugins/DirectX12RHI/DirectX12RHIConfig.h>
#include <Plugins/DirectX12RHI/System.h>
#include <Plugins/VulkanRHI/System.h>
#include <Plugins/VulkanRHI/VulkanRHIConfig.h>

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

	struct Tools {
		ThreadPool threadPool;
		TypeInfoManager typeInfoManager;
		Profiler profiler;
		LogInfo loginfo;
		FrameGraphDebugger fgdebugger;
		ReflectionExplorer reflectionExplorer;
		Outliner outliner;
	} tools;

	Logger::EventHandle hLog;
	Logger::Get()->addEvent(hLog,
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
	Ref<SwapChain> swapChain = SwapChain::Create({ .name = "MainDisplay", .window = platform::Window::Main() });
	

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
	props.merge(PointLightRenderFeature::GetProperties());
	MaterialSystem materialSystem({ props });



	// シーン生成
	RenderScene rscene;
	rscene.setPipeline(0, UniversalRenderPipeline::Create());
	rscene.setPipeline(1, ImGuiRenderPipeline::Create());

	// ビュー生成
	RenderView view(rscene, "Test");
	view.get<OutputViewData>().swapchain = swapChain;


	// シーン生成
	auto world = World::Create("MainWorld");
	auto scene = Scene::Create("SubScene");
	world->getRootScene().addSubScene(*scene);

	auto entity1 = Entity::Create("Ukulele", scene);
	entity1->setActive(true);
	entity1->addComponent<ReflectionTestComponent>();
	entity1->addComponent<MeshComponent>()->setModel("Assets/Model/Ukulele.obj");

	auto entity2 = Entity::Create<MeshComponent>("Sky", scene);
	entity2->setActive(true);
	entity2->findComponent<MeshComponent>()->setModel("Assets/Model/sky.obj");
	entity2->findComponent<TransformComponent>()->setLocalScale({ 1000 ,1000,1000});

	auto camera = Entity::Create("FlyCamera");
	camera->addComponent<TransformComponent>()->setWorldPosition({ 0,0,-10 });
	auto flyCamera = camera->addComponent<FlyCameraComponent>();
	scene->addEntity(camera);


	// デバッグ描画タスク追加
	ImGuiHandle handle;

	ImGuiPass::AddTask(
		view, handle,
		[&] {
			tools.outliner.draw(*world);
			tools.loginfo.draw();
			tools.fgdebugger.draw();
			if (ImGui::Begin("RenderPipeline")) {
				rscene.visitView([&](RenderView& view) {
					auto& data = view.get<RenderViewData>();
					if (ImGui::CollapsingHeader(data.name.c_str())) {
						ImGui::InputInt("Pipeline", &data.pipeline, 0, 2);
						ImGui::InputInt("Priority", &data.priority);
					}
					});
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

		flyCamera->update();

		Engine::Get()->update();
		swapChain->update();
		Graphics::Get()->update();

		tools.fgdebugger.update();
		editor.update();
	}

	handle.remove();

	delete scene;
	delete world;

	Engine::Get()->update();
	swapChain->update();
	Graphics::Get()->update();


	Network::Shutdown();
	return 0;
}


rhi::RHIConfig config;
rhi::DirectX12RHIConfig dx12Config;
VulkanRHIConfig vkconfig;

void OctbitInit(ServiceInjector& injector) {

	RegisterDirectX12RHIService(injector);
	//RegisterVulkanRHIService(injector);
	RegisterInputService(injector);
	RegisterGraphicsService(injector);

	config.enableBindless = true;
	//config.enableDebugLayer = true;
	//config.breakWithWarning = true;

	dx12Config.enableDirectStorage = true;
	dx12Config.enableDebugLayer = true;
	dx12Config.breakWithWarning = true;
	dx12Config.enablePIX = true;
	vkconfig.enableDebugLayer = true;

	injector.bind(config);
	injector.bind(dx12Config);
	injector.bind(vkconfig);

}

int OctbitMain() {
	TestDirectX12();
	return 0;
}
