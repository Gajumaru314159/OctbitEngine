//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#ifdef OS_LINUX
#include "Window.h"
#include "WindowImpl.h"

#include <Framework/Platform/System/PlatformSystem.h>

static const char* s_proWindowProcedure = "OctbitWindowProp";


namespace ob {
    namespace platform {

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------

        WindowsWindow::WindowsWindow(const WindowDesc& params) :Window(params) {
            m_hWnd = nullptr;
            m_accelerator = nullptr;

            HINSTANCE hInst = (HINSTANCE)::GetModuleHandle(NULL);

            // ウィンドウクラスを設定
            WNDCLASSEXW	wcex = {
                sizeof(WNDCLASSEX),
                NULL,
                ::StaticWndProc,
                0, 0,
                hInst,
                NULL,
                ::LoadCursor(NULL, IDC_ARROW),
                (HBRUSH)(COLOR_WINDOW + 1),
                NULL,
                s_windowClassName,
                NULL
            };

            // ウィンドウクラスの登録
            ATOM wc = ::RegisterClassExW(&wcex);
            if (!wc) return;


            // ウィンドウモードのときのウィンドウスタイルの選択
            DWORD mWindowedStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
            if (params.resizable) {
                mWindowedStyle |= (WS_THICKFRAME | WS_MAXIMIZEBOX);
            }
            DWORD dwExStyle = 0;
            //if (params.win32IconResourceId == 0) {
            dwExStyle |= WS_EX_DLGMODALFRAME;	// アイコンの無いスタイル
        //}

            RECT clientRect;
            clientRect.left = 0;
            clientRect.top = 0;
            clientRect.right = params.clientSize.width;
            clientRect.bottom = params.clientSize.height;
            ::AdjustWindowRect(&clientRect, mWindowedStyle, FALSE);

            // ウィンドウの作成
            m_hWnd = ::CreateWindowExW(
                dwExStyle,
                s_windowClassName,
                params.title,
                mWindowedStyle,
                CW_USEDEFAULT, CW_USEDEFAULT,
                clientRect.right - clientRect.left, clientRect.bottom - clientRect.top,
                NULL, NULL, hInst, NULL);
            if (!m_hWnd) return;

            // アクセラレータの作成
            ACCEL accels[1] =
            {
                { FALT | FVIRTKEY, VK_RETURN, 0 }
            };
            m_accelerator = ::CreateAcceleratorTable(accels, 1);
            if (!m_accelerator) {
                UnregisterClassW(s_windowClassName, hInst);
                m_hWnd = nullptr;
                m_accelerator = nullptr;
                return;
            }

            abjustLocationCentering();

            // WM_PAINTが呼ばれないようにする
            ::ValidateRect(m_hWnd, 0);

            // ウィンドウハンドルとインスタンスを関連付ける
            BOOL r = ::SetPropW(m_hWnd, s_proWindowProcedure, this);
            if (r == FALSE) {
                UnregisterClassW(s_windowClassName, hInst);
                m_hWnd = nullptr;
                m_accelerator = nullptr;
                return;
            }


            ::ShowWindow(m_hWnd, SW_SHOW);
        }

        WindowsWindow::~WindowsWindow() {
            if (m_accelerator) {
                ::DestroyAcceleratorTable(m_accelerator);
                m_accelerator = NULL;
            }

            if (m_hWnd) {
                ::DestroyWindow(m_hWnd);
                m_hWnd = NULL;
            }
        }

        void WindowsWindow::setTitle(const Char* pTitle) {
        }

        Vec2 WindowsWindow::size() {
            return Vec2();
        }

        Vec2 WindowsWindow::getScreenPoint(const Vec2& clientPoint) {
            return Vec2();
        }

        Vec2 WindowsWindow::getClientPoint(const Vec2& screenPoint) {
            return Vec2();
        }

        void WindowsWindow::setCursor() {
        }



        bool WindowsWindow::isValid() {
            return m_hWnd != nullptr;
        }

        LRESULT WindowsWindow::wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
            if (msg == WM_DESTROY) {
                PostQuitMessage(0);
                return 0;
            }
            return DefWindowProc(hwnd, msg, wparam, lparam);
        }


        void WindowsWindow::abjustLocationCentering() {
            if (m_hWnd == nullptr)return;

            RECT rcWindow;
            ::GetWindowRect(m_hWnd, &rcWindow);

            // ディスプレイ全体のサイズを取得
            int sw = ::GetSystemMetrics(SM_CXSCREEN);
            int sh = ::GetSystemMetrics(SM_CYSCREEN);
            int x = (sw - (rcWindow.right - rcWindow.left)) / 2;
            int y = (sh - (rcWindow.bottom - rcWindow.top)) / 2;

            // サイズ変更せず移動だけ行う
            ::SetWindowPos(m_hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        }


        LRESULT CALLBACK WindowsWindow::staticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
            auto pWindow = reinterpret_cast<ob::platform::WindowsWindow*>(::GetPropA(hwnd, s_proWindowProcedure));

            if (pWindow) {
                return pWindow->wndProc(hwnd, msg, wparam, lparam);
            } else {
                return ::DefWindowProc(hwnd, msg, wparam, lparam);
            }
        }

    }// namespace platform
}// namespace ob

#endif // OS_LINUX