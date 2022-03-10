#include <Framework/Platform/Window/Window.h>
#include <Framework/Platform/System/PlatformSystem.h>

using namespace ob;

int main() {
    platform::PlatformSystem sys;
    
    platform::PlatformSystem::Get().Startup();

    auto& window =platform::PlatformSystem::Get().GetMainWindow();
    if (window) {
        window->SetWindowTitle(TC("اللغة العربية"));
    }
    auto lang = platform::PlatformSystem::Get().GetUserLanguage();
    platform::PlatformSystem::Get().Shutdown();
    return 0;
}