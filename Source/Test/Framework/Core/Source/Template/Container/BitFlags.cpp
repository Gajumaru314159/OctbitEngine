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

TEST(BitFlags, Constructor)
{
    {
        BitFlags<Flags> flags;
        EXPECT_EQ(static_cast<unsigned int>(0), static_cast<unsigned int>(flags));
    }
    {
        BitFlags<Flags> flags(Flags::A);
        EXPECT_EQ(static_cast<unsigned int>(Flags::A), static_cast<unsigned int>(flags));
    }
    {
        BitFlags<Flags> flags(static_cast<unsigned int>(Flags::A));
        EXPECT_EQ(static_cast<unsigned int>(Flags::A), static_cast<unsigned int>(flags));
    }
    {
        BitFlags<Flags> flags(Flags::A);
        BitFlags<Flags> flags2(flags);
        EXPECT_EQ(static_cast<unsigned int>(Flags::A), static_cast<unsigned int>(flags2));
    }
    {
        BitFlags<Flags> flags(Flags::A);
        BitFlags<Flags> flags2;
        flags2 = flags;
        EXPECT_EQ(static_cast<unsigned int>(Flags::A), static_cast<unsigned int>(flags2));
    }
}

TEST(BitFlags, Operaor)
{
    {
        BitFlags<Flags> flags(Flags::A);
        flags |= Flags::B;
        EXPECT_EQ(static_cast<unsigned int>(Flags::A | Flags::B), static_cast<unsigned int>(flags));
    }
    {
        BitFlags<Flags> flags(Flags::A | Flags::B);
        BitFlags<Flags> flags2(Flags::A);
        flags &= flags2;
        EXPECT_EQ(static_cast<unsigned int>(Flags::A), static_cast<unsigned int>(flags));
    }
    {
        BitFlags<Flags> flags(Flags::A | Flags::B);
        BitFlags<Flags> flags2(Flags::A | Flags::C);
        flags ^= flags2;
        EXPECT_EQ(static_cast<unsigned int>(Flags::B | Flags::C), static_cast<unsigned int>(flags));
    }
    {
        BitFlags<Flags> flags(Flags::A | Flags::B);
        BitFlags<Flags> result = ~flags;
        EXPECT_EQ(static_cast<unsigned int>(~static_cast<unsigned int>(Flags::A | Flags::B)), static_cast<unsigned int>(result));
    }
    {
        BitFlags<Flags> flags(Flags::A);
        EXPECT_TRUE(static_cast<bool>(flags));
        EXPECT_FALSE(!flags);
    }

}

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