#include <Windows.h>

#include <Framework/Core/Core.h>
#include <Framework/Graphic/All.h>

#include <Framework/Graphic/MeshData.h>

#include <Framework/Platform/Window.h>
#include <Framework/Platform/System.h>

#include <Framework/Input/All.h>
#include <Framework/Input/InputManager.h>

#include <Framework/Engine/Engine.h>

#include <Framework/Engine/InitContext.h>

#include <OBJ_Loader.h>
//-----------------------------------------------------------------
using namespace ob;

void Link_DirectX12();

void OctbitInit(ob::engine::InitContext& context) {
	Link_DirectX12();
}

int OctbitMain() {

	using namespace ob::graphic;

	{
		if (true) {
			LOG_INFO("Language :{}", (int)platform::System::GetLanguage());
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
			LOG_INFO("TimeSpan :{}", TimeSpan::Days(23.12534573));
			LOG_INFO("TimeSpan :{}", TimeSpan::Hours(23.12534573));
			LOG_INFO("TimeSpan :{}", TimeSpan::Minutes(23.12534573));
			LOG_INFO("TimeSpan :{}", TimeSpan::Seconds(23.12534573));
			LOG_INFO("TimeSpan :{}", TimeSpan::MilliSeconds(23.12534573));
			LOG_INFO("UUID     :{}", ob::UUID::Generate());
			LOG_INFO("Color    :{}", Color::Cyan);
			LOG_INFO("IntColor :{}", IntColor::Cyan);
			LOG_INFO("IntColor :{:#}", IntColor::Cyan);
			LOG_INFO("HSV	   :{}", HSV(Color::Cyan));
			LOG_INFO("HSV	   :{}", HSV::Cyan.toColor());
		}

		{

			// ウィンドウ生成
			platform::WindowDesc windowDesc;
			windowDesc.title = TC("Graphic Test");
			platform::Window window(windowDesc);

			
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
				FileStream fs(L"../../../Asset/Texture/test.dds");
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
				vssrc.append(TC("struct VsIn {													\n"));
				vssrc.append(TC("  float4 pos:POSITION;											\n"));
				vssrc.append(TC("  float4 normal:NORMAL;										\n"));
				vssrc.append(TC("  float2 uv:TEXCOORD;											\n"));
				vssrc.append(TC("};																\n"));
				vssrc.append(TC("struct VsOut {													\n"));
				vssrc.append(TC("  float4 pos:SV_POSITION;										\n"));
				vssrc.append(TC("  float4 normal:NORMAL;										\n"));
				vssrc.append(TC("  float2 uv:TEXCOORD;											\n"));
				vssrc.append(TC("};																\n"));
				vssrc.append(TC("VsOut VS_Main(VsIn i) {										\n"));
				vssrc.append(TC("    VsOut o;													\n"));
				vssrc.append(TC("    o.pos = mul(g_mtx,i.pos);									\n"));
				vssrc.append(TC("    o.uv = i.uv;												\n"));
				vssrc.append(TC("    o.normal = i.normal;										\n"));
				vssrc.append(TC("    return o;													\n"));
				vssrc.append(TC("}																\n"));
				String pssrc;
				pssrc.append(TC("SamplerState g_mainSampler:register(s0);						\n"));
				pssrc.append(TC("Texture2D g_mainTex:register(t0);								\n"));
				pssrc.append(TC("struct PsIn {													\n"));
				pssrc.append(TC("  float4 pos:SV_POSITION;										\n"));
				pssrc.append(TC("  float4 normal:NORMAL;										\n"));
				pssrc.append(TC("  float2 uv:TEXCOORD;											\n"));
				pssrc.append(TC("};																\n"));
				pssrc.append(TC("struct PsOut {													\n"));
				pssrc.append(TC("	float4 color0:SV_TARGET0;									\n"));
				pssrc.append(TC("};																\n"));
				pssrc.append(TC("PsOut PS_Main(PsIn i){											\n"));
				pssrc.append(TC("    PsOut o;													\n"));
				pssrc.append(TC("    float4 color = g_mainTex.Sample(g_mainSampler,i.uv);		\n"));
				pssrc.append(TC("    color.xyz*=abs(dot(i.normal.xyz,float3(0,0,1)));			\n"));
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

			// 定数バッファ
			struct CBuf {
				Matrix matrix = Matrix::Identity;
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
				if (Loader.LoadFile("../../../Asset/Model/monky.obj")) {
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
				}
			}


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


			//------ループ-----

			Vec3 pos(0, 0, -10);
			Rot rot = Rot::Identity;


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

				//graphic::System::Get().update();
				GEngine->visit([](engine::IModule& m) {m.update(); });

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

				using namespace ob::input;
				InputManager::Get().update();

				auto speed = 4 / 60.f;
				if (input::Keyboard::K.pressed()) {
					speed *= 0.5f;
				}

				const auto rspd = 90 / 60.f;
				Rot r2(0, rot.y, 0);
				if (input::Keyboard::W.pressed()) {
					pos += r2.front() * speed;
				}
				if (input::Keyboard::S.pressed()) {
					pos -= r2.front() * speed;
				}
				if (input::Keyboard::D.pressed()) {
					pos += r2.right() * speed;
				}
				if (input::Keyboard::A.pressed()) {
					pos -= r2.right() * speed;
				}
				if (input::Keyboard::LeftArrow.pressed()) {
					rot.y -= rspd;
				}
				if (input::Keyboard::RightArrow.pressed()) {
					rot.y += rspd;
				}
				if (input::Keyboard::UpArrow.pressed()) {
					rot.x -= rspd;
				}
				if (input::Keyboard::DownArrow.pressed()) {
					rot.x += rspd;
				}
				rot.x = Math::Clamp(rot.x, -90.f, 90.f);


				cbuf.matrix = Matrix::Perspective(60, 8.f / 6.f, 0.01f, 100.0f) * Matrix::TRS(pos, rot, Vec3::One).inverse() * Matrix::Rotate(0, 180, 0);
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