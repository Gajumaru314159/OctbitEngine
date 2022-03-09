﻿//***********************************************************
//! @file
//! @brief		ウィンドウ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Window.h"
#include "Implement/Windows/WindowImpl.h"
#include "Implement/Linux/WindowImpl.h"

namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ
    //! 
    //! @details    生成情報を指定してウィンドウを生成する。
    //@―---------------------------------------------------------------------------
    Window::Window(const WindowCreationDesc& desc) {
        m_impl = make_unique<WindowImpl>(desc);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウを表示する
    //@―---------------------------------------------------------------------------
    void Window::Show() {
        OB_REQUIRE(m_impl);
        m_impl->Show();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウを閉じる
    //@―---------------------------------------------------------------------------
    void Window::Close() {
        OB_REQUIRE(m_impl);
        m_impl->Close();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      ウィンドウ・イベントのリスナを追加する
    //@―---------------------------------------------------------------------------
    void Window::AddEventListener(WindowEventType type, const WindowEvent& event) {
        OB_REQUIRE(m_impl);
        m_impl->AddEventListener(type, event);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウのタイトルを設定する
    //@―---------------------------------------------------------------------------
    void Window::SetWindowTitle(const String& title) {
        OB_REQUIRE(m_impl);
        m_impl->SetWindowTitle(title);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウサイズの取得
    //@―---------------------------------------------------------------------------
    Size Window::GetSize()const {
        OB_REQUIRE(m_impl);
        return m_impl->GetSize();
    }


    //@―---------------------------------------------------------------------------
    //! @brief              スクリーン座標の取得
    //! 
    //! @details            クライアント座標をスクリーン座標に変換して取得する。
    //! @patam clientPoint  クライアント座標
    //! @return             スクリーン座標
    //@―---------------------------------------------------------------------------
    Point Window::GetScreenPoint(const Point& clientPoint)const {
        OB_REQUIRE(m_impl);
        return m_impl->GetScreenPoint(clientPoint);
    }


    //@―---------------------------------------------------------------------------
    //! @brief              クライアント座標の取得
    //! 
    //! @details            スクリーン座標をクライアント座標に変換して取得する。
    //! @patam screenPoint  スクリーン座標  
    //! @return             クライアント座標
    //@―---------------------------------------------------------------------------
    Point Window::GetClientPoint(const Point& screenPoint)const {
        OB_REQUIRE(m_impl);
        return m_impl->GetClientPoint(screenPoint);
    }


    //@―---------------------------------------------------------------------------
    //! @brief              妥当な状態か
    //@―---------------------------------------------------------------------------
    bool Window::IsValid()const noexcept {
        return static_cast<bool>(m_impl);
    }

}// namespace ob::platform