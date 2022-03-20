#include <Framework/Platform/Window/Window.h>
#include <Framework/Platform/Module/ModuleManager.h>
#include <Framework/Platform/System/PlatformSystem.h>
#include <Framework/Graphic/System.h>

#include <Windows.h>

using namespace ob;

int main() {
    using namespace ob::graphic;
    
    platform::PlatformSystem sys;
    platform::PlatformSystem::ref().startup();
    {
        platform::ModuleManager moduleManager;
        Logger logger;
        moduleManager.ref();
        {
            graphic::System system(graphic::GraphicAPI::D3D12);
            system.ref();


            platform::WindowCreationDesc windowDesc;
            windowDesc.title = TC("Graphic Test");
            platform::Window window(windowDesc);

            window.show();


            SwapChain swapChain;
            SwapchainDesc swapChainDesc;
            swapChainDesc.window = &window;
            system.getDevice().createSwapChain(swapChain, swapChainDesc);

            MSG msg = {};
            while (true) {

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