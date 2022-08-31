//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Engine/Application.h>
#include <Framework/Engine/Engine.h>

namespace ob::engine {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    ApplicationBase::ApplicationBase() {

        if (startup()) {

        }

    }


    //@―---------------------------------------------------------------------------
    //! @brief  必須モジュールを追加
    //@―---------------------------------------------------------------------------
    bool ApplicationBase::startup() {
        addRequierModuule();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  必須モジュールを追加
    //@―---------------------------------------------------------------------------
    void ApplicationBase::addRequierModuule() {

        // OS
        // File
        // Reflection
        // Game
        //  GameInstance
        // Job
        // Debug
        // Config

        // MainThread

        // Graphic
        // Input
        // Sound
        


    }

}// namespace ob