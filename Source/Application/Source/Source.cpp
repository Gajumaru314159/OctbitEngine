#include <Windows.h>

#include <Framework/RHI/All.h>

#include <Framework/Platform/Window.h>

#include <Framework/Input/All.h>

#include <Framework/Engine/Engine.h>
#include <Framework/Engine/EngineConfig.h>
#include <Framework/Engine/Scene.h>
#include <Framework/Engine/Entity.h>

#include <Framework/Graphic/Material.h>
#include <Framework/Graphic/Mesh.h>

#include <Plugins/ImGui/ImGui.h>

#include <Framework/Engine/Component/TransformComponent.h>
#include <Test/ComponentTest.h>

#include <Model.h>

#pragma warning(push, 0)
#include <OBJ_Loader.h>
#pragma warning(pop)
//-----------------------------------------------------------------
using namespace ob;

void Link_DirectX12();
void Link_Vulkan();
void Link_Input();
int TestDirectX12();
int TestVullkan();
void Link_Entity();
void Link_GraphicModule();

void OctbitInit(ob::engine::EngineConfig& config) {
	{
		rhi::Config c;
		c.frameBufferCount = 3;
		config.set(c);
	}

	Link_DirectX12();
	//Link_Vulkan();
	Link_Input();

	Link_Entity();
	Link_GraphicModule();
}

int OctbitMain() {

	TestDirectX12();
	//TestVulkan();

	return 0;
}

int TestDirectX12() {

	using namespace ob::rhi;
	using namespace ob::graphic;

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
		auto pass0 = desc.addSubpassXCD({ color,color2 },depth);

		renderPass = RenderPass::Create(desc);
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
	Model ukulele("Asset/Model/Ukulele.obj",TC("Asset/Model/Ukulele_col.dds"));

	Material::RegisterRenderPass(engine::Name(TC("Opaque")), renderPass, 0);

	Ref<CommandBuffer> cmdBuf = CommandBuffer::Create(cmdList);

	// ImGui初期化
	imgui::Startup(window, renderPass);

	auto scene = engine::Scene::Create(TC("Sample"));
	auto entity = engine::Entity::Create();
	if (entity && scene) {
		entity->setName(TC("Entity"));
		scene->addEntity(entity);

		auto child = engine::Entity::Create();
		if (child) {
			child->setName(TC("Child"));
			entity->addChild(child);
		}

		//entity->addComponent<engine::TransformComponent>();

		//if (auto c = entity->findComponent<test::ComponentTest>()) {
		//	LOG_INFO("{}が追加されています",c->getTypeId().name());
		//}

		auto handle = entity->handle();
		if (auto ent = handle.get()) {
			LOG_INFO("Handle => {}", ent->name());
		} else {
			LOG_ERROR("Handle failed");
		}
	}

	Texture::White();
	//------ループ-----

	Vec3 pos(0, 0, -10);
	Rot rot = Rot::Identity;

	MSG msg = {};
	while (true) {

		// TODO アプリ更新内部にモジュール更新とメッセージ処理を隠ぺい
		{
			GEngine->visit([](engine::IModule& m) {m.update(); });

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

		if (input::Mouse::Left.pressed()) {
			auto md = input::Mouse::GetDeltaPos() * 0.1f;
			rot.y += md.x;
			rot.x += md.y;
		}
		rot.x = Math::Clamp(rot.x, -85.f, 85.f);

		// 行列更新
		auto modelScale = 100.0f;
		auto viewMtx = Matrix::Perspective(60, 1.0f * color2RT->width() / color2RT->height(), 0.01f, 10000.0f) * Matrix::TRS(pos, rot, Vec3::One).inverse();
		auto skyMtx = viewMtx * Matrix::Scale(Vec3(1, 1, 1) * modelScale);
		auto ukuleleMtx = viewMtx * Matrix::Scale(Vec3(1, 1, 1) * 1.0f);

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

		{
			cmdList->pushMarker(TC("ImGui"));
			imgui::BeginFrame();

			//ImGui::ShowDemoWindow();

			imgui::EndFrame(cmdList);
			cmdList->popMarker();
		}

		cmdList->endRenderPass();

		// ディスプレイ更新
		if (!input::Keyboard::Z.pressed()){

			// ディスプレイにバインド
			cmdList->applyDisplay(display, colorRT);
		} else {
			cmdList->applyDisplay(display, color2RT);
		}

		cmdList->end();

		// TODO コマンドの個別実行を許可する？
		cmdList->flush();

	}
	imgui::Shutdown();
	ImGui::DestroyContext();

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

		//graphic::System::Get().update();
		GEngine->visit([](engine::IModule& m) {m.update(); });

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
