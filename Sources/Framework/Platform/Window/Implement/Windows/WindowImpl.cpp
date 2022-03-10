//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#ifdef OS_WINDOWS
#include "WindowImpl.h"
#include <Framework/Foundation/String/StringEncoder.h>

namespace ob::platform {

    const TCHAR* WindowImpl::WINDOW_CLASS_NAME = TEXT("OctbitWindow");
    const TCHAR* WindowImpl::PROPERTY_NAME = TEXT("OctbitWindowProp");

    atomic<s32> WindowImpl::m_windowNum = 0;

    //@―---------------------------------------------------------------------------
    //! @brief t     コンストラクタ
    //! 
    //! @details    生成情報を指定してウィンドウを生成する。
    //@―---------------------------------------------------------------------------
    WindowImpl::WindowImpl(const WindowCreationDesc& desc)
        :m_className(fmt::format(TEXT("{}_{}"), WINDOW_CLASS_NAME, m_windowNum)) {
        m_hWnd = nullptr;
        m_windowID = m_windowNum++;

        // ウィンドウクラス名の設定

        HINSTANCE hInst = (HINSTANCE)::GetModuleHandle(NULL);

        // ウィンドウクラスの設定
        WNDCLASSEX	wcex = {};
        wcex.cbSize = sizeof(WNDCLASSEXW);
        wcex.style = CS_HREDRAW | CS_VREDRAW;//縦横の再描画をする
        wcex.lpfnWndProc = WindowImpl::staticWndProc;
        wcex.hInstance = ::GetModuleHandle(nullptr);
        wcex.hIcon = NULL;
        wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = m_className.c_str();
        wcex.hIconSm = NULL;

        // ウィンドウクラスの登録
        ATOM wc = ::RegisterClassEx(&wcex);
        if (!wc) return;


        // ウィンドウモードのときのウィンドウスタイルの選択
        DWORD mWindowedStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
        if (desc.resizable) {
            mWindowedStyle |= WS_THICKFRAME | WS_MAXIMIZEBOX;
        }

        DWORD dwExStyle = 0;
        //if (params.win32IconResourceId == 0) {
        dwExStyle |= WS_EX_DLGMODALFRAME;	// アイコンの無いスタイル
        //}

        // ウィンドウサイズの計算
        RECT clientRect;
        clientRect.left = 0;
        clientRect.top = 0;
        clientRect.right = desc.clientSize.width;
        clientRect.bottom = desc.clientSize.height;
        ::AdjustWindowRect(&clientRect, mWindowedStyle, FALSE);

        // 文字コードを変換
        m_windowTitle = desc.title;
        WString titleW;
        StringEncoder::Encode(desc.title, titleW);

        // ウィンドウの作成
        m_hWnd = ::CreateWindowEx(
            dwExStyle,
            m_className.c_str(),
            titleW.c_str(),
            mWindowedStyle,
            CW_USEDEFAULT,                      // X
            CW_USEDEFAULT,                      // Y
            clientRect.right - clientRect.left, // Width
            clientRect.bottom - clientRect.top, // Height
            NULL,                               // 親
            NULL,                               // メニュー
            hInst,
            NULL);
        if (!m_hWnd) return;

        abjustLocationCentering();

        // WM_PAINTが呼ばれないようにする
        ::ValidateRect(m_hWnd, NULL);

        // ウィンドウハンドルとインスタンスを関連付ける
        ::SetProp(m_hWnd, PROPERTY_NAME, this);

        // ウィンドウを表示
        Show();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      デストラクタ
    //@―---------------------------------------------------------------------------
    WindowImpl::~WindowImpl() {
        Close();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウを表示する
    //@―---------------------------------------------------------------------------
    void WindowImpl::Show() {
        if (m_hWnd) {
            // ウィンドウを表示
            ::ShowWindow(m_hWnd, SW_SHOW);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウを閉じる
    //@―---------------------------------------------------------------------------
    void WindowImpl::Close() {
        if (m_hWnd) {
            HINSTANCE hInst = (HINSTANCE)::GetModuleHandleW(NULL);
            ::DestroyWindow(m_hWnd);
            ::UnregisterClass(m_className.c_str(), hInst);
            m_hWnd = NULL;
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief      ウィンドウ・イベントのリスナを追加する
    //@―---------------------------------------------------------------------------
    void WindowImpl::AddEventListener(WindowEventType type, const WindowEvent& event) {
        switch (type) {
        case WindowEventType::Move:
            break;
        case WindowEventType::Size:
            break;
        case WindowEventType::Activate:
            break;
        case WindowEventType::Deactivate:
            break;
        case WindowEventType::Focus:
            break;
        case WindowEventType::Close:
            break;
        case WindowEventType::Destroy:
            break;
        case WindowEventType::Minimize:
            break;
        case WindowEventType::Maximize:
            break;
        default:
            break;
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウのタイトルを設定する
    //@―---------------------------------------------------------------------------
    void WindowImpl::SetWindowTitle(StringView title) {
        OB_REQUIRE(m_hWnd);
        m_windowTitle = title;
        WString titleW;
        StringEncoder::Encode(m_windowTitle, titleW);
        ::SetWindowTextW(m_hWnd, titleW.c_str());
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウのタイトルを取得する
    //@―---------------------------------------------------------------------------
    const String& WindowImpl::GetWindowTitle() const noexcept{
        return m_windowTitle;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウサイズの取得
    //@―---------------------------------------------------------------------------
    Size WindowImpl::GetSize()const {
        OB_REQUIRE(m_hWnd);
        RECT rect;
        ::GetClientRect(m_hWnd, &rect);
        return Size{
            static_cast<s32>(rect.right - rect.left),
            static_cast<s32>(rect.bottom - rect.top)
        };
    }


    //@―---------------------------------------------------------------------------
    //! @brief              スクリーン座標の取得
    //! 
    //! @details            クライアント座標をスクリーン座標に変換して取得する。
    //! @patam clientPoint  クライアント座標
    //! @return             スクリーン座標
    //@―---------------------------------------------------------------------------
    Point WindowImpl::GetScreenPoint(const Point& clientPoint) const {
        OB_REQUIRE(m_hWnd);
        POINT point;
        point.x = clientPoint.x;
        point.y = clientPoint.y;
        ::ClientToScreen(m_hWnd, &point);
        return Point();
    }


    //@―---------------------------------------------------------------------------
    //! @brief              クライアント座標の取得
    //! 
    //! @details            スクリーン座標をクライアント座標に変換して取得する。
    //! @patam screenPoint  スクリーン座標  
    //! @return             クライアント座標
    //@―---------------------------------------------------------------------------
    Point WindowImpl::GetClientPoint(const Point& screenPoint) const {
        OB_REQUIRE(m_hWnd);
        POINT point;
        point.x = screenPoint.x;
        point.y = screenPoint.y;
        ::ScreenToClient(m_hWnd, &point);
        return Point();
    }


    //@―---------------------------------------------------------------------------
    //! @brief              妥当な状態か
    //! 
    //! @details            システムからウィンドウのクローズ処理が呼ばれた場合false
    //!                     を返すようになります。
    //@―---------------------------------------------------------------------------
    bool WindowImpl::IsValid()const noexcept {
        return m_hWnd != nullptr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief              ウィンドウごとの Window Proceduer
    //@―---------------------------------------------------------------------------
    LRESULT WindowImpl::wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

        switch (msg) {
        case WM_DESTROY:
            // ウインドウの破棄
            PostQuitMessage(0);
            return 0;
        case WM_ACTIVATE:
            // ウィンドウのアクティブ化・非アクティブ化
            m_isActive = wparam & 0xFFFF;
            m_isMinimized = (wparam >> 16) & 0xFFFF;
            break;
        case WM_CLOSE:
            // クローズコマンドが呼ばれた
            //Close();
            break;
        case WM_SIZE:
            // ウィンドウサイズが変更された
            break;
        }

        return DefWindowProc(hwnd, msg, wparam, lparam);
    }


    //@―---------------------------------------------------------------------------
    //! @brief              ウィンドウ位置を画面の中央にする
    //@―---------------------------------------------------------------------------
    void WindowImpl::abjustLocationCentering() {
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


    //@―---------------------------------------------------------------------------
    //! @brief              Window Proceduer
    //@―---------------------------------------------------------------------------
    LRESULT CALLBACK WindowImpl::staticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
        // ウィンドウに結びつけられたWindowImplを取得
        auto pWindow = reinterpret_cast<ob::platform::WindowImpl*>(::GetProp(hwnd, PROPERTY_NAME));

        if (pWindow) {
            return pWindow->wndProc(hwnd, msg, wparam, lparam);
        } else {
            return ::DefWindowProc(hwnd, msg, wparam, lparam);
        }
    }

}// namespace ob::platform
#endif // OS_WINDOWS