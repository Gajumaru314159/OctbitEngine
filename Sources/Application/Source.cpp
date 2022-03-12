#include <Framework/Platform/Window/Window.h>
#include <Framework/Platform/System/PlatformSystem.h>

using namespace ob;

int main() {
    platform::PlatformSystem sys;
    
    platform::PlatformSystem::ref().startup();

    auto& window =platform::PlatformSystem::ref().getMainWindow();
    if (window) {
        window->setTitle(TC("اللغة العربية"));
    }
    auto lang = platform::PlatformSystem::ref().getSystemLanguage();
    platform::PlatformSystem::ref().shutdown();
    return 0;
}