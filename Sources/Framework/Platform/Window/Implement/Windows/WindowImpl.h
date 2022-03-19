//***********************************************************
//! @file
//! @brief		ウィンドウ(Windows)
//! @author		Gajumaru
//***********************************************************
#pragma once
#ifdef OS_WINDOWS
#include <Windows.h>
#include "../IWindowImpl.h"

namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウ
    //@―---------------------------------------------------------------------------
    class WindowImpl : public IWindowImpl {
    public:

        WindowImpl(const WindowCreationDesc& params);
        virtual ~WindowImpl()override;


        //===============================================================
        // ウィンドウ操作
        //===============================================================
        void show()override;
        void close()override;

        void maximize()override;
        void minimize()override;
        void moveToCenter()override;
        void restoreSize()override;


        //===============================================================
        // 情報取得
        //===============================================================

        bool isValid()const noexcept override;
        bool isMainWindow()const override;
        WindowStates getState()const override;

        Point getScreenPoint(const Point& clientPoint)const override;
        Point getClientPoint(const Point& screenPoint)const override;

        void setTitle(StringView getTitle) override;
        const String& getTitle()const override;

        void setPosition(Point position) override;
        Point getPosition()const noexcept override;

        Size getSize()const override;

        void setMode(WindowMode mode) override;
        WindowMode getMode()const override;

        WindowStyle getStyle()const override;
        void setStyle(WindowStyle style) override;


        //===============================================================
        // イベント
        //===============================================================

        void addEventListener(WindowEventType type, const WindowEvent& event) override;

    public:

        //@―---------------------------------------------------------------------------
        //! @brief              HWNDを取得
        //@―---------------------------------------------------------------------------
        HWND getHWND() { return m_hWnd; }

    private:

        //@―---------------------------------------------------------------------------
        //! @brief              ウィンドウごとの Window Proceduer
        //@―---------------------------------------------------------------------------
        LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


    public:

        //@―---------------------------------------------------------------------------
        //! @brief              ウィンドウ位置を画面の中央にする
        //@―---------------------------------------------------------------------------
        static LRESULT CALLBACK staticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


    private:

        static const TCHAR* WINDOW_CLASS_NAME;      //!< WNDCLASSEXに登録するウィンドウクラス名
        static const TCHAR* PROPERTY_NAME;          //!< HWNDに結びつけるプロパティ名

        static atomic<s32> m_windowNum;             //!< 生成されたウィンドウの数

        WindowMode m_mode;

        HWND    m_hWnd;                             //!< ウィンドウハンドル
        const StringBase<TCHAR> m_className;        //!< ウィンドウクラス名
        s32     m_windowID;                         //!< ウィンドウごとに割り当てられるID

        String  m_windowTitle;

        bool    m_isActive;
        bool    m_isMinimized;


    };

}// namespace ob::platform
#endif // OS_WINDOWS