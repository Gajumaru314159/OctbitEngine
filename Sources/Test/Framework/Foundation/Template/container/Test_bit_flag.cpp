//***********************************************************
//! @file
//! @brief		BitFlags テスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Foundation/Template/Container/BitFlags.h>
using namespace ob;


enum class Flags
{
    A= get_bit(0),
    B= get_bit(1),
    C= get_bit(2),
};


TEST(BitFlags, HasFlag) {
    ob::bit_flags<Flags> flag;

    flag.on(Flags::A);

    EXPECT_TRUE(flag.has(Flags::A));
    EXPECT_FALSE(flag.has(Flags::B));

    flag.off(Flags::A);

    EXPECT_FALSE(flag.has(Flags::A));
    EXPECT_FALSE(flag.has(Flags::B));

    flag.set(Flags::B, TRUE);

    EXPECT_FALSE(flag.has(Flags::A));
    EXPECT_TRUE(flag.has(Flags::B));
}



TEST(BitFlags, Cast) {
    ob::bit_flags<Flags> flag(3);

    EXPECT_TRUE(flag.has(Flags::A));
    EXPECT_TRUE(flag.has(Flags::B));
    EXPECT_FALSE(flag.has(Flags::C));
}