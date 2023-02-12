#include <Windows.h>

#include <Framework/RHI/All.h>
#include <Framework/RHI/MeshData.h>

#include <Framework/Platform/Window.h>

#include <Framework/Input/All.h>
#include <Framework/Input/Config.h>

#include <Framework/Engine/Engine.h>
#include <Framework/Engine/EngineConfig.h>
#include <Plugins/ImGui/ImGui.h>

#include <Framework/Engine/Scene.h>
#include <Framework/Engine/Entity.h>

#include <Framework/Engine/Component/TransformComponent.h>
#include <Test/ComponentTest.h>


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

void OctbitInit(ob::engine::EngineConfig& config) {
	{
		rhi::Config c;
		c.frameBufferCount = 3;
		config.set(c);
	}
	{
		input::Config c;
		//settings.useKeyboard = false;
		config.set(c);
	}

	Link_DirectX12();
	//Link_Vulkan();
	Link_Input();

	Link_Entity();
}

int OctbitMain() {

	TestDirectX12();

	return 0;
}


struct Vert {
	Vec4 pos;
	Vec4 normal;
	Vec2 uv;
};

struct CBuf {
	Matrix matrix = Matrix::Identity;
	Color color = Color::Red;
};

int TestDirectX12() {

	using namespace ob::rhi;

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

	Ref<RenderPass> renderPass;
	{
		RenderPassDescHelper desc;
		auto color = desc.addAttachment(TextureFormat::RGBA8);
		auto color2 = desc.addAttachment(TextureFormat::RGBA8);
		auto depth = desc.addAttachment(TextureFormat::D32);
		auto pass0 = desc.addSubpassXCD({ color,color2 },depth);

		renderPass = RenderPass::Create(desc);
		desc.name = TC("Forward");
		OB_ASSERT_EXPR(renderPass);
	}

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

	display->bindTexture(colorRT);

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


	Ref<Shader> vs;
	Ref<Shader> ps;
	{
		String code;
		code.append(TC("SamplerState g_mainSampler:register(s0);						\n"));
		code.append(TC("Texture2D g_mainTex:register(t0);								\n"));
		code.append(TC("cbuffer Param : register(b0) {									\n"));
		code.append(TC("  float4x4 g_mtx;												\n"));
		code.append(TC("  float4x4 g_mtx2;												\n"));
		code.append(TC("  float4 g_col;													\n"));
		code.append(TC("};																\n"));
		code.append(TC("// IN / OUT														\n"));
		code.append(TC("struct VsIn {													\n"));
		code.append(TC("  float4 pos	:POSITION;										\n"));
		code.append(TC("  float4 normal	:NORMAL;										\n"));
		code.append(TC("  float2 uv		:TEXCOORD;										\n"));
		code.append(TC("};																\n"));
		code.append(TC("struct PsIn {													\n"));
		code.append(TC("  float4 pos	:SV_POSITION;									\n"));
		code.append(TC("  float4 normal	:NORMAL;										\n"));
		code.append(TC("  float2 uv		:TEXCOORD;										\n"));
		code.append(TC("};																\n"));
		code.append(TC("struct PsOut {													\n"));
		code.append(TC("  float4 color0	:SV_TARGET0;									\n"));
		code.append(TC("  float4 color1	:SV_TARGET1;									\n"));
		code.append(TC("};																\n"));
		code.append(TC("// エントリ														\n"));
		code.append(TC("PsIn VS_Main(VsIn i) {											\n"));
		code.append(TC("    PsIn o;														\n"));
		code.append(TC("    o.pos = mul(g_mtx,i.pos);									\n"));
		code.append(TC("    o.uv = i.uv;												\n"));
		code.append(TC("    o.normal = i.normal;										\n"));
		code.append(TC("    return o;													\n"));
		code.append(TC("}																\n"));
		code.append(TC("PsOut PS_Main(PsIn i){											\n"));
		code.append(TC("    PsOut o;													\n"));
		code.append(TC("    float4 color = g_mainTex.Sample(g_mainSampler,i.uv);		\n"));
		code.append(TC("    o.color1 = i.normal;										\n"));
		//code.append(TC("    color.xyz*=dot((i.normal.xyz-0.5)*2,float3(1,0,0))*0.5+1.5;	\n"));
		code.append(TC("    o.color0 = color;											\n"));
		code.append(TC("    return o;													\n"));
		code.append(TC("}																\n"));

		vs = VertexShader::Create(code);
		ps = PixelShader::Create(code);
		OB_ASSERT_EXPR(vs&&ps);
	}

	Ref<RootSignature> signature;
	{
		RootSignatureDesc desc(
			{
				RootParameter::Range(DescriptorRangeType::SRV,1,0),
				RootParameter::Range(DescriptorRangeType::CBV,1,0),
			},
			{
				StaticSamplerDesc(SamplerDesc(),0),
			}
		);
		desc.name = TC("Common");
		signature = RootSignature::Create(desc);
		OB_ASSERT_EXPR(signature);
	}

	Ref<PipelineState> pipeline;
	{
		PipelineStateDesc desc;
		desc.name = TC("ModelDraw");
		desc.renderPass = renderPass;
		desc.subpass = 0;

		desc.rootSignature = signature;
		desc.vs = vs;
		desc.ps = ps;
		desc.vertexLayout.attributes = {
			VertexAttribute(Semantic::Position,offsetof(Vert,pos),Type::Float,4),
			VertexAttribute(Semantic::Normal,offsetof(Vert,normal),Type::Float,4),
			VertexAttribute(Semantic::TexCoord,offsetof(Vert,uv),Type::Float,2),
		};
		desc.blend[0] = BlendDesc::None;
		desc.blend[1] = BlendDesc::None;
		desc.rasterizer.cullMode = CullMode::None;
		desc.depthStencil.depth.enable = true;
		desc.depthStencil.stencil.enable = false;

		pipeline = PipelineState::Create(desc);
		OB_ASSERT_EXPR(pipeline);
	}

	Ref<Buffer> buffer;
	CBuf cbuf;
	{
		BufferDesc desc = BufferDesc::Constant(100, BindFlag::PixelShaderResource);
		desc.name = TC("TestConstant");
		buffer = Buffer::Create(desc);
		OB_ASSERT_EXPR(buffer);
		buffer->updateDirect(cbuf,0);
	}
	Ref<Buffer> buffer2;
	CBuf cbuf2;
	{
		BufferDesc desc = BufferDesc::Constant(100, BindFlag::PixelShaderResource);
		desc.name = TC("TestConstant");
		buffer2 = Buffer::Create(desc);
		OB_ASSERT_EXPR(buffer);
		buffer2->updateDirect(cbuf2, 0);
	}

	Ref<Texture> tex;
	{
		tex = Texture::Load(TC("Asset/Texture/sky.dds"));
		OB_ASSERT_EXPR(tex);
	}

	auto dt = DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, 1);
	dt->setResource(0, tex);

	auto dt2 = DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, 1);
	dt2->setResource(0, buffer);
	auto dt3 = DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, 1);
	dt3->setResource(0, buffer2);



	auto loadMesh = [](std::string path) {

		Array<Tuple<Ref<Buffer>, Ref<Buffer>, size_t>> meshes;

		// Initialize Loader
		objl::Loader Loader;

		// Load .obj File
		if (Loader.LoadFile(path)) {
			LOG_INFO("モデル読み込み");
			for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
			{

				MeshData<Vert> mesh;
				objl::Mesh& curMesh = Loader.LoadedMeshes[i];

				String name;
				StringEncoder::Encode(curMesh.MeshName, name);

				LOG_INFO("    mesh={}", name);

				for (int j = 0; j < curMesh.Vertices.size(); j++)
				{
					auto pos = curMesh.Vertices[j].Position;
					auto uv = curMesh.Vertices[j].TextureCoordinate;
					auto normal = curMesh.Vertices[j].Normal;
					auto& vert = mesh.vertices.emplace_back();
					vert.pos = Vec4(pos.X, pos.Y, pos.Z, 1.0f);
					vert.normal = Vec4(normal.X, normal.Y, normal.Z, 1.0f);
					vert.uv = Vec2(uv.X, uv.Y);
				}

				for (int j = 0; j < curMesh.Indices.size(); j++)
				{
					mesh.indices.push_back(curMesh.Indices[j]);
				}

				Ref<Buffer> vertexBuffer;
				{
					auto desc = BufferDesc::Vertex<Vert>(mesh.vertices.size());
					desc.name = Format(TC("Model[{}]"), i);
					vertexBuffer = Buffer::Create(desc, BlobView(mesh.vertices));
					OB_ASSERT_EXPR(vertexBuffer);
				}

				Ref<Buffer> indexBuffer;
				{
					auto desc = BufferDesc::Vertex<decltype(mesh)::index_type>(mesh.indices.size());
					desc.name = Format(TC("Model[{}]"), i);
					indexBuffer = Buffer::Create(desc, BlobView(mesh.indices));
					OB_ASSERT_EXPR(indexBuffer);
				}
				meshes.emplace_back(vertexBuffer, indexBuffer, mesh.indices.size());
			}
		} else {
			LOG_INFO("モデルファイルが見つかりませんでした。");
		}

		return std::move(meshes);
	};

	auto skyMesh = loadMesh("Asset/Model/sky.obj");
	auto monkyMesh = loadMesh("Asset/Model/monky.obj");

	Ref<CommandList> cmdList;
	{
		CommandListDesc desc;
		desc.name = TC("MainCommandList");
		desc.type = CommandListType::Graphic;
		cmdList = CommandList::Create(desc);
		OB_ASSERT_EXPR(cmdList);
	}

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	ImGui::StyleColorsClassic();

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

	//------ループ-----

	Vec3 pos(0, 0, -10);
	Rot rot = Rot::Identity;

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

		// 表示を更新(Present)
		display->update();

		cmdList->begin();
		cmdList->beginRenderPass(frameBuffer);
		//cmdList.clearColors();
		//cmdList.clearDepthStencil();
		{
			cmdList->setPipelineState(pipeline);

			SetDescriptorTableParam params[] = {
				SetDescriptorTableParam(dt,0),
				SetDescriptorTableParam(dt2,1),
			};
			cmdList->setRootDesciptorTable(params, 2);

			for (auto& [vert, index, count] : skyMesh) {
				cmdList->setVertexBuffer(vert);
				cmdList->setIndexBuffer(index);

				DrawIndexedParam param{};
				param.indexCount = count;
				cmdList->drawIndexed(param);
			}


			SetDescriptorTableParam params2[] = {
				SetDescriptorTableParam(dt3,1),
			};
			cmdList->setRootDesciptorTable(params2, 1);
			for (auto& [vert, index, count] : monkyMesh) {
				cmdList->setVertexBuffer(vert);
				cmdList->setIndexBuffer(index);

				DrawIndexedParam param{};
				param.indexCount = count;
				cmdList->drawIndexed(param);
			}

		}


		{
			imgui::BeginFrame();

			ImGui::NewFrame();
			ImGui::ShowDemoWindow();

			//auto pos = cbuf.matrix* Vec3(0, 0, 0);
			//pos.y *= -1;
			//pos = pos * 0.5f + 0.5;
			//ImGui::GetWindowDrawList()->AddCircle({ pos.x * window.getSize().width ,pos.y * window.getSize().height},10,0xFFFFFFFF);

			ImGui::Render();

			imgui::EndFrame(cmdList);
		}


		cmdList->endRenderPass();

		if (input::Keyboard::Z.pressed()){
			cmdList->applyDisplay(display, color2RT);
		} else {
			cmdList->applyDisplay(display, colorRT);
		}

		cmdList->end();

		// TODO コマンドの個別実行を許可する？
		cmdList->flush();

		// 入力更新
		const auto rspd = 90 / 60.f;
		Rot r2(rot.x, rot.y, 0);
		static auto speed = 4 / 60.f;
		speed += input::Mouse::Wheel.value()*0.0001f;

		if (input::Keyboard::W.pressed())pos += r2.front() * speed;
		if (input::Keyboard::S.pressed())pos -= r2.front() * speed;
		if (input::Keyboard::D.pressed())pos += r2.right() * speed;
		if (input::Keyboard::A.pressed())pos -= r2.right() * speed;
		if (input::Keyboard::LeftArrow.pressed())rot.y -= rspd;
		if (input::Keyboard::RightArrow.pressed())rot.y += rspd;
		if (input::Keyboard::UpArrow.pressed())rot.x -= rspd;
		if (input::Keyboard::DownArrow.pressed())rot.x += rspd;

		if (input::Mouse::Left.pressed()) {
			auto md = input::Mouse::GetDeltaPos() * 0.1f;
			rot.y += md.x;
			rot.x += md.y;
		}
		rot.x = Math::Clamp(rot.x, -85.f, 85.f);


		// カメラバッファ更新
		auto modelScale = 100.0f;
		cbuf.matrix = Matrix::Perspective(60, 1.0f * color2RT->width() / color2RT->height(), 0.01f, 10000.0f) * Matrix::TRS(pos, rot, Vec3::One).inverse() * Matrix::Rotate(0, 180, 0) * Matrix::Scale(Vec3(1, -1, 1) * modelScale);
		cbuf2.matrix = Matrix::Perspective(60,1.0f*color2RT->width()/ color2RT->height(), 0.01f, 10000.0f) * Matrix::TRS(pos, rot, Vec3::One).inverse() * Matrix::Rotate(0, 180, 0) * Matrix::Scale(Vec3(1,1,1)* 1.0f);
		buffer->updateDirect(cbuf, 0);
		buffer2->updateDirect(cbuf2,0);
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
