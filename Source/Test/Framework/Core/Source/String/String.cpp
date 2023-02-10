//***********************************************************
//! @file
//! @brief		Box のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/String/String.h>
#include <utf8/checked.h>

using namespace ob;

TEST(String, Construct) {

    // サイズ / 中心
    {
        String text(TC("Text"));

        utf8::utf16to8(text);
        ASSERT_EQ(text.size(), 4);
    }

}