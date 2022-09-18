//***********************************************************
//! @file
//! @brief		ウィンドウマネージャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Platform/Window/WindowManager.h>

namespace ob::platform {

    WindowManager::WindowManager() = default;
    WindowManager::~WindowManager() = default;


    //@―---------------------------------------------------------------------------
    //! @brief  メインウィンドウを設定
    //@―---------------------------------------------------------------------------
    void WindowManager::setMainWindow(const Window& window) {
        if (m_mainWindow) {
            LOG_WARNING("ウィンドウ[{}]が既にメインウィンドウに設定されているためメインウィンドウを変更できませんでした。",m_mainWindow.getTitle());
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

}// namespace ob::platform

