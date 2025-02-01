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

#include <Framework/Editor/Editor.h>
#include <Framework/Core/Misc/TimeSpan.h>
#include <Framework/Platform/Arguments.h>

//-----------------------------------------------------------------
using namespace ob;
using namespace ob::rhi;
using namespace ob::engine;
using namespace ob::graphics;
using namespace ob::platform;

#if 0

class Protocol {
public:
	virtual void onReceived() {

	}
};

void OctbitInit(ServiceInjector& injector) {

}

int OctbitMain() {
	//ob::core::Logger log;
	//
	//LOG_INFO("Startup");
	//
	//Network::Startup();
	//{
	//
	//	LOG_INFO("Editor生成");
	//	Editor editor;
	//
	//	LOG_INFO("接続待機");
	//	while (true) {
	//
	//		if (System::Update() == false)break;
	//
	//		editor.update();
	//
	//	}
	//}
	//
	//Network::Shutdown();
	return 0;
}
#endif


#if 1

OB_DEFINE_CLASS_INFO(Vector<String>) {
	constructor();
	tag("DisplayName", "Vector<int>");
	method("size", &T::size);
	method("max_size", &T::max_size);
	method<void, size_t>("resize", &T::resize, "size");
	method("capacity", &T::capacity);
	method("empty", &T::empty);
	method("reserve", &T::reserve, "n");
	method("shrink_to_fit", &T::shrink_to_fit);

	method<String&, size_t>("at", &T::at, "n");

	method<void, size_t, const String&>("assign", &T::assign, "n", "t");
	method<void, const String&>("push_back", &T::push_back, "x");
	method("pop_back", &T::pop_back);
}

struct TransformA {
	Transform transform;
	bool changed = false;
};

int TestDirectX12() {

	TypeRegister::Link<ReflectionTestComponent>();



	Network::Startup();

	ob::editor::Editor editor;

	ob::core::ThreadPool threadPool;
	ob::core::TypeInfoManager typeInfoManager;
	ob::core::Logger log;
	ob::debug::Profiler profiler;
	ob::debug::LogInfo loginfo;
	ob::debug::FrameGraphDebugger fgdebugger;
	ob::debug::ReflectionExplorer reflectionExplorer;
	ob::debug::Outliner outliner;

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

	// ディスプレイ生成
	Ref<Display> display = [&] {
		DisplayDesc desc;
		desc.name = "MainDisplay";
		desc.window = platform::Window::Main();
		return Display::Create(desc);
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
	entity->addComponent<MeshComponent>()->setModel("Assets/Model/Ukulele.obj");
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
	auto skyTexture = Texture::Load("Assets/Texture/sky.dds");

	// メッシュ読み込み
	Ref<Mesh> skyMesh = Mesh::Load("Assets/Model/sky.obj");

	// 描画物生成
	Ref<Material> material = [&] {

		auto code = File::ReadAllText("Assets/Shader/GraphicTest.hlsl");
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
		editor.update();
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

	rhi::dx12::RegisterDirectX12RHIService(injector);
	input::RegisterInputService(injector);
	graphics::RegisterGraphicsService(injector);

	rhi::RHIConfig config;
	//config.enablePIX = true;
	//config.breakWithWarning = true;
	injector.bind(config);

}

int OctbitMain() {
	TestDirectX12();
	//TestVulkan();
	return 0;
}

#endif