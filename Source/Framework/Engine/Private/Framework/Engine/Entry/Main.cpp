//***********************************************************
//! @file
//! @brief		エントリ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Log/Logger.h>
#include <Framework/Core/Utility/DI.h>
#include <Framework/Engine/Entry/Main.h>
#include <Framework/Engine/Engine.h>


//@―---------------------------------------------------------------------------
//! @brief  作業ディレクトリを設定
//@―---------------------------------------------------------------------------
static void SetupWorkingDirectory() {
    // EngineRootMarkがあるフォルダをカレントパスに変更
    auto path = std::filesystem::current_path();
    while (path.has_parent_path()) {
        auto rootMarkPath = path / "EngineRootMark";
        if (std::filesystem::exists(rootMarkPath)) {
            std::filesystem::current_path(path);
            ob::String t;
            ob::StringEncoder::Encode(path.u16string(), t);
            LOG_INFO("カレントパスを{}に設定", t);
            break;
        }
        path = path.parent_path();
    }
}


//@―---------------------------------------------------------------------------
//! @brief  プラットフォーム共通のエントリ
//@―---------------------------------------------------------------------------
int CommonMain() {

    LOG_INFO("OctbitInit()");

    SetupWorkingDirectory();

    // 初期化
    ob::core::ServiceInjector engineInjector;
    OctbitInit(engineInjector);

    // エンジン生成
    ob::engine::Engine engine(engineInjector);

    LOG_INFO("Entry OctbitMain()");
    auto result = OctbitMain();
    LOG_INFO("Leave OctbitMain()");

    return result;
}