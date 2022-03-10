#include <Framework/Platform/Window/Window.h>
using namespace ob;

int main() {
    
    platform::WindowCreationDesc desc;
    ob::platform::Window window(desc);
    window.Show();
    
    return 0;
}