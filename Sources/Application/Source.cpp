#include <Framework/Platform/Window/Window.h>
#include <Framework/Platform/Module/ModuleManager.h>
#include <Framework/Platform/System/PlatformSystem.h>
#include <Framework/Graphic/System.h>
#include <Framework/Graphic/SwapChain.h>
#include <Framework/Graphic/RenderTexture.h>
#include <Framework/Graphic/Geometry.h>
#include <Framework/Graphic/RootSignature.h>
#include <Framework/Graphic/Shader.h>
#include <Framework/Graphic/PipelineState.h>

#include <Windows.h>

using namespace ob;

int main() {
    using namespace ob::graphic;


    Logger::Instance();
    platform::PlatformSystem::Instance().startup();
    {
        platform::ModuleManager::Instance();
        {
            SystemDesc sysDesc;
            sysDesc.api = graphic::GraphicAPI::D3D12;
            sysDesc.bufferCount = 2;
            graphic::System::Instance().initialize(sysDesc);

            platform::WindowCreationDesc windowDesc;
            windowDesc.title = TC("Graphic Test");
            platform::Window window(windowDesc);

            window.show();


            SwapchainDesc swapChainDesc;
            swapChainDesc.window = &window;
            SwapChain swapChain(swapChainDesc);

            TextureDesc texDesc[1];
            texDesc[0].type = TextureType::RenderTarget;
            texDesc[0].size = { 1280,720 };
            TextureDesc depth;
            depth.format = TextureFormat::D32;
            depth.type = TextureType::DeptthStencil;
            depth.size = { 1280,720 };
            RenderTexture rt(texDesc, depth);


            RootSignatureDesc desc(
                {
                    RootParameter(
                        {
                            DescriptorRange(DescriptorRangeType::CBV,1,0),
                            DescriptorRange(DescriptorRangeType::SRV,1,0),
                        }
                    )
                },
                {
                    StaticSamplerDesc(SamplerDesc(),0)
                }
             );
            RootSignature signature(desc);

            String vssrc;
            vssrc.append(TC("\nstruct Output {float4 pos:SV_POSITION;float2 uv:TEXCOORD;};"));
            vssrc.append(TC("\nOutput VS_Main(float4 pos : POSITION ,float2 uv : TEXCOORD) {"));
            vssrc.append(TC("\n    Output o;"));
            vssrc.append(TC("\n    o.pos = pos;"));
            vssrc.append(TC("\n    o.uv = uv;"));
            vssrc.append(TC("\n    return o;"));
            vssrc.append(TC("\n}"));
            String pssrc;
            pssrc.append(TC("\nTexture2D tex:register(t0);"));
            pssrc.append(TC("\nSamplerState smp:register(s0);"));
            pssrc.append(TC("\nstruct Output {float4 pos:SV_POSITION;float2 uv:TEXCOORD;};"));
            pssrc.append(TC("\nfloat4 PS_Main(Output i) : SV_TARGET{"));
            pssrc.append(TC("\n    return float4(tex.Sample(smp,i.uv));"));
            pssrc.append(TC("\n}"));
            VertexShader vs(vssrc);
            PixelShader ps(pssrc);
            OB_CHECK_ASSERT_EXPR(vs && ps);

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

                pipeline = PipelineState(desc);
            }


            MSG msg = {};
            while (true) {

                swapChain.update(rt.getTexture());
                //mat.setMatrix("WorldMatrix",mtx);
                //mat.setTexture("MainTex",tex);
                //rt.setPipeline(pipeline);
                //rt.setMaterial(mat);
                //rt.draw(shape);

                if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                if (msg.message == WM_QUIT) {
                    break;
                }

                //swapChain.update(rt);
            }
        }
    }


    return 0;
}