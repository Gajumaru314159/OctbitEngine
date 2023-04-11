//***********************************************************
//! @file
//! @brief		ウィンドウマネージャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Platform/Window/WindowManager.h>

namespace ob::platform {

    static WindowManager* s_instance = nullptr;

    WindowManager* WindowManager::Get() {
        return s_instance;
    }

    WindowManager::WindowManager() {
        OB_ASSERT(s_instance == nullptr, "{}は既に生成されています。", TypeId::Get<WindowManager>().name());
        s_instance = this;
    }

    WindowManager::~WindowManager() {
        s_instance = nullptr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  メインウィンドウを設定
    //@―---------------------------------------------------------------------------
    void WindowManager::setMainWindow(const Window& window) {
        if (m_mainWindow) {
            LOG_WARNING("ウィンドウ[{}]が既にメインウィンドウに設定されているためメインウィンドウを変更できませんでした。", m_mainWindow.getTitle());
        } else {
            m_mainWindow = window;
        }
    }

    //@―---------------------------------------------------------------------------
    //! @brief  メインウィンドウを取得
    //@―---------------------------------------------------------------------------
    auto WindowManager::getMainWindow()->Window& {
        return m_mainWindow;
    }

}