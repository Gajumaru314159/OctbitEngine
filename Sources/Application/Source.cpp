#include <Framework/Platform/Window/Window.h>
#include <Framework/Platform/Module/ModuleManager.h>
#include <Framework/Platform/System/PlatformSystem.h>
#include <Framework/Graphic/System.h>
#include <Framework/Graphic/SwapChain.h>
#include <Framework/Graphic/RenderTexture.h>
#include <Framework/Graphic/Geometry.h>

#include <Windows.h>

using namespace ob;

int main() {
    using namespace ob::graphic;

    struct Vertex {
        Vec3 position;
        Vec2 uv;
        Color color;
    };

    VertexLayout layout
    {
        sizeof(Vertex),
        {
            {Semantic::Position ,Type::Float,3,offsetof(Vertex,position)},
            {Semantic::TexCoord ,Type::Float,2,offsetof(Vertex,uv)},
            {Semantic::Color    ,Type::Float,4,offsetof(Vertex,color)},
        }
    };

    Geometry<Vertex> geometry(layout);
    geometry.addQuad();


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


            MSG msg = {};
            while (true) {

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