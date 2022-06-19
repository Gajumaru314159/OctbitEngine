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
#include <Framework/Core/Misc/Duration.h>
#include <Framework/Core/Misc/UUID.h>
#include <Framework/Core/Math/Periodic.h>

#include <Framework/Input/InputManager.h>
#include <Framework/Input/Keyboard.h>
#include <Framework/Input/Mouse.h>

using namespace ob;

int main() {
	using namespace ob::graphic;


	Logger::Instance();
	platform::PlatformSystem::Instance();
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
			LOG_INFO("Duration :{}", Duration::Days(23.12534573));
			LOG_INFO("Duration :{}", Duration::Hours(23.12534573));
			LOG_INFO("Duration :{}", Duration::Minutes(23.12534573));
			LOG_INFO("Duration :{}", Duration::Seconds(23.12534573));
			LOG_INFO("Duration :{}", Duration::MilliSeconds(23.12534573));
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


			// ウィンドウ生成
			platform::WindowCreationDesc windowDesc2;
			windowDesc2.title = TC("Graphic Test Sub");
			platform::Window window2(windowDesc2);

			LOG_INFO("Main Window={}",platform::Window::getMainWindow().getTitle());


			// スワップチェイン
			SwapChain swapChain;
			{
				SwapchainDesc desc;
				desc.window = window;
				swapChain = SwapChain(desc);
				swapChain.setName(TC("MainWindow"));
				OB_CHECK_ASSERT_EXPR(swapChain);
			}


			// スワップチェイン
			SwapChain swapChain2;
			{
				SwapchainDesc desc;
				desc.window = window2;
				swapChain2 = SwapChain(desc);
				swapChain2.setName(TC("SubWindow"));
				OB_CHECK_ASSERT_EXPR(swapChain2);
			}


			RenderTarget rt;
			{
				RenderTargetDesc desc;
				desc.size = { 640,480 };
				desc.colors = {
					ColorTextureDesc{swapChain.getDesc().format,Color::grey},
					ColorTextureDesc{swapChain.getDesc().format,Color::grey},
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
				vssrc.append(TC("\ncbuffer Param : register(b0) {								"));
				vssrc.append(TC("\n  float2 g_pos;												"));
				vssrc.append(TC("\n};															"));
				vssrc.append(TC("\nstruct Output {float4 pos:SV_POSITION;float2 uv:TEXCOORD;};"));
				vssrc.append(TC("\nOutput VS_Main(float4 pos : POSITION ,float2 uv : TEXCOORD) {"));
				vssrc.append(TC("\n    Output o;"));
				vssrc.append(TC("\n    o.pos = pos+float4(g_pos,0,0);"));
				vssrc.append(TC("\n    o.uv = uv;"));
				vssrc.append(TC("\n    return o;"));
				vssrc.append(TC("\n}"));
				String pssrc;
				pssrc.append(TC("\nSamplerState g_mainSampler:register(s0);						"));
				pssrc.append(TC("\nTexture2D g_mainTex:register(t0);							"));
				pssrc.append(TC("\nstruct PsInput {float4 pos:SV_POSITION;float2 uv:TEXCOORD;};	"));
				pssrc.append(TC("\nstruct PsOutput {											"));
				pssrc.append(TC("\n		float4 color0:SV_TARGET0;								"));
				pssrc.append(TC("\n		float4 color1:SV_TARGET1;								"));
				pssrc.append(TC("\n};															"));
				pssrc.append(TC("\nPsOutput PS_Main(PsInput i){									"));
				pssrc.append(TC("\n    PsOutput o=(PsOutput)0;								"));
				pssrc.append(TC("\n    float4 color = g_mainTex.Sample(g_mainSampler,i.uv);		"));
				pssrc.append(TC("\n    o.color0 = color;										"));
				pssrc.append(TC("\n    float gray = (color.x+color.y+color.z)/3.0f;				"));
				pssrc.append(TC("\n    o.color1 = float4(gray,gray,gray,1.0f);				"));
				pssrc.append(TC("\n    return o;												"));
				pssrc.append(TC("\n}															"));

				vs = VertexShader(vssrc);
				ps = PixelShader(pssrc);
				OB_CHECK_ASSERT_EXPR(vs && ps);
			}


			struct Vert {
				Vec4 pos;
				Vec2 uv;
			};

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
				desc.bufferSize = 101;     //!< バッファサイズ
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


			Vec2 pos(0.0f, 0.0f);
			Random random;
			MSG msg = {};

			auto prev = DateTime::Now();

			while (true) {

				auto next = DateTime::Now();
				//LOG_INFO("{}", 1.0f/Duration(prev,next).secondsF());
				prev = next;

				cmdList.begin();

				cmdList.beginRender(rt);
				cmdList.clearColors();
				cmdList.setRootSignature(signature);
				cmdList.setPipelineState(pipeline);

				cmdList.setVertexBuffer(meshBuffer.getVertexBuffer());
				cmdList.setIndexBuffer(meshBuffer.getIndexBuffer());

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
				cmdList.applySwapChain(swapChain2, rt.getColorTexture(1));
				cmdList.end();

				// TODO コマンドの個別実行を許可する？
				cmdList.flush();

				graphic::System::Instance().update();

				// 表示を更新(Present)
				swapChain.update();
				swapChain2.update();

				if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				if (msg.message == WM_QUIT) {
					break;
				}


				buffer.updateDirect(sizeof(pos), &pos);


				using namespace ob::input;
				InputManager::Instance().update();

				if (Keyboard::W.down()) {
					pos.setZero();
				}

				{
					using namespace ob::input;
					auto winSize = window.getSize();
					pos += Mouse::Pos.value()/Vec2(winSize.width*0.5f,winSize.height*-0.5f);
				}
			}

			// グラフィックオブジェクトはここで全て解放予約
		}
		// ModuleManager / graphic::System / platform::System が解放されるが順序が固定でないので要修正
		// graphic::System -> ModuleManager -> platform::System の順に解放する必要がある
		// 現状Singletonのstatic変数の解放順に依存している
	}


	return 0;
}