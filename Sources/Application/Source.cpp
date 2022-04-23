﻿#include <Framework/Platform/Window/Window.h>
#include <Framework/Platform/Module/ModuleManager.h>
#include <Framework/Platform/System/PlatformSystem.h>
#include <Framework/Graphic/System.h>
#include <Framework/Graphic/SwapChain.h>
#include <Framework/Graphic/RenderTexture.h>
#include <Framework/Graphic/Geometry.h>
#include <Framework/Graphic/RootSignature.h>

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
            depth.format = TextureFormat::D24S8;
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