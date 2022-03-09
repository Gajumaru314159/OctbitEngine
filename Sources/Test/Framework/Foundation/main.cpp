//***********************************************************
//! @file
//! @brief		エントリー
//! @author		Gajumaru
//***********************************************************
#include <Framework/Foundation/Log/Logger.h>
#include "gtest/iutest_switch.hpp"

int main(int argc, char** argv)
{
    ob::Logger logger;

    IUTEST_INIT(&argc, argv);
    return IUTEST_RUN_ALL_TESTS();
}