#include <Framework/Platform/Window/Window.h>
#include <Framework/Platform/Module/ModuleManager.h>
#include <Framework/Platform/System/PlatformSystem.h>
#include <Framework/Graphic/System.h>
#include <Framework/Graphic/SwapChain.h>
#include <Framework/Graphic/RenderTarget.h>
#include <Framework/Graphic/RootSignature.h>
#include <Framework/Graphic/Shader.h>
#include <Framework/Graphic/PipelineState.h>
#include <Framework/Graphic/DescriptorTable.h>
#include <Framework/Graphic/CommandList.h>
#include <Framework/Graphic/MeshBuffer.h>
#include <Framework/Graphic/Mesh.h>
#include <Framework/Graphic/Types/CommandParam.h>
#include <Framework/Core/File/FileStream.h>
#include <Framework/Core/File/BinaryReader.h>
#include <Windows.h>

#include <Framework/Core/String/Format.h>
#include <Framework/Core/String/StringEncoder.h>
#include <Framework/Core/Misc/DateTime.h>

#include <Framework/Core/Math/Vectors.h>
#include <Framework/Core/Math/Quaternion.h>
#include <Framework/Core/Math/Rotation.h>
#include <Framework/Core/Misc/DateTime.h>
#include <Framework/Core/Misc/TimeSpan.h>
#include <Framework/Core/Misc/UUID.h>

using namespace ob;

struct Vert {
	Vec4 pos;
	Vec2 uv;
};

struct ShaderData {

};


int main() {
	using namespace ob::graphic;


	Logger::Instance();
	platform::PlatformSystem::Instance().startup();
	{
		if (false) {
			LOG_INFO("Vec2     :{}", Vec2(1, 2));
			LOG_INFO("Vec3     :{}", Vec3(1.5235223f, 2.11f, -0.01f));
			LOG_INFO("Vec4     :{}", Vec4(3.1415f, 2.235e6f, -0.01f, -123.5e-5f));
			LOG_INFO("IntVec2  :{}", IntVec2(4, 12));
			LOG_INFO("IntVec3  :{}", IntVec3(474, 124, 12));
			LOG_INFO("IntVec4  :{}", IntVec4(45, 1472, 313, 1444));
			LOG_INFO("Quat     :{}", Quat::identity);
			LOG_INFO("Rot      :{}", Rot(Math::PI, Math::EPSILON, Math::HALF_PI));
			LOG_INFO("DateTime :{}", DateTime::Now());
			LOG_INFO("TimeSpan :{}", TimeSpan::Days(23.12534573));
			LOG_INFO("TimeSpan :{}", TimeSpan::Hours(23.12534573));
			LOG_INFO("TimeSpan :{}", TimeSpan::Minutes(23.12534573));
			LOG_INFO("TimeSpan :{}", TimeSpan::Seconds(23.12534573));
			LOG_INFO("TimeSpan :{}", TimeSpan::MilliSeconds(23.12534573));
			LOG_INFO("UUID     :{}", ob::UUID::Generate());
		}

		platform::ModuleManager::Instance();
		{
			// グラフィックシステム初期化
			SystemDesc sysDesc;
			sysDesc.api = graphic::GraphicAPI::D3D12;
			sysDesc.bufferCount = 2;
			graphic::System::Instance().initialize(sysDesc);

			// ウィンドウ生成
			platform::WindowCreationDesc windowDesc;
			windowDesc.title = TC("Graphic Test");
			platform::Window window(windowDesc);
			window.show();

			// スワップチェイン
			SwapChain swapChain;
			{
				SwapchainDesc desc;
				desc.window = &window;
				swapChain = SwapChain(desc);
				swapChain.setName(TC("MainWindow"));
				OB_CHECK_ASSERT_EXPR(swapChain);
			}


			RenderTarget rt;
			{
				RenderTargetDesc desc;
				desc.size = { 640,480 };
				desc.colors = {
					ColorTextureDesc{swapChain.getDesc().format,Color::red},
				};

				desc.depth = {
					DepthTextureDesc{TextureFormat::D32,0,0},
				};

				rt = RenderTarget(desc);
				rt.setName(TC("メインレンダ―ターゲット"));
				OB_CHECK_ASSERT_EXPR(rt);
			}
			Texture tex;
			{
				FileStream fs(L"D:/Users/Gajumaru/Pictures/test.dds");
				if (fs) {
					Blob blob(fs.size());
					fs.read(blob.data(), blob.size());
					tex = Texture(blob);
					tex.setName(TC("test.dds"));
				}

				OB_CHECK_ASSERT_EXPR(tex);
			}

			RootSignature signature;
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
				signature = RootSignature(desc);
				signature.setName(TC("TestRootSignature"));
				OB_CHECK_ASSERT_EXPR(signature);
			}

			VertexShader vs;
			PixelShader ps;
			{
				String vssrc;
				vssrc.append(TC("\nstruct Output {float4 pos:SV_POSITION;float2 uv:TEXCOORD;};"));
				vssrc.append(TC("\nOutput VS_Main(float4 pos : POSITION ,float2 uv : TEXCOORD) {"));
				vssrc.append(TC("\n    Output o;"));
				vssrc.append(TC("\n    o.pos = pos;"));
				vssrc.append(TC("\n    o.uv = uv;"));
				vssrc.append(TC("\n    return o;"));
				vssrc.append(TC("\n}"));
				String pssrc;
				pssrc.append(TC("\ncbuffer Param : register(b0) {								"));
				pssrc.append(TC("\n  float4 color;												"));
				pssrc.append(TC("\n};															"));
				pssrc.append(TC("\nSamplerState g_mainSampler:register(s0);						"));
				pssrc.append(TC("\nTexture2D g_mainTex:register(t0);							"));
				pssrc.append(TC("\nstruct Output {float4 pos:SV_POSITION;float2 uv:TEXCOORD;};	"));
				pssrc.append(TC("\nfloat4 PS_Main(Output i) : SV_TARGET{						"));
				pssrc.append(TC("\n    return g_mainTex.Sample(g_mainSampler,i.uv)*color;		"));
				pssrc.append(TC("\n}															"));

				vs = VertexShader(vssrc);
				ps = PixelShader(pssrc);
				OB_CHECK_ASSERT_EXPR(vs && ps);
			}

			PipelineState pipeline;
			{
				PipelineStateDesc desc;
				desc.rootSignature = signature;
				desc.vs = vs;
				desc.ps = ps;
				desc.vertexLayout.attributes = {
					VertexAttribute(Semantic::Position,offsetof(Vert,pos),Type::Float,4),
					VertexAttribute(Semantic::TexCoord,offsetof(Vert,uv),Type::Float,2),
				};
				desc.target = rt;
				desc.blend[0] = BlendDesc::AlphaBlend;
				desc.rasterizer.cullMode = CullMode::None;

				pipeline = PipelineState(desc);
				pipeline.setName(TC("TestPipeline"));
				OB_CHECK_ASSERT_EXPR(pipeline);
			}

			Buffer buffer;
			{
				BufferDesc desc;

				desc.bufferType = BufferType::ConstantBuffer;     //!< バッファタイプ
				desc.usage = ResourceUsage::Dynamic;          //!< リソース使用法
				desc.bufferSize = 256;     //!< バッファサイズ
				desc.bufferStride = 0;   //!< ストライブ幅
				desc.bufferFlags;    //!< バッファフラグ
				desc.bindFlags = BindFlag::PixelShaderResource;      //!< バインドフラグ

				buffer = Buffer(desc);
				buffer.setName(TC("TestBuffer"));
				OB_CHECK_ASSERT_EXPR(buffer);

				Color color = Color::red;
				buffer.updateDirect(sizeof(color), &color);
			}

			DescriptorTable dt(DescriptorHeapType::CBV_SRV_UAV, 1);
			dt.setResource(0, tex);

			DescriptorTable dt2(DescriptorHeapType::CBV_SRV_UAV, 1);
			dt2.setResource(0, buffer);

			CommandList cmdList;
			{
				CommandListDesc desc;
				desc.type = CommandListType::Graphic;
				cmdList = CommandList(desc);
				cmdList.setName(TC("TestCommandList"));
				OB_CHECK_ASSERT_EXPR(cmdList);
			}

			Mesh<Vert> mesh;
			mesh.appendQuad(
				{ Vec4(0,0,0,1),Vec2(0,0) },
				{ Vec4(0.5f,0,0,1),Vec2(1,0) },
				{ Vec4(0,0.5f,0,1),Vec2(0,1) },
				{ Vec4(0.5f,0.5f,0,1),Vec2(1,1) }
			);

			MeshBuffer meshBuffer(mesh);
			meshBuffer.setName(TC("Mesh1"));

			Mesh<Vert> mesh2;
			mesh2.appendQuad(
				{ Vec4(0.3f,0,0,1),Vec2(0,0) },
				{ Vec4(0.5f,0,0,1),Vec2(1,0) },
				{ Vec4(0,0.5f,0,1),Vec2(0,1) },
				{ Vec4(0.5f,0.5f,0,1),Vec2(1,1) }
			);

			MeshBuffer meshBuffer2(mesh2);
			meshBuffer2.setName(TC("Mesh2"));


			static s32 flag = 0;
			Random random;
			MSG msg = {};
			while (true) {

				cmdList.begin();

				cmdList.beginRender(rt);
				cmdList.clearColors();
				cmdList.setRootSignature(signature);
				cmdList.setPipelineState(pipeline);

				if ((flag / 60) % 2) {
					cmdList.setVertexBuffer(meshBuffer.getVertexBuffer());
					cmdList.setIndexBuffer(meshBuffer.getIndexBuffer());
				} else {
					cmdList.setVertexBuffer(meshBuffer2.getVertexBuffer());
					cmdList.setIndexBuffer(meshBuffer2.getIndexBuffer());
				}

				SetDescriptorTableParam params[] = {
					SetDescriptorTableParam(dt,0),
					SetDescriptorTableParam(dt2,1),
				};
				cmdList.setRootDesciptorTable(params, 2);

				{
					DrawIndexedParam param{};
					param.indexCount = 6;
					cmdList.drawIndexed(param);
				}

				cmdList.applySwapChain(swapChain, rt.getColorTexture(0));
				cmdList.end();

				// TODO コマンドの個別実行を許可する？
				cmdList.flush();

				graphic::System::Instance().update();

				// 表示を更新(Present)
				swapChain.update();

				if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				if (msg.message == WM_QUIT) {
					break;
				}

				
				Color color(random.get0_1(), random.get0_1(), random.get0_1(), 1);
				buffer.updateDirect(sizeof(color), &color);

				flag++;

			}
		}
	}


	return 0;
}