//***********************************************************
//! @file
//! @brief		IntColor のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Graphic/IntColor.h>

using namespace ob;

TEST(IntColor, Constructor)
{
	IntColor c1(0, 0, 0, 255);
	EXPECT_FLOAT_EQ(c1.r, 0);
	EXPECT_FLOAT_EQ(c1.g, 0);
	EXPECT_FLOAT_EQ(c1.b, 0);
	EXPECT_FLOAT_EQ(c1.a, 255);

	// RGB
	IntColor c2(0, 128, 255);
	EXPECT_FLOAT_EQ(c2.r, 0);
	EXPECT_FLOAT_EQ(c2.g, 128);
	EXPECT_FLOAT_EQ(c2.b, 255);
	EXPECT_FLOAT_EQ(c2.a, 255);

	// RGBA
	IntColor c3(10,20,30,40);
	EXPECT_FLOAT_EQ(c3.r, 10);
	EXPECT_FLOAT_EQ(c3.g, 20);
	EXPECT_FLOAT_EQ(c3.b, 30);
	EXPECT_FLOAT_EQ(c3.a, 40);

	// グレースケール
	IntColor c4(128,64);
	EXPECT_FLOAT_EQ(c4.r, 128);
	EXPECT_FLOAT_EQ(c4.g, 128);
	EXPECT_FLOAT_EQ(c4.b, 128);
	EXPECT_FLOAT_EQ(c4.a, 64);

	// カラーコード
	IntColor c5 = IntColor::FromCode(0xF0804020,ColorCodeFormat::ARGB);
	EXPECT_FLOAT_EQ(c5.r, 0x80);
	EXPECT_FLOAT_EQ(c5.g, 0x40);
	EXPECT_FLOAT_EQ(c5.b, 0x20);
	EXPECT_FLOAT_EQ(c5.a, 0xF0);

	IntColor c6 = IntColor::FromCode(0xF0804020, ColorCodeFormat::ABGR);
	EXPECT_FLOAT_EQ(c6.r, 0x20);
	EXPECT_FLOAT_EQ(c6.g, 0x40);
	EXPECT_FLOAT_EQ(c6.b, 0x80);
	EXPECT_FLOAT_EQ(c6.a, 0xF0);

	IntColor c7 = IntColor::FromCode(0xF0804020, ColorCodeFormat::RGBA);
	EXPECT_FLOAT_EQ(c7.r, 0xF0);
	EXPECT_FLOAT_EQ(c7.g, 0x80);
	EXPECT_FLOAT_EQ(c7.b, 0x40);
	EXPECT_FLOAT_EQ(c7.a, 0x20);

	IntColor c8 = IntColor::FromCode(0xF0804020, ColorCodeFormat::BGRA);
	EXPECT_FLOAT_EQ(c8.r, 0x40);
	EXPECT_FLOAT_EQ(c8.g, 0x80);
	EXPECT_FLOAT_EQ(c8.b, 0xF0);
	EXPECT_FLOAT_EQ(c8.a, 0x20);
}

TEST(IntColor, Equality)
{
	const IntColor c1(1, 2, 3, 4);
	const IntColor c2(1, 2, 3, 4);
	const IntColor c3(4, 3, 2, 1);

	EXPECT_TRUE(c1 == c2);
	EXPECT_TRUE(c2 == c1);
	EXPECT_FALSE(c1 == c3);
	EXPECT_FALSE(c3 == c1);
}

TEST(IntColor, Inequality) {
	const IntColor c1(1, 2, 3, 4);
	const IntColor c2(1, 2, 3, 4);
	const IntColor c3(4, 3, 2, 1);

	EXPECT_FALSE(c1 != c2);
	EXPECT_FALSE(c2 != c1);
	EXPECT_TRUE(c1 != c3);
	EXPECT_TRUE(c3 != c1);
}

TEST(IntColor, Operators)
{
	const IntColor c1(7,11,13,17);
	const IntColor c2(1,2,3,5);

	const IntColor sum = c1 + c2;
	EXPECT_FLOAT_EQ(sum.r, 8);
	EXPECT_FLOAT_EQ(sum.g, 13);
	EXPECT_FLOAT_EQ(sum.b, 16);
	EXPECT_FLOAT_EQ(sum.a, 17);

	const IntColor diff = c1 - c2;
	EXPECT_FLOAT_EQ(diff.r, 6);
	EXPECT_FLOAT_EQ(diff.g, 9);
	EXPECT_FLOAT_EQ(diff.b, 10);
	EXPECT_FLOAT_EQ(diff.a, 17);

}

TEST(IntColor, Component) {
	{
		const IntColor c(1, 0, 1, 2);

		EXPECT_EQ(c.minComponent(), 0);
		EXPECT_EQ(c.maxComponent(), 1);
	}
	{
		const IntColor c(0, 1, 2, 3);

		EXPECT_EQ(c.minComponent(), 0);
		EXPECT_EQ(c.maxComponent(), 2);
	}
	{
		const IntColor c(2,1,0,3);

		EXPECT_EQ(c.minComponent(), 0);
		EXPECT_EQ(c.maxComponent(), 2);
	}
}