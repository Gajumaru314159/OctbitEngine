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

#include <Framework/Graphics/Material/Material.h>


#include <Framework/Core/String/FixedString.h>

#include <Framework/Core/Reflection/TypeInfoManager.h>
#include <Framework/Engine/Reflection.h>

#include <Framework/Engine/Component/ReflectionTestComponent.h>
#include <Framework/Engine/Component/MeshComponent.h>
#include <Framework/Engine/Component/FlyCameraComponent.h>

//-----------------------------------------------------------------
using namespace ob;
using namespace ob::rhi;
using namespace ob::engine;
using namespace ob::graphics;
using namespace ob::platform;


void drawOutliner(Scene* scene);
void drawComponents(Entity* pEntity = nullptr);

int TestDirectX12() {

	ob::core::TypeInfoManager typeInfoManager;
	ob::core::Logger log;
	ob::debug::Profiler profiler;
	ob::debug::LogInfo loginfo;
	ob::debug::FrameGraphDebugger fgdebugger;
	ob::debug::ReflectionExplorer reflectionExplorer;

	System::Setup();

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
		transform->setWorldPosition({0,0,-10});
	}
	auto flyCamera = camera->addComponent<FlyCameraComponent>();
	scene2->addEntity(camera);


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

	// デバッグ描画タスク追加
	ImGuiHandle handle;
	ImGuiHandle handle2;
	ImGuiHandle handle3;

	ImGuiRenderFeature::AddTask(
		scene, handle,
		[&] {
			profiler.draw();
			loginfo.draw();
			fgdebugger.draw();
			drawOutliner(&world->getRootScene());
			drawComponents();
		}
	);
	ImGuiRenderFeature::AddTask(
		scene, handle3,
		[&] {
			reflectionExplorer.draw();
		}
	);





	// テクスチャ読み込み
	auto texture = Texture::Load("Asset/Model/Ukulele_col.dds");
	auto skyTexture = Texture::Load("Asset/Texture/sky.dds");

	// メッシュ読み込み
	Ref<Mesh> mesh = Mesh::Load("Asset/Model/Ukulele.obj");
	Ref<Mesh> skyMesh = Mesh::Load("Asset/Model/sky.obj");

	// 描画物生成
	Ref<Material> material = [&] {

		auto code = ReadFile("Asset/Shader/GraphicTest.hlsl");
		OB_ASSERT(code, "ファイル読み込み失敗");

		MaterialDesc desc;
		desc.name = "Default";
		desc.matrixProperties = { "Matrix" };
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
	auto skyMat = Material::Create(material->getDesc());

	material->setMatrix("Matrix", Matrix::Identity);
	material->setTexture("Main", texture);

	skyMat->setMatrix("Matrix", Matrix::Scale(Vec3(1000)));
	skyMat->setTexture("Main", skyTexture);


	// モデル登録
	if (auto feature = scene.findFeature<MaterialRenderFeature>()) {
		// feature->addRenderable(mesh, material);
		feature->addRenderable(skyMesh, skyMat);
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
		material->setMatrix("Matrix", mtx);
		material->setColor("Color", Color::White);

		flyCamera->update();

		input::InputModule::Get()->update();
		RHI::Get()->update();
		display->update();
		Graphics::Get()->update();

		fgdebugger.update();

	}

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