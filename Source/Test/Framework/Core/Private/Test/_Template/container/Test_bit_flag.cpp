//***********************************************************
//! @file
//! @brief		BitFlags テスト
//! @author		Gajumaru
//***********************************************************
using namespace ob;
#include <Framework/Core/Template/Container/BitFlags.h>

enum class Flags
{
    A= get_bit(0),
    B= get_bit(1),
    C= get_bit(2),
};


TEST(BitFlags, HasFlag) {
    ob::BitFlags<Flags> flag;

    flag.on(Flags::A);

    CHECK_EQUAL(flag.has(Flags::A));
    EXPECT_FALSE(flag.has(Flags::B));

    flag.off(Flags::A);

    EXPECT_FALSE(flag.has(Flags::A));
    EXPECT_FALSE(flag.has(Flags::B));

    flag.set(Flags::B, TRUE);

    EXPECT_FALSE(flag.has(Flags::A));
    CHECK_EQUAL(flag.has(Flags::B));
}



TEST(BitFlags, Cast) {
    ob::BitFlags<Flags> flag(3);

    CHECK_EQUAL(flag.has(Flags::A));
    CHECK_EQUAL(flag.has(Flags::B));
    EXPECT_FALSE(flag.has(Flags::C));
}