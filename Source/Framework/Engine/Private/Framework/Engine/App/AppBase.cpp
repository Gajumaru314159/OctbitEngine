//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Engine/App.h>
#include <Framework/Engine/Engine.h>
#include <Framework/Core/File/FileStream.h>
#include <Framework/Core/File/BinaryReader.h>

namespace ob::engine {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    AppBase::AppBase() {
    }


    //@―---------------------------------------------------------------------------
    //! @brief  必須モジュールを追加
    //@―---------------------------------------------------------------------------
    bool AppBase::startup() {
        return true;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  必須モジュールを追加
    //@―---------------------------------------------------------------------------
    void AppBase::addRequierModuule() {

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