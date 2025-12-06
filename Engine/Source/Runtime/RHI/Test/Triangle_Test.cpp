//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <RHITestBase.h>
#include <magic_enum.hpp>

using namespace ob;
using namespace ob::rhi;

TYPED_TEST(RHITest, Triangle_Test) {

	using namespace ob::rhi;


	{

		// ウィンドウ生成
		platform::WindowDesc windowDesc;
		windowDesc.title = "Graphic Test";
		platform::Window window(windowDesc); window.show();


		// ディスプレイ
		Ref<SwapChain> swapChain;
		{
			SwapChainDesc desc;
			desc.name = "RHITest Triangle";
			desc.window = window;
			swapChain = SwapChain::Create(desc);
			OB_ASSERT_EXPR(swapChain);
		}

		// 描画先生成
		Ref<RenderTexture> colorRT;
		{
			RenderTextureDesc desc;
			desc.name = "ColorRT";
			desc.size = swapChain->getDesc().size;
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
	float4 position : POSITION;
	float4 color : COLOR;
};
struct PsIn {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};
struct PsOut {
	float4 color : SV_TARGET0;
};											
PsIn VS_Main(VsIn i) {
	PsIn o;
	o.position = i.position;
	o.color = i.color;
	return o;
}
PsOut PS_Main(PsIn i) {
	PsOut o;
	o.color = i.color;
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
			Vec4  position;
			Color color;
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
				VertexAttribute(Semantic::Position,offsetof(Vert,position),ElementType::Float,4),
				VertexAttribute(Semantic::Color,offsetof(Vert,color),ElementType::Float,4),
			};
			desc.vertexLayout.vertexStride = sizeof(Vert);
			desc.blend[0] = BlendDesc::AlphaBlend;
			desc.rasterizer.cullMode = CullMode::None;
			desc.depthStencil.depth.enable = false;
			desc.depthStencil.stencil.enable = false;

			pipeline = PipelineState::Create(desc);
			OB_ASSERT_EXPR(pipeline);
		}

		Vector<Vert> vertices{
			{{0.0f, 0.5f, 0.0f,1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // 上
			{{0.5f, -0.5f, 0.0f,1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // 右下
			{{-0.5f, -0.5f, 0.0f,1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, // 左下
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
			auto desc = BufferDesc::Index<u16>(indices.size());
			desc.name = "ModelIndices";
			indexBuffer = Buffer::Create(desc, BlobView(indices));
			OB_ASSERT_EXPR(indexBuffer);
		}

		Ref<CommandList> cmdList;
		{
			CommandListDesc desc;
			desc.name = "MainCmdList";
			desc.type = CommandListType::Graphic;
			cmdList = CommandList::Create(desc);
			OB_ASSERT_EXPR(cmdList);
		}

		auto start = DateTime::Now();

		f32 endTime = 5.0f;

		while (true) {

			auto time = TimeSpan(start, DateTime::Now()).totalSecondsF();

			if (endTime < time)break;

			cmdList->begin();

			BeginPassParam renderPass;
			{
				renderPass.colors.emplace_back(colorRT, RenderPassBeforeAccessType::Clear, RenderPassAfterAccessType::Preserve);
			}

			cmdList->beginRenderPass(renderPass);

			cmdList->setPipelineState(pipeline);

			cmdList->setVertexBuffer(vertexBuffer);
			cmdList->setIndexBuffer(indexBuffer);

			DrawIndexedParam param;
			param.indexCount = 3;
			param.startIndex = 0;
			param.startVertex = 0;
			cmdList->drawIndexed(param);


			cmdList->endRenderPass();


			cmdList->applySwapChain(swapChain, colorRT);

			cmdList->end();
			cmdList->flush();

			Device::Get()->update();
			swapChain->update();

			Thread::Sleep(33);
		}

		Thread::Sleep(33);

	}
}