//***********************************************************
//! @file
//! @brief		エントリ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Log/Logger.h>
#include <Framework/Engine/Entry/Main.h>
#include <Framework/Engine/Engine.h>

//@―---------------------------------------------------------------------------
//! @brief  プラットフォーム共通のエントリ
//@―---------------------------------------------------------------------------
int CommonMain() {

    LOG_INFO("OctbitInit()");

    // 設定
    ob::engine::InitContext context;
    OctbitInit(context);

    LOG_TRACE("エンジン生成");
    // エンジン初期化
    ob::engine::Engine engine;
    GEngine = &engine;

    LOG_INFO("Entry OctbitMain()");
    auto result = OctbitMain();
    LOG_INFO("Leave OctbitMain()");

    return result;
}