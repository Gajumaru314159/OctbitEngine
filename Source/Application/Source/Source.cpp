#include <Framework/Core/Utility/DI.h>
#include <Framework/Graphics/All.h>
#include <Framework/Graphics/Builtin/RenderPipeline/TestRenderPipeline.h>
#include <Framework/RHI/All.h>
#include <Framework/Engine/All.h>
#include <Framework/Input/All.h>
#include <Framework/Platform/System.h>
#include <Framework/Platform/Window.h>
#include <Plugins/DirectX12RHI/System.h>

#include <Framework/Debug/LogInfo.h>
#include <Framework/Debug/Profiler.h>
#include <Framework/Debug/ReflectionExplorer.h>
#include <Framework/Debug/FrameGraphDebugger.h>
#include <Framework/Debug/Outliner.h>

#include <Framework/Graphics/Material/Material.h>


#include <Framework/Core/String/FixedString.h>

#include <Framework/Core/Reflection/TypeInfoManager.h>
#include <Framework/Engine/Reflection.h>

#include <Framework/Engine/Component/ReflectionTestComponent.h>
#include <Framework/Engine/Component/MeshComponent.h>
#include <Framework/Engine/Component/FlyCameraComponent.h>

#include <Framework/Core/Thread/ThreadPool.h>

#include <Framework/Core/Network/Network.h>
#include <Framework/Core/Network/TCPServer.h>

//-----------------------------------------------------------------
using namespace ob;
using namespace ob::rhi;
using namespace ob::engine;
using namespace ob::graphics;
using namespace ob::platform;

int TestDirectX12() {

	Network::Startup();

	ob::core::ThreadPool threadPool;
	ob::core::TypeInfoManager typeInfoManager;
	ob::core::Logger log;
	ob::debug::Profiler profiler;
	ob::debug::LogInfo loginfo;
	ob::debug::FrameGraphDebugger fgdebugger;
	ob::debug::ReflectionExplorer reflectionExplorer;
	ob::debug::Outliner outliner;

	System::Setup();

	TCPClient client;
	if (client.connect(IPAddress::LocalHost(), 5000)) {
		char message[] = "Connected";
		client.send(message, sizeof(message));
		LOG_INFO("送信済み");
	}



	// ディスプレイ生成
	Ref<Display> display = [&] {
		DisplayDesc desc;
		desc.name = "MainDisplay";
		desc.window = platform::Window::Main();
		return Display::Create(desc);
	}();

	// 描画オブジェクト生成
	RenderScene scene;
	RenderView view(scene, "Test");
	scene.addFeature<ImGuiRenderFeature>(scene);
	scene.addFeature<MaterialRenderFeature>();
	view.setDisplay(display);
	view.setPipeline<TestRenderPipeline>(view);

	// シーン生成
	auto world = World::Create("MainWorld");
	auto scene2 = Scene::Create("SubScene");
	auto entity = Entity::Create("RootEntity");
	// entity->addComponent<TransformComponent>();
	entity->setActive(true);
	entity->addComponent<ReflectionTestComponent>();
	entity->addComponent<MeshComponent>()->setModel("Asset/Model/Ukulele.obj");
	scene2->addEntity(entity);

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

	ImGuiRenderFeature::AddTask(
		scene, handle,
		[&] {
			// profiler.draw();
			loginfo.draw();
			// fgdebugger.draw();
			// ImGui::ShowDemoWindow();
		}
	);
	ImGuiRenderFeature::AddTask(
		scene, handle3,
		[&] {
			reflectionExplorer.draw();
			outliner.draw(*world);
		}
	);



	// テクスチャ読み込み
	auto skyTexture = Texture::Load("Asset/Texture/sky.dds");

	// メッシュ読み込み
	Ref<Mesh> skyMesh = Mesh::Load("Asset/Model/sky.obj");

	// 描画物生成
	Ref<Material> material = [&] {

		auto code = File::ReadAllText("Asset/Shader/GraphicTest.hlsl");
		OB_ASSERT(code, "ファイル読み込み失敗");

		MaterialDesc desc;
		desc.name = "Default";
		desc.matrixProperties = { "Matrix" };
		desc.colorProperties = { "Color" };
		desc.textureProperties = { "Main" };

		MaterialPass& opaque = desc.passes["Opaque"];
		opaque.depthStencil.depth.enable = true;
		opaque.colors = { TextureFormat::RGBA8 ,TextureFormat::RGBA8 ,TextureFormat::RGBA8 };	// Shaderに情報を持たせたい
		opaque.depth = TextureFormat::D32;
		opaque.vs = Shader::CompileVS(code.value());
		opaque.ps = Shader::CompilePS(code.value());
		opaque.requiredLayout = {
			{Semantic::Position,ElementType::Float,4},
			{Semantic::Normal,ElementType::Float,4},
			{Semantic::TexCoord,ElementType::Float,2},
		};

		return Material::Create(desc);
	}();
	material->setMatrix("Matrix", Matrix::Scale(Vec3(100)));
	material->setTexture("Main", skyTexture);
	material->setColor("Color", Color::White);


	// モデル登録
	if (auto feature = scene.findFeature<MaterialRenderFeature>()) {
		feature->addRenderable(skyMesh, material);
	}


	auto viewMtx =
		Matrix::Perspective(60, display->getDesc().size, 0.01f, 10000.0f) *
		Matrix::TRS(Vec3(0,0,-10), Rot::Identity, Vec3::One).inverse();
	graphics::Material::SetGlobalMatrix("Matrix", viewMtx);

	auto now = DateTime::Now();
	while (true) {

		if (System::Update() == false)break;

		// 行列更新
		auto t = TimeSpan(now, DateTime::Now()).totalSecondsF();
		auto mtx = Matrix::TRS(Vec3::Zero, Quat(0, t * 30.0f, 70), Vec3::One);

		flyCamera->update();

		Engine::Get()->update();
		display->update();
		Graphics::Get()->update();

		fgdebugger.update();

	}

	handle.remove();
	handle2.remove();
	handle3.remove();

	delete scene2;
	delete world;

	Engine::Get()->update();
	display->update();
	Graphics::Get()->update();


	Network::Shutdown();
	return 0;
}

void OctbitInit(ServiceInjector& injector) {

	rhi::dx12::Register(injector);
	input::Register(injector);
	graphics::Register(injector);

	rhi::Config config;
	//config.enablePIX = true;
	//config.breakWithWarning = true;
	injector.bind(config);

}

int OctbitMain() {
	TestDirectX12();
	//TestVulkan();
	return 0;
}