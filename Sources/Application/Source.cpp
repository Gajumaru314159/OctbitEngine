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

#include <Windows.h>

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
                OB_CHECK_ASSERT_EXPR(swapChain);
            }

            Texture tex;
            {
                TextureDesc desc;
                desc.size = {512,512};
                desc.color.set(1, 0, 1);
                tex = Texture(desc);
            }

            RenderTarget rt;
            {
                RenderTargetDesc desc;
                desc.size = { 1280,720 };
                desc.colors = {
                    ColorTextureDesc{TextureFormat::RGBA8,Color::red},
                };

                desc.depth = {
                    DepthTextureDesc{TextureFormat::D32,0,0},
                };

                rt = RenderTarget(desc);
                OB_CHECK_ASSERT_EXPR(rt);
            }

            RootSignature signature;
            {
                RootSignatureDesc desc(
                    {
                        RootParameter(
                            {
                                DescriptorRange(DescriptorRangeType::SRV,1,0),
                            }
                        ),
                    },
                    {
                        StaticSamplerDesc(SamplerDesc(),0),
                    }
                );
                signature = RootSignature(desc);
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
                pssrc.append(TC("\nTexture2D g_mainTex;"));
                pssrc.append(TC("\nSamplerState g_mainSampler;"));
                pssrc.append(TC("\nstruct Output {float4 pos:SV_POSITION;float2 uv:TEXCOORD;};"));
                pssrc.append(TC("\nfloat4 PS_Main(Output i) : SV_TARGET{"));
                pssrc.append(TC("\n    return g_mainTex.Sample(g_mainSampler,i.uv);"));
                pssrc.append(TC("\n}"));
                vs= VertexShader(vssrc);
                ps= PixelShader(pssrc);
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
                OB_CHECK_ASSERT_EXPR(pipeline);
            }


            DescriptorTable dt(DescriptorHeapType::CBV_SRV_UAV, 1);
            dt.setResource(0,tex);
            
            CommandList cmdList;
            {
                CommandListDesc desc;
                desc.type = CommandListType::Graphic;
                cmdList = CommandList(desc);
                OB_CHECK_ASSERT_EXPR(cmdList);
            }

            Mesh<Vert> mesh;
            mesh.appendQuad(
                {Vec4(0,0,0,1),Vec2(0,0)},
                {Vec4(0.5,0,0,1),Vec2(1,0)},
                {Vec4(0,0.5,0,1),Vec2(0,1)},
                {Vec4(0.5,0.5,0,1),Vec2(1,1)}
            );

            MeshBuffer meshBuffer(mesh);

            Random random;
            MSG msg = {};
            while (true) {

                cmdList.begin();
                
                cmdList.setRenderTarget(rt);
                cmdList.clearColors();
                cmdList.setRootSignature(signature);
                cmdList.setPipelineState(pipeline);
                cmdList.setVertexBuffer(meshBuffer.getVertexBuffer());
                cmdList.setIndexBuffer(meshBuffer.getIndexBuffer());

                SetDescriptorTableParam params[] = {
                    SetDescriptorTableParam(dt,0),
                };
                cmdList.setRootDesciptorTable(params,1);
                cmdList.drawIndexedInstanced();
                //mat.setMatrix("WorldMatrix",mtx);
                //mat.setTexture("MainTex",tex);

                //cmdList.applySwapChain(swapChain, rt.getColorTexture(0));
                cmdList.end();
                cmdList.flush();
                //swapChain.update(rt.getTexture());

                graphic::System::Instance().update();

                swapChain.update(rt.getColorTexture());

                if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                if (msg.message == WM_QUIT) {
                    break;
                }

            }
        }
    }


    return 0;
}