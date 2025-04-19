//***********************************************************
//! @file
//! @brief		Buffer のテスト
//! @author		Gajumaru
//***********************************************************
#include <RHITestBase.h>
#include <magic_enum.hpp>

class RHITest : public RHITestBase {};

TEST_F(RHITest, Triangle) {
	return;
	using namespace ob::rhi;

	Logger log;

	{

		// ウィンドウ生成
		platform::WindowDesc windowDesc;
		windowDesc.title = "Graphic Test";
		platform::Window window(windowDesc); window.show();

		// ディスプレイ
		Ref<Display> display;
		{
			DisplayDesc desc;
			desc.name = "RHITest Triangle";
			desc.window = window;
			display = Display::Create(desc);
			OB_ASSERT_EXPR(display);
		}

		// 描画先生成
		Ref<RenderTexture> colorRT;
		{
			RenderTextureDesc desc;
			desc.name = "ColorRT";
			desc.size = display->getDesc().size;
			desc.format = TextureFormat::RGBA8;
			desc.clear.color = Color::Black;

			colorRT = RenderTexture::Create(desc);
			OB_ASSERT_EXPR(colorRT);
		}

		Ref<Shader> vs;
		Ref<Shader> ps;
		{
			String code = R"(	
struct VsIn {
	float4 pos : POSITION;
};
struct PsIn {
	float4 pos : SV_POSITION;
};
struct PsOut {
	float4 color : SV_TARGET0;
};											
PsIn VS_Main(VsIn i) {
	PsIn o;
	o.pos = i.pos;
	return o;
}
PsOut PS_Main(PsIn i) {
	PsOut o;
	o.color = float4(1.f,1.f,0.f,1.f);
	return o;
}
)";

			vs = Shader::CompileVS(code);
			ps = Shader::CompilePS(code);
			OB_ASSERT_EXPR(vs && ps);
		}

		Ref<RootSignature> signature;
		{
			RootSignatureDesc desc;
			signature = RootSignature::Create(desc);
			OB_ASSERT_EXPR(signature);
		}


		struct Vert {
			Vec4 pos;
		};

		Ref<PipelineState> pipeline;
		{
			PipelineStateDesc desc;
			desc.name = "ModelDraw";
			desc.colors = { TextureFormat::RGBA8 };

			desc.rootSignature = signature;
			desc.vs = vs;
			desc.ps = ps;
			desc.vertexLayout.attributes = {
				VertexAttribute(Semantic::Position,offsetof(Vert,pos),ElementType::Float,4),
			};
			desc.blend[0] = BlendDesc::AlphaBlend;
			desc.rasterizer.cullMode = CullMode::None;
			desc.depthStencil.depth.enable = false;
			desc.depthStencil.stencil.enable = false;

			pipeline = PipelineState::Create(desc);
			OB_ASSERT_EXPR(pipeline);
		}

		Vector<Vert> vertices{
			{Vec4(0,0,0,1)},
			{Vec4(1,0,0,1)},
			{Vec4(0,1,0,1)},
		};
		Vector<u16> indices{
			0,1,2
		};

		Ref<Buffer> vertexBuffer;
		{
			auto desc = BufferDesc::Vertex<Vert>(vertices.size());
			desc.name = "ModelVertices";
			vertexBuffer = Buffer::Create(desc, BlobView(vertices));
			OB_ASSERT_EXPR(vertexBuffer);
		}

		Ref<Buffer> indexBuffer;
		{
			auto desc = BufferDesc::Vertex<u16>(indices.size());
			desc.name = "ModelIndices";
			indexBuffer = Buffer::Create(desc, BlobView(indices));
			OB_ASSERT_EXPR(indexBuffer);
		}

		Ref<CommandList> cmdList;
		{
			CommandListDesc desc;
			desc.name = "MaincmdList";
			desc.type = CommandListType::Graphic;
			cmdList = CommandList::Create(desc);
			OB_ASSERT_EXPR(cmdList);
		}

		auto start = DateTime::Now();

		f32 endTime = 5.0f;

		while (true) {

			if (System::Update() == false);// break;

			auto time = TimeSpan(start, DateTime::Now()).totalSecondsF();

			if (endTime < time)break;

			cmdList->begin();

			Viewport viewport;
			viewport.right = colorRT->width();
			viewport.bottom = colorRT->height();
			cmdList->setViewport(&viewport, 1);

			cmdList->setRenderTarget(colorRT);
			cmdList->clearColors();

			cmdList->setPipelineState(pipeline);

			cmdList->setVertexBuffer(vertexBuffer);
			cmdList->setIndexBuffer(indexBuffer);

			DrawIndexedParam param;
			param.indexCount = 3;
			param.startIndex = 0;
			param.startVertex = 0;
			cmdList->drawIndexed(param);

			cmdList->applyDisplay(display, colorRT);

			cmdList->end();
			cmdList->flush();

			RHI::Get()->update();
			display->update();

			Thread::Sleep(33);
		}

	}
}