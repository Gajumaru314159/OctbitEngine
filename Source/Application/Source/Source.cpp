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

//-----------------------------------------------------------------
using namespace ob;
using namespace ob::rhi;
using namespace ob::engine;
using namespace ob::graphics;
using namespace ob::platform;


void drawOutliner(Scene* scene);
void drawComponents(Entity* pEntity = nullptr);

int TestDirectX12() {

	{
	FixedString<120> fstr;
	Char aaa[129];

	FormatTo(std::back_inserter(fstr), "test{}", 123);
	}

	ob::core::Logger log;

	TypeInfoManager typeInfoManager;

	ob::debug::Profiler profiler;
	ob::debug::LogInfo loginfo;
	ob::debug::FrameGraphDebugger fgdebugger;
	ob::debug::ReflectionExplorer reflectionExplorer(typeInfoManager);

	System::Setup();

	auto world = World::Create("MainWorld");
	auto scene2 = Scene::Create("SubScene");
	auto entity = Entity::Create("RootEntity");
	entity->addComponent<TransformComponent>();
	entity->addComponent<ReflectionTestComponent>();
	scene2->addEntity(entity);
	world->getRootScene().addSubScene(*scene2);

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
#pragma endregion
	// 事前セットアップここまで

	RenderScene scene;
	RenderView view(scene, "Test");
	scene.addFeature<ImGuiRenderFeature>(scene);
	scene.addFeature<MaterialRenderFeature>();
	view.setDisplay(display);
	view.setPipeline<TestRenderPipeline>(view);

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


	auto texture = Texture::Load("Asset/Model/Ukulele_col.dds");
	auto skyTexture = Texture::Load("Asset/Texture/sky.dds");
	Ref<Mesh> mesh = Mesh::Load("Asset/Model/Ukulele.obj");
	Ref<Mesh> skyMesh = Mesh::Load("Asset/Model/sky.obj");

	material->setMatrix("Matrix", Matrix::Identity);
	material->setTexture("Main", texture);

	skyMat->setMatrix("Matrix", Matrix::Identity);
	skyMat->setTexture("Main", skyTexture);

	if (auto feature = scene.findFeature<MaterialRenderFeature>()) {
		feature->addRenderable(mesh, material);
		feature->addRenderable(skyMesh, skyMat);
	}

	Vec3 pos(0, 0, -10);
	Rot rot = Rot::Identity;
	auto now = DateTime::Now();


	auto size = display->getDesc().size;
	auto viewMtx =
		Matrix::Perspective(60, size.width, size.height, 0.01f, 10000.0f) *
		Matrix::TRS(pos, rot, Vec3::One).inverse();
	graphics::Material::SetGlobalMatrix("Matrix", viewMtx);

	while (true) {

		if (System::Update() == false)break;

		RHI::Get()->update();
		input::InputModule::Get()->update();
		display->update();


		// 行列更新
		auto t = TimeSpan(now, DateTime::Now()).totalSecondsF();
		auto mtx = Matrix::TRS(Vec3::Zero, Quat(0, t * 30.0f, 70), Vec3::One);
		material->setMatrix("Matrix", mtx);
		material->setColor("Color", Color::White);


		if (auto graphics = Graphics::Get()) {
			graphics->update();
		}

		fgdebugger.update();

	}

	return 0;
}

void OctbitInit(ServiceInjector& injector) {

	rhi::dx12::Register(injector);
	input::Register(injector);
	graphics::Register(injector);

	rhi::Config config;
	config.enablePIX = true;
	config.breakWithWarning = true;
	injector.bind(config);

}

int OctbitMain() {
	TestDirectX12();
	//TestVulkan();
	return 0;
}