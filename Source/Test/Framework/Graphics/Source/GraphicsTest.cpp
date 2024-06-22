//***********************************************************
//! @file
//! @brief		Box のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Utility/DI.h>
#include <Framework/Graphics/All.h>
#include <Framework/Graphics/Builtin/RenderPipeline/TestRenderPipeline.h>
#include <Framework/RHI/All.h>
#include <Framework/Input/All.h>
#include <Framework/Platform/System.h>
#include <Framework/Platform/Window.h>
#include <Plugins/DirectX12RHI/System.h>

#include <Framework/Debug/LogInfo.h>
#include <Framework/Debug/Profiler.h>
#include <Framework/Debug/FrameGraphDebugger.h>

#include <Framework/Graphics/Material/Material.h>

using namespace ob;
using namespace ob::graphics;

TEST(Graphis, Create) {
#pragma region
	using namespace ob::rhi;
	using namespace ob::graphics;
	using namespace ob::platform;

	System::Setup();
	
	ServiceInjector injector;
	ServiceContainer container;
	{
		rhi::dx12::Register(injector);
		input::Register(injector);
		graphics::Register(injector);

		rhi::Config config;
		config.enablePIX = true;
		config.breakWithWarning = true;
		injector.bind(config);

		struct Dependency {
			Dependency(ob::graphics::Graphics&, ob::input::InputModule&) {}
		};
		injector.bind<Dependency>();

		injector.create<Dependency>(container);
	}

	ob::debug::Profiler profiler;
	ob::debug::LogInfo loginfo;
	ob::debug::FrameGraphDebugger fgdebugger;

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
	RenderView view(scene,"Test");
	scene.addFeature<ImGuiRenderFeature>(scene);
	scene.addFeature<MaterialRenderFeature>();
	view.setDisplay(display);
	view.setPipeline<TestRenderPipeline>(view);

	ImGuiHandle handle;
	ImGuiHandle handle2;

	ImGuiRenderFeature::AddTask(
		scene, handle,
		[&] {
			profiler.draw();
			loginfo.draw();
			fgdebugger.draw();
		}
	);

	Ref<Material> material = [&]{

		auto code = ReadFile("Asset/Shader/GraphicTest.hlsl");
		OB_ASSERT(code,"ファイル読み込み失敗");

		MaterialDesc desc;
		desc.name = "Default";
		desc.colorProperties = { "Color" , "Color2"};
		desc.matrixProperties = { "Matrix" };
		desc.textureProperties = { "Main" };

		MaterialPass opaque;
		opaque.depthStencil.depth.enable = true;
		opaque.depthStencil.stencil.enable = false;
		opaque.colors = { TextureFormat::RGBA8 ,TextureFormat::RGBA8 };	// Shaderに情報を持たせたい
		opaque.depth = TextureFormat::D32;
		opaque.vs = Shader::CompileVS(code.value());
		opaque.ps = Shader::CompilePS(code.value());
		opaque.blends = { BlendDesc::AlphaBlend };
		opaque.requiredLayout = {
			{Semantic::Position,Type::Float,4},
			{Semantic::Normal,Type::Float,4},
			{Semantic::TexCoord,Type::Float,2},
		};

		desc.passes.emplace("Opaque",opaque);
		return Material::Create(desc);
	}();
	auto skyMat = Material::Create(material->getDesc());


	auto texture = Texture::Load("Asset/Model/Ukulele_col.dds");
	auto skyTexture = Texture::Load("Asset/Texture/sky.dds");
	Ref<Mesh> mesh = Mesh::Load("Asset/Model/Ukulele.obj");
	Ref<Mesh> skyMesh = Mesh::Load("Asset/Model/sky.obj");

	material->setColor("Color", Color::White);
	material->setMatrix("Matrix", Matrix::Identity);
	material->setTexture("Main", texture);

	skyMat->setColor("Color", Color::White);
	skyMat->setMatrix("Matrix", Matrix::Identity);
	skyMat->setTexture("Main", skyTexture);

	if (auto feature = scene.findFeature<MaterialRenderFeature>()) {
		feature->addRenderable(mesh, material);
		feature->addRenderable(skyMesh, skyMat);
	}

	Vec3 pos(0, 0, -10);
	Rot rot = Rot::Identity;
	f32 modelRotSpeed = 30.0f;
	auto now = DateTime::Now();

	for (s32 i = 0; i < 100000; ++i) {

		if (System::Update() == false)break;

		RHI::Get()->update();
		input::InputModule::Get()->update();

		display->update();



		// 入力更新
		const auto rspd = 90 / 60.f;
		Rot r2(rot.x, rot.y, 0);
		static auto speed = 4 / 60.f;
		auto t = TimeSpan(now, DateTime::Now()).totalSecondsF();
		// 行列更新
		auto size = display->getDesc().size;
		auto viewMtx =
			Matrix::Perspective(60, size.width,size.height, 0.01f, 10000.0f) * 
			Matrix::TRS(pos, rot, Vec3::One).inverse();
		auto ukuleleMtx = Matrix::TRS(Vec3::Zero, Quat(0, t*modelRotSpeed, 70), Vec3::One);

		graphics::Material::SetGlobalColor("LightDir", Color(1, 1, 1));
		graphics::Material::SetGlobalMatrix("Matrix", viewMtx);
		material->setMatrix("Matrix",ukuleleMtx);
		material->setColor("Color", Color::White);


		if (auto graphics = container.get<Graphics>()) {
			graphics->update();
		}

		fgdebugger.update();

	}

}