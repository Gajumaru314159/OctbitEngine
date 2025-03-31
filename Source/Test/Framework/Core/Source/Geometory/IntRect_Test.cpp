//***********************************************************
//! @file
//! @brief		Capsule のテスト
//! @author		Gajumaru
//***********************************************************
#include <gtest/gtest.h>
#include <Framework/Core/Geometry/IntRect.h>

using namespace ob::core;

TEST(IntRect, Constructor) {
	// デフォルトコンストラクタ
	{
		IntRect rect;
	}
	
	// LTRB指定
	{
		IntRect rect(1, 2, 3, 4);
	}
	// 2点から矩形を生成
	{
		IntVec2 pos1(1, 2);
		IntVec2 pos2(4, 6);
		IntRect rect(pos1, pos2);
	}
}

TEST(IntRect, Getter) {

	// tl
	{
		IntRect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.tl(), IntVec2(1, 2));
	}
	// tr
	{
		IntRect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.tr(), IntVec2(3, 2));
	}
	// bl
	{
		IntRect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.bl(), IntVec2(1, 4));
	}
	// br
	{
		IntRect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.br(), IntVec2(3, 4));
	}
	// center
	{
		IntRect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.center(), Vec2(2, 3));
	}
	// topCenter
	{
		IntRect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.topCenter(), Vec2(2, 2));
	}
	// bottomCenter
	{
		IntRect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.bottomCenter(), Vec2(2, 4));
	}
	// leftCenter
	{
		IntRect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.leftCenter(), Vec2(1, 3));
	}
	// rightCenter
	{
		IntRect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.rightCenter(), Vec2(3, 3));
	}
	// size
	{
		IntRect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.size(), IntVec2(2, 2));
	}
	// width
	{
		IntRect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.width(), 2);
	}
	// height
	{
		IntRect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.height(), 2);
	}
	// area
	{
		IntRect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.area(), 4);
	}
	// perimeter
	{
		IntRect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.perimeter(), 8);
	}
	// empty(false)
	{
		IntRect rect(1, 2, 3, 4);
		EXPECT_FALSE(rect.empty());
	}
	// empty(true)
	{
		IntRect rect;
		EXPECT_TRUE(rect.empty());
	}
}


TEST(IntRect, Member) {
	IntRect rect(1, 2, 3, 4);
	rect.left = 5;
	EXPECT_EQ(rect.left, 5);
	rect.top = 6;
	EXPECT_EQ(rect.top, 6);
	rect.right = 7;
	EXPECT_EQ(rect.right, 7);
	rect.bottom = 8;
	EXPECT_EQ(rect.bottom, 8);
}