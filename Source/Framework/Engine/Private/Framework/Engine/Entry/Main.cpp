﻿//***********************************************************
//! @file
//! @brief		エントリ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Log/Logger.h>
#include <Framework/Engine/Entry/Main.h>
#include <Framework/Engine/Engine.h>
#include <Framework/Core/String/StringEncoder.h>
#include <Framework/Core/Utility/DI.h>

//@―---------------------------------------------------------------------------
//! @brief  プラットフォーム共通のエントリ
//@―---------------------------------------------------------------------------
int CommonMain() {

    LOG_INFO("OctbitInit()");

    // EngineRootMarkがあるフォルダをカレントパスに変更
    auto path = std::filesystem::current_path();
    while (path.has_parent_path()) {
        auto rootMarkPath = path / "EngineRootMark";
        if (std::filesystem::exists(rootMarkPath)) {
            std::filesystem::current_path(path);
            ob::String t;
            ob::StringEncoder::Encode(path.c_str(),t);
            LOG_INFO("カレントパスを{}に設定",t);
            break;
        }
        path = path.parent_path();
    }

    // 設定
    ob::engine::EngineConfig config;
    ob::core::ServiceInjector engineInjector;

    OctbitInit(config, engineInjector);

    LOG_TRACE("エンジン生成");
    // エンジン初期化
    ob::engine::Engine engine(std::move(config), engineInjector);
    GEngine = &engine;
    engine.startup();

    LOG_INFO("Entry OctbitMain()");
    auto result = OctbitMain();
    LOG_INFO("Leave OctbitMain()");

    return result;
}