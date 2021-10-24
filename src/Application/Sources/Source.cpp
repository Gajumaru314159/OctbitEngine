#include <Runtime/Platform/Platform.h>

#include <Windows.h>

using namespace ob::platform;

int main() {
    ob::Logger logger;

    WindowCreationDesc desc;
    desc.title = TEXT("Test");
    desc.fullscreen = false;
    desc.resizable = true;
    desc.clientSize.width = 640;
    desc.clientSize.height = 480;

    auto window = ob::MakeRef<Window>(desc);

    auto window2 = ob::MakeRef<Window>(desc);


    while (1) { /* メインループ */
        MSG msg{};
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

            if (msg.message == WM_QUIT) {
                break;
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

        }

    }
    //window.show();

    // window.Show();


    // window.Close();

    return 0;
}