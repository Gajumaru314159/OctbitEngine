#include <Windows.h>

#include <Framework/RHI/All.h>

#include <Framework/Platform/Window.h>

#include <Framework/Input/All.h>

#include <Framework/Engine/Engine.h>
#include <Framework/Engine/EngineConfig.h>
#include <Framework/Engine/World.h>
#include <Framework/Engine/Scene.h>
#include <Framework/Engine/Entity.h>

#include <Framework/Graphic/Material.h>
#include <Framework/Graphic/Mesh.h>

#include <Plugins/ImGui/ImGui.h>

#include <Framework/Engine/Component/TransformComponent.h>
#include <Framework/Graphic/Component/CameraComponent.h>
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
		//c.enablePIX = true;
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

void drawOutliner_Entity(engine::Entity* entity) {
	if (!entity)return;
	auto name = ImGui::ToImChars(entity->name());
	bool empty = entity->getChildren().empty();
	
	auto flag = empty?ImGuiTreeNodeFlags_Leaf:0;
	flag |= ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::TreeNodeEx(name,flag,name)) {

		for (auto& child : entity->getChildren()) {

			drawOutliner_Entity(child);

		}

		ImGui::TreePop();

	}

}
void drawOutliner_Scene(const Ref<engine::Scene>& scene) {
	if (!scene)return;
	auto name = ImGui::ToImChars(scene->getName());
	bool empty = scene->getChildren().empty()&& scene->getEntities().empty();

	auto flag = empty ? ImGuiTreeNodeFlags_Leaf : 0;
	flag |= ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::TreeNodeEx(name, flag, name)) {

		for (auto& child : scene->getChildren()) {

			drawOutliner_Scene(child);

		}

		ImGui::ScopedTextColor textColor(Color::Yellow);

		for (auto& entity : scene->getEntities()) {

			drawOutliner_Entity(entity.get());

		}

		ImGui::TreePop();

	}
}
void drawOutliner(const Ref<engine::Scene>& scene) {

	if (ImGui::Begin("Outliner")) {
		drawOutliner_Scene(scene);
	}
	ImGui::End();

}


void drawComponents(engine::Entity& entity) {
	for (auto& component : entity.componets()) {
		StringBase<char> name;
		StringEncoder::Encode(component->getTypeId().name(), name);
		if (ImGui::CollapsingHeader(name.c_str())) {

			ImGui::ScopedIndent indent;

			if (component->getTypeId() == TypeId::Get<engine::TransformComponent>()) {
				auto c = reinterpret_cast<engine::TransformComponent*>(component.get());
				{
					Vec3 value = c->getLocal().position;
					if (ImGui::DragFloat3("Position", value)) {
						c->setLocalPosition(value);
					}
				}
				{
					auto eulerAngles = c->getLocal().rotation.toRot();
					f32 xyz[] = { eulerAngles.x,eulerAngles.y,eulerAngles.z };
					ImGui::DragFloat3("Rotation", xyz);
				}
				{
					Vec3 value = c->getLocal().scale;
					if (ImGui::DragFloat3("Scale", value)) {
						c->setLocalScale(value);
					}
				}
			}
			if (component->getTypeId() == TypeId::Get<graphic::CameraComponent>()) {
				auto c = reinterpret_cast<graphic::CameraComponent*>(component.get());
				{
					f32 value[] = { c->getFov() };
					if (ImGui::SliderFloat("FovY", value,0,180)) {
						c->setFov(value[0]);
					}
				}
				{
					auto rect = c->getVieportRect();
					f32 values[] = { rect.left,rect.top,rect.right,rect.bottom };
					if (ImGui::DragFloat4("ViewportRect", values,0.01f,0.0f,1.0f)) {
						c->setVieportRect({ values[0] ,values[1],values[2] ,values[3] });
					}
				}
				{
					auto value = c->getClearColor();
					if (ImGui::ColorEdit3("ClearColor", value)) {
						c->setClearColor(value);
					}
				}
			}

		}
	}
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
	Model ukulele("Asset/Model/Ukulele.obj",TC("Asset/Model/Ukulele_col.dds"));

	Material::RegisterRenderPass(engine::Name(TC("Opaque")), renderPass, 0);

	Ref<CommandBuffer> cmdBuf = CommandBuffer::Create(cmdList);

	// ImGui初期化
	ImGui::Startup(window, renderPass);



	// シーン生成テスト
	auto world = engine::World::Create(TC("Test"));
	auto scene = engine::Scene::Create(TC("Sample"));
	auto entity = engine::Entity::Create(TC("Test"));

	world->getRootScene()->addSubScene(scene);

	if (entity && scene) {
		entity->setName(TC("Entity"));
		scene->addEntity(entity);

		auto child = engine::Entity::Create(TC("Child"));
		if (child) {
			child->setName(TC("Child"));
			entity->addChild(child);
		}

		entity->addComponent<engine::TransformComponent>();
		entity->addComponent<graphic::CameraComponent>();

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
	f32 modelRotSpeed = 1.0f;

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
		auto ukuleleMtx = Matrix::TRS(Vec3::Zero,Quat(0,t,70),Vec3::One);

		graphic::Material::SetGlobalColor(TC("LightDir"), Color(1,1,1));
		graphic::Material::SetGlobalMatrix(TC("Matrix"), viewMtx);
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

		if(true){
			cmdList->pushMarker(TC("ImGui"));
			ImGui::BeginFrame();

			ImGui::BeginMainMenuBar();

			static bool bShowDemo = false;
			ImGui::MenuItem("ImGui", 0, &bShowDemo);

			static bool bShowComponents = true;
			ImGui::MenuItem("Components", 0, &bShowComponents);

			static bool bShowOutliner= true;
			ImGui::MenuItem("Outliner", 0, &bShowOutliner);

			ImGui::EndMainMenuBar();


			if(bShowDemo)ImGui::ShowDemoWindow();

			if (ImGui::Begin("MaterialEdirot")) {
				ImGui::SliderFloat("Speed", &modelRotSpeed, -2.0f, 2.0f);
			}
			ImGui::End();

			if (bShowComponents) {
				if (entity) {
					drawComponents(*entity);
				}
			}
			if (bShowOutliner) {
				drawOutliner(world->getRootScene());
			}



			ImGui::EndFrame(cmdList);
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
	ImGui::Shutdown();
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
