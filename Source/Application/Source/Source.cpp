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
#include <Plugins/DirectX12RHI/System.h>

//-----------------------------------------------------------------
using namespace ob;
using namespace ob::debug;
using namespace ob::rhi;
using namespace ob::engine;
using namespace ob::graphics;
using namespace ob::platform;
using namespace ob::rhi::dx12;
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
#if 1
	auto skyTexture = Texture::Load("Assets/Texture/test.dds");
#else

	auto skyTexture = Texture::Create(
		[] {
			TextureDesc desc;
			desc.type = TextureType::Texture2D;
			desc.format = TextureFormat::RGBA16;
			desc.size = { 4096,2048};
			desc.mipLevels = 5;
			return desc;
		}()
	);
#endif
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
		desc.textureProperties = { "Main", "Normal", "Parameter" };

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
	material->setTexture("Normal", Texture::Normal());
	material->setTexture("Parameter", Texture::White());
	material->setColor("Color", Color::White);


	{
		String src = "Assets/Texture/test2.dds";
		String dest = "Assets/Texture/test2.bin";
		GraphicFile::Generate(src, dest,1);

		GraphicFileQueueDesc desc;
		desc.name = "GraphicFileQueue";
		auto queue = GraphicFileQueue::Create(desc);

		auto infos = GraphicFile::Prepare(dest);

		s32 index = 1;
		
		GraphicFileRequest request;
		if(true){
			request.handle = GraphicFileHandle::Create(dest);
			request.offset = infos[index].offset;
			request.size = infos[index].size;
			request.uncompressedSize = infos[index].uncompressedSize;
			GraphicFileRequest::TextureDesc t;
			t.texture = skyTexture;
			t.subresourceIndex = index;

			request.dest = t;
		} else {
			request.handle = GraphicFileHandle::Create(dest);
			request.offset = infos[0].offset;
			for (auto& info : infos) {
				request.size += info.size;
			}
			GraphicFileRequest::TextureSequenceDesc t;
			t.texture = skyTexture;
			t.firstSubresourceIndex = 0;
			request.dest = t;
		}
		queue->add(request);

		auto event = GraphicFileEvent::Create();
		queue->add(event);
		queue->submit();

		event->wait();

		queue->validate();
	}


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

	RegisterDirectX12RHIService(injector);
	RegisterInputService(injector);
	RegisterGraphicsService(injector);

	rhi::RHIConfig config;
	config.enableDirectStorageDebug = true;
	//config.enableDebugLayer = true;
	//config.breakWithWarning = true;
	injector.bind(config);

}

int OctbitMain() {
	TestDirectX12();
	return 0;
}
