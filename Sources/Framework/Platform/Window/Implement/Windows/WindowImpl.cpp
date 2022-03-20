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

        // TODO ウィンドウモードを設定
        m_mode = desc.mode;

        // ウィンドウクラス名を設定

        HINSTANCE hInst = (HINSTANCE)::GetModuleHandle(NULL);

        // ウィンドウクラスを設定
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

        moveToCenter();

        // WM_PAINTが呼ばれないようにする
        ::ValidateRect(m_hWnd, NULL);

        // ウィンドウハンドルとインスタンスを関連付ける
        ::SetProp(m_hWnd, PROPERTY_NAME, this);

        // ウィンドウを表示
        show();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      デストラクタ
    //@―---------------------------------------------------------------------------
    WindowImpl::~WindowImpl() {
        close();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウを表示する
    //@―---------------------------------------------------------------------------
    void WindowImpl::show() {
        if (m_hWnd) {
            // ウィンドウを表示
            ::ShowWindow(m_hWnd, SW_SHOW);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウを閉じる
    //@―---------------------------------------------------------------------------
    void WindowImpl::close() {
        if (m_hWnd) {
            HINSTANCE hInst = (HINSTANCE)::GetModuleHandleW(NULL);
            ::DestroyWindow(m_hWnd);
            ::UnregisterClass(m_className.c_str(), hInst);
            m_hWnd = NULL;
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウを最大化する
    //@―---------------------------------------------------------------------------
    void WindowImpl::maximize() {
        if (m_hWnd) {
            ::ShowWindow(m_hWnd, SW_MAXIMIZE);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウを最小化する
    //@―---------------------------------------------------------------------------
    void WindowImpl::minimize() {
        if (m_hWnd) {
            ::ShowWindow(m_hWnd, SW_MINIMIZE);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウを最小化する
    //@―---------------------------------------------------------------------------
    void WindowImpl::moveToCenter() {
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
    //! @brief      ウィンドウサイズを元のサイズに戻す
    //@―---------------------------------------------------------------------------
    void WindowImpl::restoreSize() {
        if (m_hWnd) {
            ::ShowWindow(m_hWnd, SW_RESTORE);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief              妥当な状態か
    //! 
    //! @details            システムからウィンドウのクローズ処理が呼ばれた場合false
    //!                     を返すようになります。
    //@―---------------------------------------------------------------------------
    bool WindowImpl::isValid()const noexcept {
        return m_hWnd != nullptr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      メインウィンドウか
    //@―---------------------------------------------------------------------------
    bool WindowImpl::isMainWindow()const {
        if (!m_hWnd) return false;
        return ::GetParent(m_hWnd) == NULL;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      ウィンドウの状態を取得する
    //@―---------------------------------------------------------------------------
    WindowStates WindowImpl::getState()const {
        WindowStates status;
        status.clear();
        if (!m_hWnd)return status;

        // TODO フルスクリーン判定(HWNDにフルスクリーンはない？)
        if (::IsZoomed(m_hWnd))status.on(WindowState::Maximized);
        if (::IsIconic(m_hWnd))status.on(WindowState::Minimized);
        if (::GetFocus() == m_hWnd)status.on(WindowState::Focused);
        if (::GetFocus() == m_hWnd)status.on(WindowState::Focused);

        return status;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      ウィンドウの位置を設定する
    //@―---------------------------------------------------------------------------
    void WindowImpl::setPosition(Point position) {
        if (!m_hWnd)return;
        auto size = getSize();
        ::MoveWindow(m_hWnd, position.x, position.y, size.width, size.height, TRUE);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      ウィンドウの位置を取得する
    //@―---------------------------------------------------------------------------
    Point WindowImpl::getPosition()const noexcept {
        if (!m_hWnd)return{0,0};
        RECT rect;
        ::GetWindowRect(m_hWnd, &rect);
        return {rect.left,rect.top};
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウサイズを取得
    //@―---------------------------------------------------------------------------
    Size WindowImpl::getSize()const {
        OB_CHECK_ASSERT(m_hWnd);
        RECT rect;
        ::GetClientRect(m_hWnd, &rect);
        return Size{
            static_cast<s32>(rect.right - rect.left),
            static_cast<s32>(rect.bottom - rect.top)
        };
    }


    //@―---------------------------------------------------------------------------
    //! @brief      ウィンドウ・モードを設定する
    //@―---------------------------------------------------------------------------
    void WindowImpl::setMode(WindowMode mode) {
        switch (mode) {
        case WindowMode::Window:
            break;
        case WindowMode::FullScreen:
            break;
        case WindowMode::Borderless:
            break;
        default:
            OB_NOTIMPLEMENTED();
            break;
        }
        m_mode = mode;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      ウィンドウ・モードを取得する
    //@―---------------------------------------------------------------------------
    WindowMode WindowImpl::getMode()const {
        return m_mode;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      ウィンドウのスタイルを取得する
    //@―---------------------------------------------------------------------------
    WindowStyle WindowImpl::getStyle()const {
        // TODO ウィンドウスタイル設定
        return WindowStyle::Sizable;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      ウィンドウのスタイルを設定する
    //@―---------------------------------------------------------------------------
    void WindowImpl::setStyle(WindowStyle style) {
        // TODO ウィンドウスタイル設定
    }


    //@―---------------------------------------------------------------------------
    //! @brief      ウィンドウ・イベントのリスナを追加する
    //@―---------------------------------------------------------------------------
    void WindowImpl::addEventListener(WindowEventType type, const WindowEvent& event) {
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
        case WindowEventType::close:
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
    void WindowImpl::setTitle(StringView title) {
        OB_CHECK_ASSERT(m_hWnd);
        m_windowTitle = title;
        WString titleW;
        StringEncoder::Encode(m_windowTitle, titleW);
        ::SetWindowTextW(m_hWnd, titleW.c_str());
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウのタイトルを取得する
    //@―---------------------------------------------------------------------------
    const String& WindowImpl::getTitle() const{
        return m_windowTitle;
    }



    //@―---------------------------------------------------------------------------
    //! @brief              スクリーン座標を取得
    //! 
    //! @details            クライアント座標をスクリーン座標に変換して取得する。
    //! @patam clientPoint  クライアント座標
    //! @return             スクリーン座標
    //@―---------------------------------------------------------------------------
    Point WindowImpl::getScreenPoint(const Point& clientPoint) const {
        OB_CHECK_ASSERT(m_hWnd);
        POINT point;
        point.x = clientPoint.x;
        point.y = clientPoint.y;
        ::ClientToScreen(m_hWnd, &point);
        return Point();
    }


    //@―---------------------------------------------------------------------------
    //! @brief              クライアント座標を取得
    //! 
    //! @details            スクリーン座標をクライアント座標に変換して取得する。
    //! @patam screenPoint  スクリーン座標  
    //! @return             クライアント座標
    //@―---------------------------------------------------------------------------
    Point WindowImpl::getClientPoint(const Point& screenPoint) const {
        OB_CHECK_ASSERT(m_hWnd);
        POINT point;
        point.x = screenPoint.x;
        point.y = screenPoint.y;
        ::ScreenToClient(m_hWnd, &point);
        return Point();
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