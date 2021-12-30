//***********************************************************
//! @file
//! @brief		エントリー
//! @author		Gajumaru
//***********************************************************
#include <Runtime/Foundation/Log/Logger.h>

int main(int argc, char** argv)
{
    ob::Logger logger;

    IUTEST_INIT(&argc, argv);
    return IUTEST_RUN_ALL_TESTS();
}