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
#ifdef OS_WINDOWS
#include <Framework/DirectX12RHI/DirectX12RHIConfig.h>
#include <Framework/DirectX12RHI/System.h>
#endif
#include <Framework/VulkanRHI/System.h>
#include <Framework/VulkanRHI/VulkanRHIConfig.h>
#include <Framework/Core/Profile/Profile.h>

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

	std::filesystem::current_path("Sample/Sandbox");



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
					auto& deferredData = view.get<DeferredData>();
					if (ImGui::CollapsingHeader(data.name.c_str())) {
						static int gbuffer = 0;
						if (ImGui::SliderInt("GBuffer", &gbuffer, 0, 5))
						{
							deferredData.material->setInteger("GBuffer",gbuffer);
						}
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

		OB_PROFILE_FRAME();

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
#ifdef OS_WINDOWS
rhi::DirectX12RHIConfig dx12Config;
#endif
VulkanRHIConfig vkconfig;

void OctbitInit(ServiceInjector& injector) {
#ifdef OS_WINDOWS
	RegisterDirectX12RHIService(injector);
#else
	RegisterVulkanRHIService(injector);
#endif
	RegisterInputService(injector);
	RegisterGraphicsService(injector);

	config.enableBindless = true;
	//config.enableDebugLayer = true;
	//config.breakWithWarning = true;

#ifdef OS_WINDOWS
	dx12Config.enableDirectStorage = true;
	dx12Config.enableDebugLayer = true;
	dx12Config.breakWithWarning = true;
	dx12Config.enablePIX = true;
#endif
	vkconfig.enableDebugLayer = true;

	injector.bind(config);

#ifdef OS_WINDOWS
	injector.bind(dx12Config);
#endif
	injector.bind(vkconfig);

}


int OctbitMain(int argc, char *argv[])
{
	ob::core::Logger logger;

	LOG_INFO("OctbitInit()");

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

	// 初期化
	ob::core::ServiceInjector engineInjector;
	OctbitInit(engineInjector);

	// エンジン生成
	ob::engine::Engine engine(engineInjector);

	TestDirectX12();

	return 0;
}