#include <Windows.h>

#include <Framework/Core/Utility/DI.h>

#include <Framework/RHI/All.h>
#include <Framework/Input/All.h>
#include <Framework/Platform/Window.h>

#include <Framework/Engine/All.h>

#include <Framework/Graphics/Material.h>
#include <Framework/Graphics/Mesh.h>
#include <Framework/Engine/Component/CameraComponent.h>

#include <Framework/Debug/LogInfo.h>

#include <Plugins/ImGui/ImGui.h>

#include <Model.h>

#include <Framework/Platform/System.h>
#include <Framework/Input/System.h>
#include <Framework/Input/InputManager.h>
#include <Framework/Graphics/System.h>
#include <Framework/Graphics/GraphicModule.h>
#include <Plugins/DirectX12RHI/System.h>

//-----------------------------------------------------------------
using namespace ob;

void Link_Vulkan();
int TestDirectX12();
int TestVullkan();

void OctbitInit(ServiceInjector& injector) {

	input::
		Register(injector);
	platform::
		Register(injector);
	rhi::dx12::
		Register(injector);
	rhi::
		Register(injector);
	graphics::
		Register(injector);

}

int OctbitMain() {
	TestDirectX12();
	//TestVulkan();
	return 0;
}

void drawOutliner(const Ref<engine::Scene>& scene);
void drawComponents(engine::Entity* pEntity = nullptr);

int TestDirectX12() {

	using namespace ob::rhi;
	using namespace ob::graphics;

	debug::LogInfo logInfo;


	// ウィンドウ生成
	platform::WindowDesc windowDesc;
	windowDesc.title = TC("Graphic Test");
	platform::Window window(windowDesc);

	// ディスプレイ
	Ref<Display> display;
	{
		DisplayDesc desc;
		desc.name = TC("MainDisplay");
		desc.window = window;
		display = Display::Create(desc);
		OB_ASSERT_EXPR(display);
	}

	// レンダーパス
	Ref<RenderPass> renderPass;
	{
		RenderPassDescHelper desc;
		desc.name = TC("Forward");
		auto color = desc.addAttachment(TextureFormat::RGBA8);
		auto color2 = desc.addAttachment(TextureFormat::RGBA8);
		auto depth = desc.addAttachment(TextureFormat::D32);
		auto pass0 = desc.addSubpassXCD(TC("Opaque"), {color,color2}, depth);

		renderPass = Material::AddRenderPass(desc);
		OB_ASSERT_EXPR(renderPass);
	}

	// 描画先生成
	Ref<RenderTexture> colorRT;
	{
		RenderTextureDesc desc;
		desc.name = TC("Color0");
		desc.size = display->getDesc().size;
		desc.format = TextureFormat::RGBA8;
		desc.clear.color = Color::Black;
		desc.display = display;

		colorRT = RenderTexture::Create(desc);
		OB_ASSERT_EXPR(colorRT);
	}
	Ref<RenderTexture> color2RT;
	{
		RenderTextureDesc desc;
		desc.name = TC("Color1");
		desc.size = display->getDesc().size;
		desc.format = TextureFormat::RGBA8;
		desc.clear.color = Color::Green;
		desc.display = display;

		color2RT = RenderTexture::Create(desc);
		OB_ASSERT_EXPR(color2RT);
	}
	Ref<RenderTexture> depthRT;
	{
		RenderTextureDesc desc;
		desc.name = TC("Depth");
		desc.size = display->getDesc().size;
		desc.format = TextureFormat::D32;
		desc.clear.depth = 1.0f;
		desc.display = display;

		depthRT = RenderTexture::Create(desc);
		OB_ASSERT_EXPR(depthRT);
	}

	Ref<FrameBuffer> frameBuffer;
	{
		FrameBufferDesc desc;
		desc.name = TC("Test");
		desc.renderPass = renderPass;
		desc.attachments.push_back(colorRT);
		desc.attachments.push_back(color2RT);
		desc.attachments.push_back(depthRT);

		frameBuffer = FrameBuffer::Create(desc);
		OB_ASSERT_EXPR(frameBuffer);
	}

	Ref<CommandList> cmdList;
	{
		CommandListDesc desc;
		desc.name = TC("MainCommandList");
		desc.type = CommandListType::Graphic;
		cmdList = CommandList::Create(desc);
		OB_ASSERT_EXPR(cmdList);
	}

	Model sky("Asset/Model/sky.obj", TC("Asset/Texture/sky.dds"));
	Model ukulele("Asset/Model/Ukulele.obj", TC("Asset/Model/Ukulele_col.dds"));


	Ref<CommandBuffer> cmdBuf = CommandBuffer::Create(cmdList);

	// ImGui初期化
	ImGui::StartupImGui(window, renderPass);



	// シーン生成テスト
	auto world = engine::World::Create(TC("TestWorld"));
	auto scene = engine::Scene::Create(TC("SampleScene"));
	auto entity = engine::Entity::Create(TC("Parent"));
	auto child = engine::Entity::Create(TC("CHild"));
	entity->addChild(child);
	scene->addEntity(entity);

	world->getRootScene()->addSubScene(scene);

	if (entity && scene) {


		entity->addComponent<engine::TransformComponent>();
		//entity->addComponent<graphics::CameraComponent>();

	}

	Texture::White();
	//------ループ-----

	Vec3 pos(0, 0, -10);
	Rot rot = Rot::Identity;
	f32 modelRotSpeed = 1.0f;

	MSG msg = {};
	while (true) {

		// TODO アプリ更新内部にモジュール更新とメッセージ処理を隠ぺい
		{
			if (auto engine = engine::Engine::Get()) {
				engine->update();
				engine->get<input::InputModule>()->update();
				engine->get<rhi::RHI>()->update();
			}

			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (msg.message == WM_QUIT) {
				break;
			}
		}

		if (input::Keyboard::Escape.down())break;

		// 入力更新
		const auto rspd = 90 / 60.f;
		Rot r2(rot.x, rot.y, 0);
		static auto speed = 4 / 60.f;
		speed += input::Mouse::Wheel.value() * 0.0001f;

		pos += r2.front() * speed * (input::Keyboard::W.pressed() - input::Keyboard::S.pressed());
		pos += r2.right() * speed * (input::Keyboard::D.pressed() - input::Keyboard::A.pressed());

		if (input::Mouse::Right.pressed()) {
			auto md = input::Mouse::GetDeltaPos() * 0.1f;
			rot.y += md.x;
			rot.x += md.y;
		}
		rot.x = Math::Clamp(rot.x, -85.f, 85.f);


		static f32 t = 1.0f;
		t -= modelRotSpeed;
		// 行列更新
		auto modelScale = 100.0f;
		auto viewMtx = Matrix::Perspective(60, 1.0f * color2RT->width() / color2RT->height(), 0.01f, 10000.0f) * Matrix::TRS(pos, rot, Vec3::One).inverse();
		auto skyMtx = Matrix::Scale(Vec3(1, 1, 1) * modelScale);
		auto ukuleleMtx = Matrix::TRS(Vec3::Zero, Quat(0, t, 70), Vec3::One);

		graphics::Material::SetGlobalColor(TC("LightDir"), Color(1, 1, 1));
		graphics::Material::SetGlobalMatrix(TC("Matrix"), viewMtx);
		sky.setMatrix(skyMtx);
		ukulele.setMatrix(ukuleleMtx);



		// 表示を更新(Present)
		display->update();

		cmdList->begin();

		cmdList->beginRenderPass(frameBuffer);

		{
			cmdList->pushMarker(TC("My"));

			sky.draw(cmdBuf);
			ukulele.draw(cmdBuf);

			cmdList->popMarker();
		}

		if (true) {
			cmdList->pushMarker(TC("ImGui"));
			ImGui::BeginFrame();

			ImGui::BeginMainMenuBar();

			static bool bShowDemo = false;
			ImGui::MenuItem("ImGui", 0, &bShowDemo);

			static bool bShowComponents = true;
			ImGui::MenuItem("Components", 0, &bShowComponents);

			static bool bShowOutliner = true;
			ImGui::MenuItem("Outliner", 0, &bShowOutliner);

			ImGui::EndMainMenuBar();


			if (bShowDemo)ImGui::ShowDemoWindow();

			if (ImGui::Begin("MaterialEdirot")) {
				ImGui::SliderFloat("Speed", &modelRotSpeed, -2.0f, 2.0f);
			}
			ImGui::End();

			if (bShowComponents) {
				drawComponents();
			}
			if (bShowOutliner) {
				drawOutliner(world->getRootScene());
			}

			logInfo.update();


			ImGui::EndFrame(cmdList);
			cmdList->popMarker();
		}

		cmdList->endRenderPass();

		// ディスプレイ更新
		if (!input::Keyboard::R.pressed()) {

			// ディスプレイにバインド
			cmdList->applyDisplay(display, colorRT);
		} else {
			cmdList->applyDisplay(display, color2RT);
		}

		cmdList->end();

		// TODO コマンドの個別実行を許可する？
		cmdList->flush();

	}
	ImGui::ShutdownImGui();

	return 0;
}

int TestVullkan() {
	/*
	using namespace ob::rhi;

	// ウィンドウ生成
	platform::WindowDesc windowDesc;
	windowDesc.title = TC("Graphic Test");
	platform::Window window(windowDesc);

	// スワップチェイン
	Display display;
	{
		DisplayDesc desc;
		desc.window = window;
		desc.format = TextureFormat::RGBA8;

		display = Display(desc);
		display.setName(TC("MainWindow"));
		OB_ASSERT_EXPR(display);
	}

	// レンダーパス生成
	RenderPass renderPass;
	{
		RenderPassDescHelper desc;
		auto color = desc.addAttachment(TextureFormat::RGBA8);

		auto colorPass = desc.addSubpassXCX({ color });

		desc.addDependeny(-1, colorPass);

		RenderPass renderPass(desc);

	}

	//描画ターゲット生成
	RenderTexture colorRT;
	Size screenSize = window.getSize();
	{
		RenderTextureDesc desc;
		desc.format = TextureFormat::RGBA8;
		desc.size = Size(1280,720);

		colorRT = RenderTexture(desc);
		OB_ASSERT_EXPR(colorRT);
	}

	// フレームバッファ生成
	FrameBuffer frameBuffer;
	{
		FrameBufferDesc desc;
		desc.renderPass = renderPass;
		desc.attachments.push_back(colorRT);

	}

	// コマンドリスト生成
	CommandList cmdList;
	{
		CommandListDesc desc;
		desc.type = CommandListType::Graphic;
		cmdList = CommandList(desc);
	}


	MSG msg = {};
	while (true) {

		//graphics::System::Get().update();
		GEngine->update();

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT) {
			break;
		}
		if (input::Keyboard::Escape.down())break;

		cmdList.applyDisplay(display, colorRT);

	}
	*/
	return 0;
}
