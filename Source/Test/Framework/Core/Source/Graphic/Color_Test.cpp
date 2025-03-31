//***********************************************************
//! @file
//! @brief		Color のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Graphic/Color.h>

using namespace ob;

TEST(Color, Constructor)
{
	Color c1(0, 0, 0, 1);
	EXPECT_FLOAT_EQ(c1.r, 0.0f);
	EXPECT_FLOAT_EQ(c1.g, 0.0f);
	EXPECT_FLOAT_EQ(c1.b, 0.0f);
	EXPECT_FLOAT_EQ(c1.a, 1.0f);

	// RGB
	Color c2(0.0f, 0.5f, 1.0f);
	EXPECT_FLOAT_EQ(c2.r, 0.0f);
	EXPECT_FLOAT_EQ(c2.g, 0.5f);
	EXPECT_FLOAT_EQ(c2.b, 1.0f);
	EXPECT_FLOAT_EQ(c2.a, 1.0f);

	// RGBA
	Color c3(0.1f, 0.2f, 0.3f, 0.4f);
	EXPECT_FLOAT_EQ(c3.r, 0.1f);
	EXPECT_FLOAT_EQ(c3.g, 0.2f);
	EXPECT_FLOAT_EQ(c3.b, 0.3f);
	EXPECT_FLOAT_EQ(c3.a, 0.4f);

	// グレースケール
	Color c4(0.5f);
	EXPECT_FLOAT_EQ(c4.r, 0.5f);
	EXPECT_FLOAT_EQ(c4.g, 0.5f);
	EXPECT_FLOAT_EQ(c4.b, 0.5f);
	EXPECT_FLOAT_EQ(c4.a, 1.0f);

	// IntColor ※0~255なので0.5はつくれない
	IntColor ic(128, 64, 32 ,16);
	Color c5(ic);
	EXPECT_FLOAT_EQ(c5.r, 128.f/255.f);
	EXPECT_FLOAT_EQ(c5.g,  64.f / 255.f);
	EXPECT_FLOAT_EQ(c5.b,  32.f / 255.f);
	EXPECT_FLOAT_EQ(c5.a,  16.f / 255.f);

	// Vec3
	Vec3 v3(0.1f, 0.2f, 0.3f);
	Color c6(v3);
	EXPECT_FLOAT_EQ(c6.r, 0.1f);
	EXPECT_FLOAT_EQ(c6.g, 0.2f);
	EXPECT_FLOAT_EQ(c6.b, 0.3f);
	EXPECT_FLOAT_EQ(c6.a, 1.0f);

	// Vec4
	Vec4 v4(0.1f, 0.2f, 0.3f, 0.4f);
	Color c7(v4);
	EXPECT_FLOAT_EQ(c7.r, 0.1f);
	EXPECT_FLOAT_EQ(c7.g, 0.2f);
	EXPECT_FLOAT_EQ(c7.b, 0.3f);
	EXPECT_FLOAT_EQ(c7.a, 0.4f);

	Color fc(128 / 255.f, 64 / 255.f, 32 / 255.f, 16 / 255.f);
	IntColor c8 = fc.toIntColor();
	EXPECT_EQ(c8.r, 128);
	EXPECT_EQ(c8.g, 64);
	EXPECT_EQ(c8.b, 32);
	EXPECT_EQ(c8.a, 16);
}

TEST(Color, Equality)
{
	const Color c1(0.1f, 0.2f, 0.3f, 0.4f);
	const Color c2(0.1f, 0.2f, 0.3f, 0.4f);
	const Color c3(0.4f, 0.3f, 0.2f, 0.1f);

	EXPECT_TRUE(c1 == c2);
	EXPECT_TRUE(c2 == c1);
	EXPECT_FALSE(c1 == c3);
	EXPECT_FALSE(c3 == c1);
}

TEST(Color, Inequality) {
	const Color c1(0.1f, 0.2f, 0.3f, 0.4f);
	const Color c2(0.1f, 0.2f, 0.3f, 0.4f);
	const Color c3(0.4f, 0.3f, 0.2f, 0.1f);

	EXPECT_FALSE(c1 != c2);
	EXPECT_FALSE(c2 != c1);
	EXPECT_TRUE(c1 != c3);
	EXPECT_TRUE(c3 != c1);
}

TEST(Color, Operators)
{
	const Color c1(1.0f, 0.5f, 0.25f, 0.8f);
	const Color c2(0.5f, 0.25f, 0.75f, 0.8f);

	const Color sum = c1 + c2;
	EXPECT_FLOAT_EQ(sum.r, 1.5f);
	EXPECT_FLOAT_EQ(sum.g, 0.75f);
	EXPECT_FLOAT_EQ(sum.b, 1.0f);
	EXPECT_FLOAT_EQ(sum.a, 0.8f);

	const Color diff = c1 - c2;
	EXPECT_FLOAT_EQ(diff.r, 0.5f);
	EXPECT_FLOAT_EQ(diff.g, 0.25f);
	EXPECT_FLOAT_EQ(diff.b, -0.5f);
	EXPECT_FLOAT_EQ(diff.a, 0.8f);

	const Color prod = c1 * c2;
	EXPECT_FLOAT_EQ(prod.r, 0.5f);
	EXPECT_FLOAT_EQ(prod.g, 0.125f);
	EXPECT_FLOAT_EQ(prod.b, 0.1875f);
	EXPECT_FLOAT_EQ(prod.a, 0.64f);

	const Color scalarProd = c1 * 0.5f;
	EXPECT_FLOAT_EQ(scalarProd.r, 0.5f);
	EXPECT_FLOAT_EQ(scalarProd.g, 0.25f);
	EXPECT_FLOAT_EQ(scalarProd.b, 0.125f);
	EXPECT_FLOAT_EQ(scalarProd.a, 0.4f);

	const Color div = c1 / c2;
	EXPECT_FLOAT_EQ(div.r, 2.0f);
	EXPECT_FLOAT_EQ(div.g, 2.0f);
	EXPECT_FLOAT_EQ(div.b, 0.3333333f);
	EXPECT_FLOAT_EQ(div.a, 1.0f);
}

TEST(Color, Clamp) {
	const Color c(-1, 0, 2, 1);

	Color c1 = c;
	Color c2 = c;
	c1.clamp();
	c2.clamp01();
	EXPECT_EQ(c1, Color(0, 0, 2, 1));
	EXPECT_EQ(c2, Color(0, 0, 1, 1));
}

TEST(Color, Component) {
	{
		const Color c(-1, 0, 1, 2);

		EXPECT_EQ(c.minComponent(), -1);
		EXPECT_EQ(c.maxComponent(), 1);
	}
	{
		const Color c(0, -1, 2, 1);

		EXPECT_EQ(c.minComponent(), -1);
		EXPECT_EQ(c.maxComponent(), 2);
	}
	{
		const Color c(2,1,0,-1);

		EXPECT_EQ(c.minComponent(), 0);
		EXPECT_EQ(c.maxComponent(), 2);
	}
}