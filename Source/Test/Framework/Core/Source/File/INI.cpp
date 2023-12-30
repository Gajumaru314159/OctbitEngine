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

TEST(INITest, BasicOperations) {
    INI ini;

    // セクションとキーを指定して値を設定
    ini["section1"]["key1"] = "value1";
    ini["section1"]["key2"] = "value2";
    ini["section2"]["key3"] = "value3";

    // セクションとキーが存在するかテスト
    ASSERT_TRUE(ini.has("section1"));
    ASSERT_TRUE(ini.has("section2"));
    ASSERT_TRUE(ini.has("section1", "key1"));
    ASSERT_TRUE(ini.has("section2", "key3"));
    ASSERT_FALSE(ini.has("section3"));
    ASSERT_FALSE(ini.has("section1", "key3"));

    // セクションとキーを指定して値を取得
    ASSERT_EQ(ini.get("section1", "key1"), "value1");
    ASSERT_EQ(ini.get("section2", "key3", "default_value"), "value3");
    ASSERT_EQ(ini.get("section2", "key4", "default_value"), "default_value");

    // セクションを削除して存在しないことを確認
    ini.remove("section1");
    ASSERT_FALSE(ini.has("section1"));

    // セクションとキーを指定して値を削除して存在しないことを確認
    ini.remove("section2", "key3");
    ASSERT_FALSE(ini.has("section2", "key3"));

    // INIをクリアして空であることを確認
    ini.clear();
    ASSERT_TRUE(ini.empty());
}


TEST(INITest, Parse) {

    INI ini;
    ini.parse(
        R"(
; これはコメント行です

[Section1]
Key1 = Value1
Key2 = Value2

[ Section2 ]
Key3 = Value3
Key4 = Value4

[EmptySection]

        )"
    );

    ASSERT_EQ(ini["Section1"]["Key1"], "Value1");
    ASSERT_EQ(ini["Section1"]["Key2"], "Value2");

    ASSERT_EQ(ini["Section2"]["Key3"], "Value3");
    ASSERT_EQ(ini["Section2"]["Key4"], "Value4");

}