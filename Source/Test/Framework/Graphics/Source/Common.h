﻿//***********************************************************
//! @file
//! @brief		Box のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Platform/Window.h>
#include <Framework/Core/Utility/DI.h>
#include <Framework/RHI/All.h>
#include <Plugins/DirectX12RHI/System.h>
#include <Windows.h>

using namespace ob::core;
using namespace ob::rhi;

struct Vertex {
	Vector4 pos;
	Color color;
};

class RHIBase {
public:
	RHIBase() {
		using namespace ob::rhi;

		ServiceInjector injector;
		ServiceContainer container;
		rhi::dx12::Register(injector);
		rhi::Register(injector);

		injector.create<RHI>(m_container);
	}
private:
	ServiceContainer m_container;
	Ref<Display> m_window;
	Ref<RenderPass> m_renderPass;
	Ref<RenderTexture> m_colorRT;
	Ref<FrameBuffer> m_frameBuffer;
	Ref<Shader> m_vs;
	Ref<Shader> m_ps;
	Ref<RootSignature> m_signature;
	Ref<PipelineState> m_pipeline;

};
using namespace ob;

void CommonStartup() {

	using namespace ob::rhi;

	ServiceInjector injector;
	ServiceContainer container;
	rhi::dx12::Register(injector);
	rhi::Register(injector);

	injector.create<RHI>(container);
	{

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
			auto pass0 = desc.addSubpassXCX(TC("Opaque"), {color});

			renderPass = RenderPass::Create(desc);
			OB_ASSERT_EXPR(renderPass);
		}

		// 描画先生成
		Ref<RenderTexture> colorRT;
		{
			RenderTextureDesc desc;
			desc.name = TC("ColorRT");
			desc.size = display->getDesc().size;
			desc.format = TextureFormat::RGBA8;
			desc.clear.color = Color::Black;
			desc.display = display;

			colorRT = RenderTexture::Create(desc);
			OB_ASSERT_EXPR(colorRT);
		}

		Ref<FrameBuffer> frameBuffer;
		{
			FrameBufferDesc desc;
			desc.name = TC("Test");
			desc.renderPass = renderPass;
			desc.attachments.push_back(colorRT);

			frameBuffer = FrameBuffer::Create(desc);
			OB_ASSERT_EXPR(frameBuffer);
		}


		Ref<Shader> vs;
		Ref<Shader> ps;
		{
			String code;
			code.append(TC("SamplerState g_mainSampler:register(s0);						\n"));
			code.append(TC("cbuffer Param : register(b0) {									\n"));
			code.append(TC("  float4 g_col;													\n"));
			code.append(TC("};																\n"));
			code.append(TC("// IN / OUT														\n"));
			code.append(TC("struct VsIn {													\n"));
			code.append(TC("  float4 pos	:POSITION;										\n"));
			code.append(TC("  float2 uv		:TEXCOORD;										\n"));
			code.append(TC("};																\n"));
			code.append(TC("struct PsIn {													\n"));
			code.append(TC("  float4 pos	:SV_POSITION;									\n"));
			code.append(TC("  float2 uv		:TEXCOORD;										\n"));
			code.append(TC("};																\n"));
			code.append(TC("struct PsOut {													\n"));
			code.append(TC("  float4 color	:SV_TARGET0;									\n"));
			code.append(TC("};																\n"));
			code.append(TC("// エントリ														\n"));
			code.append(TC("PsIn VS_Main(VsIn i) {											\n"));
			code.append(TC("    PsIn o;														\n"));
			code.append(TC("    o.pos = i.pos;												\n"));
			code.append(TC("    o.uv = i.uv;												\n"));
			code.append(TC("    return o;													\n"));
			code.append(TC("}																\n"));
			code.append(TC("PsOut PS_Main(PsIn i){											\n"));
			code.append(TC("    PsOut o;													\n"));
			code.append(TC("    float4 color = g_col;										\n"));
			code.append(TC("    o.color = color;											\n"));
			code.append(TC("    return o;													\n"));
			code.append(TC("}																\n"));

			vs = Shader::CompileVS(code);
			ps = Shader::CompilePS(code);
			OB_ASSERT_EXPR(vs && ps);
		}

		Ref<RootSignature> signature;
		{
			RootSignatureDesc desc(
				{
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


		struct Vert {
			Vec4 pos;
			Vec2 uv;
		};

		struct CBuf {
			Color color = Color::Red;
		};

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
				VertexAttribute(Semantic::TexCoord,offsetof(Vert,uv),Type::Float,2),
			};
			desc.blend[0] = BlendDesc::AlphaBlend;
			desc.rasterizer.cullMode = CullMode::None;
			desc.depthStencil.depth.enable = false;
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


		Array<Vert> vertices{
			{Vec4(0,0,0,1),Vec2(0,0)},
			{Vec4(1,0,0,1),Vec2(1,0)},
			{Vec4(0,1,0,1),Vec2(0,1)},
			{Vec4(1,1,0,1),Vec2(1,1)},
		};
		Array<u16> indices{
			0,1,2,2,1,3
		};

		Ref<Buffer> vertexBuffer;
		{
			auto desc = BufferDesc::Vertex<Vert>(vertices.size());
			desc.name = TC("ModelVertices");
			vertexBuffer = Buffer::Create(desc, BlobView(vertices));
			OB_ASSERT_EXPR(vertexBuffer);
		}

		Ref<Buffer> indexBuffer;
		{
			auto desc = BufferDesc::Vertex<u16>(indices.size());
			desc.name = TC("ModelIndices");
			indexBuffer = Buffer::Create(desc, BlobView(indices));
			OB_ASSERT_EXPR(indexBuffer);
		}

		Ref<CommandList> cmdList;
		{
			CommandListDesc desc;
			desc.name = TC("MainCommandList");
			desc.type = CommandListType::Graphic;
			cmdList = CommandList::Create(desc);
			OB_ASSERT_EXPR(cmdList);
		}

		auto dt = DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV, 1);
		dt->setResource(0, buffer);


		//------ループ-----

		MSG msg = {};
		for (s32 i = 0; i < 60;++i) {

			cbuf.color = HSV(i * 6, 1, 1).toColor();
			buffer->updateDirect(cbuf, 0);

			if (auto rhi = container.get<RHI>()) {
				rhi->update();
			}

			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (msg.message == WM_QUIT) {
				break;
			}

			// 表示を更新(Present)
			display->update();

			cmdList->begin();
			cmdList->beginRenderPass(frameBuffer);

			{
				cmdList->setPipelineState(pipeline);


				cmdList->setVertexBuffer(vertexBuffer);
				cmdList->setIndexBuffer(indexBuffer);

				StaticArray<SetDescriptorTableParam,1> params = {
					SetDescriptorTableParam(dt,0),
				};
				cmdList->setRootDesciptorTable(params.data(), params.size());

				DrawIndexedParam param{};
				param.indexCount = indices.size();
				cmdList->drawIndexed(param);
			}
			cmdList->endRenderPass();

			cmdList->applyDisplay(display, colorRT);

			cmdList->end();

			// TODO コマンドの個別実行を許可する？
			cmdList->flush();

		}
	}
}