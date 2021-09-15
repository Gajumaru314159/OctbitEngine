//***********************************************************
//! @file
//! @brief		BitFlags テスト
//! @author		Gajumaru
//***********************************************************
#include <Foundation/Template/Container/bitflag.h>
using namespace ob;


enum class Flags
{
    A= BIT(0),
    B= BIT(1),
    C= BIT(2),
};


TEST(BitFlags, HasFlag) {
    ob::bitflag<Flags> flag;

    flag.Set(Flags::A);

    EXPECT_TRUE(flag.HasFlag(Flags::A));
    EXPECT_FALSE(flag.HasFlag(Flags::B));

    flag.Unset(Flags::A);

    EXPECT_FALSE(flag.HasFlag(Flags::A));
    EXPECT_FALSE(flag.HasFlag(Flags::B));

    flag.Set(Flags::B, TRUE);

    EXPECT_FALSE(flag.HasFlag(Flags::A));
    EXPECT_TRUE(flag.HasFlag(Flags::B));
}



TEST(BitFlags, Cast) {
    ob::bitflag<Flags> flag(3);

    EXPECT_TRUE(flag.HasFlag(Flags::A));
    EXPECT_TRUE(flag.HasFlag(Flags::B));
    EXPECT_FALSE(flag.HasFlag(Flags::C));
}