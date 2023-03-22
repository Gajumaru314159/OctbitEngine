//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#ifdef OS_WINDOWS
#include <Framework/Platform/Window/Implement/Windows/WindowImpl.h>
#include <Framework/Platform/Window.h>
#include <Framework/Core/String/StringEncoder.h>

namespace ob::platform {

	static TCHAR WINDOW_CLASS_NAME[] = TEXT("OctbitWindow");//!< WNDCLASSEXに登録するウィンドウクラス名
	static TCHAR PROPERTY_NAME[] = TEXT("OctbitWindowProp");//!< HWNDに結びつけるプロパティ名

	Atomic<s32> WindowImpl::m_windowNum = 0;

	//@―---------------------------------------------------------------------------
	//! @brief t     コンストラクタ
	//! 
	//! @details    生成情報を指定してウィンドウを生成する。
	//@―---------------------------------------------------------------------------
	WindowImpl::WindowImpl(const WindowDesc& desc)
		:m_className(Format(TEXT("{}_{}"), WINDOW_CLASS_NAME, m_windowNum))
		, m_hWnd(nullptr)
		, m_hParentWnd(nullptr)
	{
		m_hParentWnd = (HWND)Window::Main().getHandle();
		const bool hasParent = m_hParentWnd != nullptr;

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
		wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = m_className.c_str();
		wcex.hIconSm = NULL;

		// ウィンドウクラスの登録
		ATOM wc = ::RegisterClassEx(&wcex);
		if (!wc) return;


		// ウィンドウモードのときのウィンドウスタイルの選択
		//						タイトルバー　コントロールメニュー　最小化ボタン
		DWORD mWindowedStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		if (desc.resizable) {
			//					リサイズ用のフレーム　最大化ボタン
			mWindowedStyle |= WS_THICKFRAME | WS_MAXIMIZEBOX;
		}
		if (hasParent) {
			//mWindowedStyle |= WS_CHILD;
		}

		DWORD dwExStyle = 0;
		//if (params.win32IconResourceId == 0) {
		dwExStyle |= WS_EX_DLGMODALFRAME;	// アイコンの無いスタイル
		//}

		// ウィンドウサイズの計算
		RECT clientRect;
		clientRect.left = 0;
		clientRect.top = 0;
		clientRect.right = desc.clientSize.x;
		clientRect.bottom = desc.clientSize.y;
		::AdjustWindowRect(&clientRect, mWindowedStyle, FALSE);

		// 文字コードを変換
		m_title = desc.title;
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
			m_hParentWnd,                       // 親
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
	void WindowImpl::setPosition(Vec2 position) {
		if (!m_hWnd)return;
		auto size = getSize();
		::MoveWindow(m_hWnd, (int)position.x, (int)position.y, (int)size.x, (int)size.y, TRUE);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      ウィンドウの位置を取得する
	//@―---------------------------------------------------------------------------
	Vec2 WindowImpl::getPosition()const noexcept {
		if (!m_hWnd)return{ 0,0 };
		RECT rect;
		::GetWindowRect(m_hWnd, &rect);
		return Vec2((f32)rect.left, (f32)rect.top);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      ウィンドウのサイズを設定する
	//@―---------------------------------------------------------------------------
	void WindowImpl::setSize(Vec2 size) {
		if (!m_hWnd)return;

		RECT rw, rc;
		::GetWindowRect(m_hWnd, &rw);
		::GetClientRect(m_hWnd, &rc);

		int newWidth = (rw.right - rw.left) - (rc.right - rc.left) + (int)size.x;
		int newHeight = (rw.bottom - rw.top) - (rc.bottom - rc.top) + (int)size.y;

		::SetWindowPos(m_hWnd, NULL, 0, 0, newWidth, newHeight, SWP_NOMOVE | SWP_NOZORDER);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ウィンドウサイズを取得
	//@―---------------------------------------------------------------------------
	Vec2 WindowImpl::getSize()const {
		OB_ASSERT_EXPR(m_hWnd);
		RECT rect;
		::GetClientRect(m_hWnd, &rect);
		return Vec2{
			static_cast<f32>(rect.right - rect.left),
			static_cast<f32>(rect.bottom - rect.top)
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
	//! @brief      ウィンドウのハンドルを取得
	//@―---------------------------------------------------------------------------
	void* WindowImpl::getHandle() const {
		return m_hWnd;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      ウィンドウ・イベントのリスナを追加する
	//@―---------------------------------------------------------------------------
	void WindowImpl::addEventListener(WindowEventHandle& handle, WindowEventNotifier::delegate_type& func) {
		m_notifier.add(handle, func);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ウィンドウのタイトルを設定する
	//@―---------------------------------------------------------------------------
	void WindowImpl::setTitle(StringView title) {
		OB_ASSERT_EXPR(m_hWnd);
		m_title = title;
		WString titleW;
		StringEncoder::Encode(m_title, titleW);
		::SetWindowTextW(m_hWnd, titleW.c_str());
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ウィンドウのタイトルを取得する
	//@―---------------------------------------------------------------------------
	const String& WindowImpl::getTitle() const {
		return m_title;
	}



	//@―---------------------------------------------------------------------------
	//! @brief              スクリーン座標を取得
	//! 
	//! @details            クライアント座標をスクリーン座標に変換して取得する。
	//! @patam clientPoint  クライアント座標
	//! @return             スクリーン座標
	//@―---------------------------------------------------------------------------
	Vec2 WindowImpl::getScreenPoint(const Vec2& clientPoint) const {
		OB_ASSERT_EXPR(m_hWnd);
		POINT point;
		point.x = clientPoint.x;
		point.y = clientPoint.y;
		if (!::ClientToScreen(m_hWnd, &point))return Vec2::Zero;
		return Vec2(point.x, point.y);
	}


	//@―---------------------------------------------------------------------------
	//! @brief              クライアント座標を取得
	//! 
	//! @details            スクリーン座標をクライアント座標に変換して取得する。
	//! @patam screenPoint  スクリーン座標  
	//! @return             クライアント座標
	//@―---------------------------------------------------------------------------
	Vec2 WindowImpl::getClientPoint(const Vec2& screenPoint) const {
		OB_ASSERT_EXPR(m_hWnd);
		POINT point;
		point.x = screenPoint.x;
		point.y = screenPoint.y;
		if (!::ScreenToClient(m_hWnd, &point))return Vec2::Zero;
		return Vec2(point.x, point.y);
	}


	//@―---------------------------------------------------------------------------
	//! @brief              ウィンドウごとの Window Proceduer
	//@―---------------------------------------------------------------------------
	LRESULT WindowImpl::wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

		bool maximized = false;
		bool minimized = false;
		bool restored = false;

		WindowEventArgs args;
		args.type = WindowEventType::Unknown;

		switch (msg) {
		case WM_CREATE:
			//	ウインドウが作成されていることを示します。
			break;
		case WM_DESTROY:
			//	ウインドウが破棄されようとしていることを示します。
			PostQuitMessage(0);
			args.type = WindowEventType::Destroy;
			break;
		case WM_MOVE:
			//	ウインドウの位置が変更されたことを示します。
			args.type = WindowEventType::Move;
			args.oldPos = getPosition();
			args.newPos = { (f32)LOWORD(lparam),(f32)HIWORD(lparam) };
			break;
		case WM_ENTERSIZEMOVE:
			m_isSizing = true;
			break;
		case WM_EXITSIZEMOVE:
			m_isSizing = false;
			args.type = WindowEventType::Size;
			break;
		case WM_SIZE:
			//	ウインドウのサイズが変更されていることを示します。
			m_lastSize = { (f32)LOWORD(lparam),(f32)HIWORD(lparam) };
			if (wparam == SIZE_RESTORED)args.type = WindowEventType::Size;
			if (wparam == SIZE_MAXIMIZED)args.type = WindowEventType::Maximize;
			if (wparam == SIZE_MINIMIZED)args.type = WindowEventType::Minimize;
			break;
		case WM_ACTIVATE:
			//	アクティブ状態が変更されていることを示します。
			if (wparam)args.type = WindowEventType::Activate;
			if (!wparam)args.type = WindowEventType::Deactivate;
			// 記入力をリセットする必要がある？
			break;
		case WM_ENABLE:
			//	ウインドウの有効または無効の状態(デバイス入力)が変更されていることを示します。
			if (wparam)args.type = WindowEventType::Enable;
			if (!wparam)args.type = WindowEventType::Disable;
			break;
		case WM_CLOSE:
			//	コントロール・メニューの[クローズ]コマンドが選ばれました。デフォルトでWM_QUITを呼び出します。
			args.type = WindowEventType::Close;
			break;
		case WM_QUIT:
			//	アプリケーションを強制終了するよう要求します。
			break;
		case WM_SHOWWINDOW:
			//	ウインドウの表示または非表示の状態が変更されようとしていることを示します。
			if (wparam)args.type = WindowEventType::Show;
			if (!wparam)args.type = WindowEventType::Hide;
			break;


		case WM_SETFOCUS:
			//	ウインドウがキーボード・フォーカスを取得したことを示します。
			break;
		case WM_WININICHANGE:
			//	WIN.INIが変更されたことをアプリケーションに通知します。
			break;
		case WM_SETCURSOR:
			//	マウス カーソルの形状を設定するようウインドウに促します。
			break;
		case WM_MOUSEACTIVATE:
			//	非アクティブ ウインドウ内でマウスがクリックされたことを示します。
			break;
		case WM_CHILDACTIVATE:
			//	子ウインドウにアクティブであることを通知します。
			break;
		case WM_DISPLAYCHANGE:
			//	ディスプレイの解像度が変更されたことを示します。
			break;
		case WM_INPUT:
			//	RAW Input Device (キーボード/マウス/リモコン等) からの入力があったことを示します。
			break;
		case WM_CHAR:
		{
			// 文字入力
			std::wstring c(1,static_cast<wchar_t>(wparam));
			String tmp;
			StringEncoder::Encode(c, tmp);
			m_inputText += tmp;
			break;
		}
		default:break;
		}

		args.newSize = m_lastSize;
		args.isSizing = m_isSizing;

		if (args.type != WindowEventType::Unknown) {
			if (!m_handled) {
				ScopeValue sv(m_handled, true);
				m_notifier.invoke(args);
			}
		}

		if (msg == WM_CLOSE) {
			if (m_hParentWnd) {
				CloseWindow(hwnd);
				m_hWnd = nullptr;
				return 0;
			}
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