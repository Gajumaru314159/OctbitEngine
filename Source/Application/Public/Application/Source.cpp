#include <Framework/Platform/Window/Window.h>
#include <Framework/Platform/Module/ModuleManager.h>
#include <Framework/Platform/System/PlatformSystem.h>
#include <Framework/Graphic/System.h>
#include <Framework/Graphic/SwapChain.h>
#include <Framework/Graphic/RenderTarget.h>
#include <Framework/Graphic/RootSignature.h>
#include <Framework/Graphic/Buffer.h>
#include <Framework/Graphic/Shader.h>
#include <Framework/Graphic/PipelineState.h>
#include <Framework/Graphic/DescriptorTable.h>
#include <Framework/Graphic/CommandList.h>
#include <Framework/Graphic/MeshData.h>
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
#include <Framework/Core/Graphic/HSV.h>

#include <Framework/Input/InputManager.h>
#include <Framework/Input/Keyboard.h>
#include <Framework/Input/Mouse.h>

#include <OBJ_Loader.h>

using namespace ob;

int main() {
	using namespace ob::graphic;


	Logger::Instance();
	platform::PlatformSystem::Instance();
	{
		if (true) {
			LOG_INFO("Vec2     :{}", Vec2(1, 2));
			LOG_INFO("Vec3     :{}", Vec3(1.5235223f, 2.11f, -0.01f));
			LOG_INFO("Vec4     :{}", Vec4(3.1415f, 2.235e6f, -0.01f, -123.5e-5f));
			LOG_INFO("IntVec2  :{}", IntVec2(4, 12));
			LOG_INFO("IntVec3  :{}", IntVec3(474, 124, 12));
			LOG_INFO("IntVec4  :{}", IntVec4(45, 1472, 313, 1444));
			LOG_INFO("Quat     :{}", Quat::Identity);
			LOG_INFO("Rot(Deg) :{}", Rot(180, 90, 45));
			LOG_INFO("Rot(Rad) :{:.3}", Rot(180, 90, 45));
			LOG_INFO("DateTime :{}", DateTime::Now());
			LOG_INFO("Duration :{}", Duration::Days(23.12534573));
			LOG_INFO("Duration :{}", Duration::Hours(23.12534573));
			LOG_INFO("Duration :{}", Duration::Minutes(23.12534573));
			LOG_INFO("Duration :{}", Duration::Seconds(23.12534573));
			LOG_INFO("Duration :{}", Duration::MilliSeconds(23.12534573));
			LOG_INFO("UUID     :{}", ob::UUID::Generate());
			LOG_INFO("Color    :{}", Color::Cyan);
			LOG_INFO("IntColor :{}", IntColor::Cyan);
			LOG_INFO("IntColor :{:#}", IntColor::Cyan);
			LOG_INFO("HSV	   :{}", HSV(Color::Cyan));
			LOG_INFO("HSV	   :{}", HSV::Cyan.toColor());
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


			input::InputManager::Instance();

			// スワップチェイン
			SwapChain swapChain;
			{
				SwapchainDesc desc;
				desc.window = window;
				swapChain = SwapChain(desc);
				swapChain.setName(TC("MainWindow"));
				OB_CHECK_ASSERT_EXPR(swapChain);
			}

			RenderTarget rt;
			{
				RenderTargetDesc desc;
				desc.size = { 640,480 };
				desc.colors.push_back({ swapChain.getDesc().format, Color::Gray });
				desc.depth.push_back({ TextureFormat::D32, 1, 0 });

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
				vssrc.append(TC("cbuffer Param : register(b0) {									\n"));
				vssrc.append(TC("  float4x4 g_mtx;												\n"));
				vssrc.append(TC("  float4x4 g_mtx2;												\n"));
				vssrc.append(TC("  float4 g_col;												\n"));
				vssrc.append(TC("};																\n"));
				vssrc.append(TC("struct Output {float4 pos:SV_POSITION;float4 normal:NORMAL;float2 uv:TEXCOORD;};	\n"));
				vssrc.append(TC("Output VS_Main(float4 pos : POSITION ,float4 normal : NORMAL,float2 uv : TEXCOORD) {	\n"));
				vssrc.append(TC("    Output o;													\n"));
				vssrc.append(TC("    o.pos = mul(g_mtx,pos);									\n"));
				vssrc.append(TC("    o.uv = uv;													\n"));
				vssrc.append(TC("    o.normal = normal;													\n"));
				vssrc.append(TC("    return o;													\n"));
				vssrc.append(TC("}																\n"));
				String pssrc;
				pssrc.append(TC("SamplerState g_mainSampler:register(s0);						\n"));
				pssrc.append(TC("Texture2D g_mainTex:register(t0);								\n"));
				pssrc.append(TC("struct PsInput {float4 pos:SV_POSITION;float4 normal:NORMAL;float2 uv:TEXCOORD;};	\n"));
				pssrc.append(TC("struct PsOutput {												\n"));
				pssrc.append(TC("		float4 color0:SV_TARGET0;								\n"));
				pssrc.append(TC("};																\n"));
				pssrc.append(TC("PsOutput PS_Main(PsInput i){									\n"));
				pssrc.append(TC("    PsOutput o=(PsOutput)0;									\n"));
				pssrc.append(TC("    float4 color = g_mainTex.Sample(g_mainSampler,i.uv);		\n"));
				pssrc.append(TC("    color.xyz*=abs(dot(i.normal.xyz,float3(0,0,1)));											\n"));
				pssrc.append(TC("    o.color0 = color;											\n"));
				pssrc.append(TC("    return o;													\n"));
				pssrc.append(TC("}																\n"));

				vs = VertexShader(vssrc);
				ps = PixelShader(pssrc);
				OB_CHECK_ASSERT_EXPR(vs && ps);
			}


			struct Vert {
				Vec4 pos;
				Vec4 normal;
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
					VertexAttribute(Semantic::Normal,offsetof(Vert,normal),Type::Float,4),
					VertexAttribute(Semantic::TexCoord,offsetof(Vert,uv),Type::Float,2),
				};
				desc.target = rt;
				desc.blend[0] = BlendDesc::AlphaBlend;
				desc.rasterizer.cullMode = CullMode::None;
				desc.depthStencil.depth.enable = true;

				pipeline = PipelineState(desc);
				pipeline.setName(TC("TestPipeline"));
				OB_CHECK_ASSERT_EXPR(pipeline);
			}

			struct CBuf {
				Matrix matrix = Matrix::Identity;
				Matrix matrix2=Matrix::Identity;
				Color color = Color::Red;
			} cbuf;
			Buffer buffer;
			{
				BufferDesc desc = BufferDesc::Constant(100, BindFlag::PixelShaderResource);
				buffer = Buffer(desc);
				buffer.setName(TC("TestBuffer"));
				OB_CHECK_ASSERT_EXPR(buffer);
				buffer.updateDirect(cbuf);
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


			MeshData<Vert> mesh;
			{
				// Initialize Loader
				objl::Loader Loader;

				// Load .obj File
				if (Loader.LoadFile("D:/My/Temp/monky.obj")) {
					for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
					{
						objl::Mesh& curMesh = Loader.LoadedMeshes[i];

						String name;
						StringEncoder::Encode(curMesh.MeshName, name);

						LOG_INFO("name={}",name);

						for (int j = 0; j < curMesh.Vertices.size(); j++)
						{
							auto pos = curMesh.Vertices[j].Position;
							auto uv = curMesh.Vertices[j].TextureCoordinate;
							auto normal = curMesh.Vertices[j].Normal;
							auto& vert = mesh.vertices.emplace_back();
							vert.pos = Vec4(pos.X, pos.Y, pos.Z, 1.0f);
							vert.normal = Vec4(normal.X, normal.Y, normal.Z,1.0f);
							vert.uv = Vec2(uv.X,uv.Y);
						}
						
						for (int j = 0; j < curMesh.Indices.size(); j ++)
						{
							mesh.indices.push_back(curMesh.Indices[j]);
						}
						break;
					}
				}
			}


			//MeshData<Vert> mesh;
			//mesh.append(
			//	{ Vec4(0,0,0,1),Vec2(0,0) },
			//	{ Vec4(0.5f,0,0,1),Vec2(1,0) },
			//	{ Vec4(0,0.5f,0,1),Vec2(0,1) },
			//	{ Vec4(0.5f,0.5f,0,1),Vec2(1,1) }
			//);

			Buffer vertexBuffer;
			Buffer indexBuffer;
			{
				auto desc = BufferDesc::Vertex<Vert>(mesh.vertices.size());
				vertexBuffer = Buffer(desc, BlobView(mesh.vertices));
				OB_CHECK_ASSERT_EXPR(vertexBuffer);
			}
			{
				auto desc = BufferDesc::Vertex<decltype(mesh)::index_type>(mesh.indices.size());
				indexBuffer = Buffer(desc, BlobView(mesh.indices));
				OB_CHECK_ASSERT_EXPR(indexBuffer);
			}

			Vec2 pos(0.0f, 0.0f);
			Random random;


			input::ButtonHandle hClick;
			input::Mouse::Right.bindDown(
				hClick,
				[]() {
					LOG_INFO("クリック");
				}
			);

			input::AxisHandle hPosX;
			input::AxisHandle hPosY;

			auto posFunc = [&pos, &window](f32 value) {
				auto screenPos = input::Mouse::GetPos();
				auto clientPos = window.getClientPoint({ (s32)screenPos.x,(s32)screenPos.y });
				pos.x = clientPos.x * 2.0f / window.getSize().width - 1;
				pos.y = -clientPos.y * 2.0f / window.getSize().height + 1;
				//LOG_TRACE("{}", pos);
			};

			input::Mouse::X.bind(hPosX, posFunc);
			input::Mouse::Y.bind(hPosY, posFunc);



			f32 t = 0.0f;
			MSG msg = {};
			while (true) {

				cmdList.begin();

				cmdList.beginRender(rt);
				cmdList.clearColors();
				cmdList.clearDepthStencil();
				{
					cmdList.setRootSignature(signature);
					cmdList.setPipelineState(pipeline);

					cmdList.setVertexBuffer(vertexBuffer);
					cmdList.setIndexBuffer(indexBuffer);

					SetDescriptorTableParam params[] = {
						SetDescriptorTableParam(dt,0),
						SetDescriptorTableParam(dt2,1),
					};
					cmdList.setRootDesciptorTable(params, 2);

					DrawIndexedParam param{};
					param.indexCount = mesh.indices.size();
					cmdList.drawIndexed(param);
				}
				cmdList.endRender();
				cmdList.applySwapChain(swapChain, rt.getColorTexture(0));
				cmdList.end();

				// TODO コマンドの個別実行を許可する？
				cmdList.flush();

				graphic::System::Instance().update();

				// CommandBufferにコマンドが積まれているとSwapChain解放に失敗する
				if (input::Keyboard::GetButton(input::Key::Escape).down())break;

				// 表示を更新(Present)
				swapChain.update();


				if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				if (msg.message == WM_QUIT) {
					break;
				}
				auto r = Matrix::Rotate(0, 90, 0);
				auto tr = Matrix::Translate(0, 0, 1);
				auto a = tr*r;
				auto p = a * Vec3(1, 0, 0);
				auto p2 = a * Vec3(0, 0, 1);
				auto p3 = a * Vec3(0,0,0);

				using namespace ob::input;
				InputManager::Instance().update();

				cbuf.matrix = MatrixHelper::CreatePerspective(60, 8.f / 6.f, 0.01f, 100.0f)* Matrix::Translate(0, 0, 10) * Matrix::Translate(0, 0, 10)* Matrix::Rotate(0, t, 0);
				cbuf.matrix.transpose();
				buffer.updateDirect(cbuf);
				t += 2.f;
			}

			// グラフィックオブジェクトはここで全て解放予約
		}
		// ModuleManager / graphic::System / platform::System が解放されるが順序が固定でないので要修正
		// graphic::System -> ModuleManager -> platform::System の順に解放する必要がある
		// 現状Singletonのstatic変数の解放順に依存している
	}


	return 0;
}