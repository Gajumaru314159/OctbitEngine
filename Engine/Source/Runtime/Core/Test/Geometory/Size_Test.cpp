//***********************************************************
//! @file
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
		s32 width = 1;
		s32 height = 2;
		Size size(width, height);
		EXPECT_EQ(size.width, width);
		EXPECT_EQ(size.height, height);
		EXPECT_EQ(size.depth, 1);
	}
	// 要素指定(depth)
	{
		s32 width = 1;
		s32 height = 2;
		s32 depth = 3;
		Size size(width, height, depth);
		EXPECT_EQ(size.width, width);
		EXPECT_EQ(size.height, height);
		EXPECT_EQ(size.depth, depth);
	}
}

TEST(Size,Operator) {
	// ==
	{
		Size size1(1, 2);
		Size size2(1, 2);
		EXPECT_TRUE(size1 == size2);
	}
	{
		Size size1(1, 2);
		Size size2(2, 2);
		EXPECT_FALSE(size1 == size2);
	}
	{
		Size size1(1, 2);
		Size size2(1, 3);
		EXPECT_FALSE(size1 == size2);
	}
	// !=
	{
		Size size1(1, 2);
		Size size2(1, 2);
		EXPECT_FALSE(size1 != size2);
	}
	{
		Size size1(1, 2);
		Size size2(2, 2);
		EXPECT_TRUE(size1 != size2);
	}
	{
		Size size1(1, 2);
		Size size2(1, 3);
		EXPECT_TRUE(size1 != size2);
	}
}

TEST(Size, Getter) {
	// area
	{
		Size size(1, 2);
		EXPECT_EQ(size.area(), 2);
	}
	// volume
	{
		Size size(1, 2, 3);
		EXPECT_EQ(size.volume(), 6);
	}
}

TEST(Size, Normalize) {
	// normalize
	{
		Size size(1, 2, 3);
		size.normalize();
		EXPECT_EQ(size.width, 1);
		EXPECT_EQ(size.height, 2);
		EXPECT_EQ(size.depth, 3);
	}
}

TEST(Size, Member) {
	Size size(1, 2, 3);
	EXPECT_EQ(size.width, 1);
	EXPECT_EQ(size.height, 2);
	EXPECT_EQ(size.depth, 3);
}