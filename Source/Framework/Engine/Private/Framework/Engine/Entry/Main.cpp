//***********************************************************
//! @file
//! @brief		エントリ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Log/Logger.h>
#include <Framework/Engine/Main.h>

//@―---------------------------------------------------------------------------
//! @brief  プラットフォーム共通のエントリ
//@―---------------------------------------------------------------------------
int CommonMain() {

    ob::Logger::Instance();

    LOG_INFO("Entry OctbitMain()");
    auto result = OctbitMain();
    LOG_INFO("Leave OctbitMain()");

    return result;
}