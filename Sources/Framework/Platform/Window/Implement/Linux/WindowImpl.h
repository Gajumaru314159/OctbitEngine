//***********************************************************
//! @file
//! @brief		ウィンドウ(Linux)
//! @author		Gajumaru
//***********************************************************
#pragma once
#ifdef OS_LINUX
#include "../IWindowImpl.h"

namespace ob {
    namespace platform {

        //@―---------------------------------------------------------------------------
        //! @brief  ウィンドウ
        //@―---------------------------------------------------------------------------
        class WindowImpl :public Window {
        public:
            WindowImpl(const WindowCreationParams& params);
            ~WindowImpl()override;

            virtual void setTitle(const Char* pTitle) override;
            virtual Size size() override;
            virtual Point getScreenPoint(const Point& clientPoint) override;
            virtual Point getClientPoint(const Point& screenPoint) override;
            virtual void setCursor() override;
            virtual bool isValid();

            LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

        public:

            static LRESULT CALLBACK staticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


        private:

            void abjustLocationCentering();


        private:

            HWND m_hWnd;
            HACCEL m_accelerator;

        };

    }// namespace platform
}// namespcae ob
#endif OS_LINUX