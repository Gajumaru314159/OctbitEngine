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

        if (startup()) {

        }

    }


    //@―---------------------------------------------------------------------------
    //! @brief  必須モジュールを追加
    //@―---------------------------------------------------------------------------
    bool AppBase::startup() {

        //FileStream stream("module.txt");
        //BinaryReader reader(stream);
        //if (stream) {
        //    reader.read()
        //}

        // モジュールオプション設定

        //m_engine->set<

        //m_engine->add<SceneModule>();
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