//***********************************************************
//! @file
//! @brief		ウィンドウマネージャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Platform/WindowManager.h>
#include <Framework/Engine/Engine.h>

namespace ob::platform {

    WindowManager& WindowManager::Get() {
        static WindowManager* manager = nullptr;
        if (manager == nullptr) {
            manager = GEngine->get<WindowManager>();
        }
        OB_ASSERT_EXPR(manager);
        return *manager;
    }

    WindowManager::WindowManager() = default;
    WindowManager::~WindowManager() = default;


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