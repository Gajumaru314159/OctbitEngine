//***********************************************************
//! @file
//! @brief		Path のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/INI.h>
#include <filesystem>

using namespace ob;

TEST(INI, Test)
{
    auto filename = "engine.ini";
    std::filesystem::remove(filename);

    {
        INI ini(filename);
        EXPECT_TRUE(ini.empty());

        EXPECT_TRUE(ini["common"]["platform"].empty());
        ini["common"]["platform"] = "windows";
        EXPECT_EQ(ini["common"]["platform"],"windows");

        ini.save();
    }

    {
        INI ini;
        if (ini.load(filename)) {

            EXPECT_EQ(ini["common"]["platform"], "windows");

        }
    }
}