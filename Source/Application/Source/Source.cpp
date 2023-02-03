#include <Windows.h>

#include <Framework/RHI/All.h>
#include <Framework/RHI/MeshData.h>

#include <Framework/Platform/Window.h>

#include <Framework/Input/All.h>
#include <Framework/Input/Config.h>

#include <Framework/Engine/Engine.h>
#include <Framework/Engine/EngineConfig.h>

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
		desc.window = window;
		display = Display::Create(desc);
		display->setName(TC("MainWindow"));
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
		OB_ASSERT_EXPR(renderPass);
	}

	Ref<RenderTexture> colorRT;
	{
		RenderTextureDesc desc;
		desc.name = TC("Color");
		desc.size = display->getDesc().size;
		desc.format = TextureFormat::RGBA8;
		desc.clear.color = Color::Gray;

		colorRT = RenderTexture::Create(desc);
		OB_ASSERT_EXPR(colorRT);
	}
	Ref<RenderTexture> color2RT;
	{
		RenderTextureDesc desc;
		desc.name = TC("Color");
		desc.size = display->getDesc().size;
		desc.format = TextureFormat::RGBA8;
		desc.clear.color = Color::White;

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
		code.append(TC("    o.color1 = color;											\n"));
		code.append(TC("    color.xyz*=abs(dot(i.normal.xyz,float3(0,0,1)));			\n"));
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
		signature = RootSignature::Create(desc);
		signature->setName(TC("TestRootSignature"));
		OB_ASSERT_EXPR(signature);
	}

	Ref<PipelineState> pipeline;
	{
		PipelineStateDesc desc;
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
		desc.blend[0] = BlendDesc::AlphaBlend;
		desc.rasterizer.cullMode = CullMode::None;
		desc.depthStencil.depth.enable = true;
		desc.depthStencil.stencil.enable = false;

		pipeline = PipelineState::Create(desc);
		pipeline->setName(TC("TestPipeline"));
		OB_ASSERT_EXPR(pipeline);
	}

	Ref<Buffer> buffer;
	CBuf cbuf;
	{
		BufferDesc desc = BufferDesc::Constant(100, BindFlag::PixelShaderResource);
		buffer = Buffer::Create(desc);
		buffer->setName(TC("TestBuffer"));
		OB_ASSERT_EXPR(buffer);
		buffer->updateDirect(cbuf);
	}

	Ref<Texture> tex;
	{
		FileStream fs(TC("Asset/Texture/test.dds"));
		if (fs) {
			Blob blob(fs.size());
			fs.read(blob.data(), blob.size());
			tex = Texture::Create(blob);
			tex->setName(TC("test.dds"));
		}

		OB_ASSERT_EXPR(tex);
	}

	auto dt = DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, 1);
	dt->setResource(0, tex);

	auto dt2 = DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, 1);
	dt2->setResource(0, buffer);


	MeshData<Vert> mesh;
	{
		// Initialize Loader
		objl::Loader Loader;

		// Load .obj File
		if (Loader.LoadFile("Asset/Model/monky.obj")) {
			for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
			{
				objl::Mesh& curMesh = Loader.LoadedMeshes[i];

String name;
StringEncoder::Encode(curMesh.MeshName, name);

LOG_INFO("name={}", name);

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
break;
			}
			} else {
				LOG_INFO("モデルファイルが見つかりませんでした。");
		}
	}

	Ref<Buffer> vertexBuffer;
	{
		auto desc = BufferDesc::Vertex<Vert>(mesh.vertices.size());
		vertexBuffer = Buffer::Create(desc, BlobView(mesh.vertices));
		OB_ASSERT_EXPR(vertexBuffer);
	}

	Ref<Buffer> indexBuffer;
	{
		auto desc = BufferDesc::Vertex<decltype(mesh)::index_type>(mesh.indices.size());
		indexBuffer = Buffer::Create(desc, BlobView(mesh.indices));
		OB_ASSERT_EXPR(indexBuffer);
	}



	Ref<CommandList> cmdList;
	{
		CommandListDesc desc;
		desc.type = CommandListType::Graphic;
		cmdList = CommandList::Create(desc);
		cmdList->setName(TC("TestCommandList"));
		OB_ASSERT_EXPR(cmdList);
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
			cmdList->setRootSignature(signature);
			cmdList->setPipelineState(pipeline);


			cmdList->setVertexBuffer(vertexBuffer);
			cmdList->setIndexBuffer(indexBuffer);

			SetDescriptorTableParam params[] = {
				SetDescriptorTableParam(dt,0),
				SetDescriptorTableParam(dt2,1),
			};
			cmdList->setRootDesciptorTable(params, 2);

			DrawIndexedParam param{};
			param.indexCount = mesh.indices.size();
			cmdList->drawIndexed(param);
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
		Rot r2(0, rot.y, 0);
		auto speed = 4 / 60.f;
		if (input::Keyboard::K.pressed()) speed *= 0.5f;
		if (input::Keyboard::W.pressed())pos += r2.front() * speed;
		if (input::Keyboard::S.pressed())pos -= r2.front() * speed;
		if (input::Keyboard::D.pressed())pos += r2.right() * speed;
		if (input::Keyboard::A.pressed())pos -= r2.right() * speed;
		if (input::Keyboard::LeftArrow.pressed())rot.y -= rspd;
		if (input::Keyboard::RightArrow.pressed())rot.y += rspd;
		if (input::Keyboard::UpArrow.pressed())rot.x -= rspd;
		if (input::Keyboard::DownArrow.pressed())rot.x += rspd;
		rot.x = Math::Clamp(rot.x, -90.f, 90.f);

		// カメラバッファ更新
		cbuf.matrix = Matrix::Perspective(60, 8.f / 6.f, 0.01f, 100.0f) * Matrix::TRS(pos, rot, Vec3::One).inverse() * Matrix::Rotate(0, 180, 0);
		buffer->updateDirect(cbuf);
	}

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
