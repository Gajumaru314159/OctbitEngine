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

    ASSERT_TRUE(flag.has(Flags::A));
    ASSERT_FALSE(flag.has(Flags::B));

    flag.off(Flags::A);

    ASSERT_FALSE(flag.has(Flags::A));
    ASSERT_FALSE(flag.has(Flags::B));

    flag.set(Flags::B, true);

    ASSERT_FALSE(flag.has(Flags::A));
    ASSERT_TRUE(flag.has(Flags::B));
}



TEST(BitFlags, Cast) {
    ob::BitFlags<Flags> flag(3);

    ASSERT_TRUE(flag.has(Flags::A));
    ASSERT_TRUE(flag.has(Flags::B));
    ASSERT_FALSE(flag.has(Flags::C));
}