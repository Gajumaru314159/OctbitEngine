//***********************************************************
//! @file
//! @brief		Capsule のテスト
//! @author		Gajumaru
//***********************************************************
#include <gtest/gtest.h>
#include <Framework/Core/Geometry/Size.h>

using namespace ob::core;

TEST(Size, Constructor) {
	// デフォルトコンストラクタ
	{
		Size size;
	}
	// 要素指定
	{
		f32 width = 1.0f;
		f32 height = 2.0f;
		Size size(width, height);
		EXPECT_EQ(size.width, width);
		EXPECT_EQ(size.height, height);
		EXPECT_EQ(size.depth, 0.0f);
	}
	// 要素指定(depth)
	{
		f32 width = 1.0f;
		f32 height = 2.0f;
		f32 depth = 3.0f;
		Size size(width, height, depth);
		EXPECT_EQ(size.width, width);
		EXPECT_EQ(size.height, height);
		EXPECT_EQ(size.depth, depth);
	}
}

TEST(Size,Operator) {
	// ==
	{
		Size size1(1.0f, 2.0f);
		Size size2(1.0f, 2.0f);
		EXPECT_TRUE(size1 == size2);
	}
	{
		Size size1(1.0f, 2.0f);
		Size size2(2.0f, 2.0f);
		EXPECT_FALSE(size1 == size2);
	}
	{
		Size size1(1.0f, 2.0f);
		Size size2(1.0f, 3.0f);
		EXPECT_FALSE(size1 == size2);
	}
	// !=
	{
		Size size1(1.0f, 2.0f);
		Size size2(1.0f, 2.0f);
		EXPECT_FALSE(size1 != size2);
	}
	{
		Size size1(1.0f, 2.0f);
		Size size2(2.0f, 2.0f);
		EXPECT_TRUE(size1 != size2);
	}
	{
		Size size1(1.0f, 2.0f);
		Size size2(1.0f, 3.0f);
		EXPECT_TRUE(size1 != size2);
	}
}

TEST(Size, Getter) {
	// area
	{
		Size size(1.0f, 2.0f);
		EXPECT_EQ(size.area(), 2.0f);
	}
	// volume
	{
		Size size(1.0f, 2.0f, 3.0f);
		EXPECT_EQ(size.volume(), 6.0f);
	}
}

TEST(Size, Normalize) {
	// normalize
	{
		Size size(1.0f, 2.0f, 3.0f);
		size.normalize();
		EXPECT_EQ(size.width, 1.0f);
		EXPECT_EQ(size.height, 2.0f);
		EXPECT_EQ(size.depth, 3.0f);
	}
}

TEST(Size, Member) {
	Size size(1.0f, 2.0f, 3.0f);
	EXPECT_EQ(size.width, 1.0f);
	EXPECT_EQ(size.height, 2.0f);
	EXPECT_EQ(size.depth, 3.0f);
}