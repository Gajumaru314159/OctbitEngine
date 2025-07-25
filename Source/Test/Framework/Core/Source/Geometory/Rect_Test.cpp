//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <gtest/gtest.h>
#include <Framework/Core/Geometry/Rect.h>

using namespace ob::core;

TEST(Rect, Constructor) {
	// デフォルトコンストラクタ
	{
		Rect rect;
	}

	// LTRB指定
	{
		Rect rect(1, 2, 3, 4);
	}
	// 2点から矩形を生成
	{
		Vec2 pos1(1, 2);
		Vec2 pos2(4, 6);
		Rect rect(pos1, pos2);
	}
}

TEST(Rect, Getter) {

	// tl
	{
		Rect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.tl(), Vec2(1, 2));
	}
	// tr
	{
		Rect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.tr(), Vec2(3, 2));
	}
	// bl
	{
		Rect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.bl(), Vec2(1, 4));
	}
	// br
	{
		Rect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.br(), Vec2(3, 4));
	}
	// center
	{
		Rect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.center(), Vec2(2, 3));
	}
	// topCenter
	{
		Rect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.topCenter(), Vec2(2, 2));
	}
	// bottomCenter
	{
		Rect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.bottomCenter(), Vec2(2, 4));
	}
	// leftCenter
	{
		Rect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.leftCenter(), Vec2(1, 3));
	}
	// rightCenter
	{
		Rect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.rightCenter(), Vec2(3, 3));
	}
	// size
	{
		Rect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.size(), Vec2(2, 2));
	}
	// width
	{
		Rect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.width(), 2);
	}
	// height
	{
		Rect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.height(), 2);
	}
	// area
	{
		Rect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.area(), 4);
	}
	// perimeter
	{
		Rect rect(1, 2, 3, 4);
		EXPECT_EQ(rect.perimeter(), 8);
	}
	// empty(false)
	{
		Rect rect(1, 2, 3, 4);
		EXPECT_FALSE(rect.empty());
	}
}

TEST(Rect, Member) {
	Rect rect(1, 2, 3, 4);
	rect.left = 5;
	EXPECT_EQ(rect.left, 5);
	rect.top = 6;
	EXPECT_EQ(rect.top, 6);
	rect.right = 7;
	EXPECT_EQ(rect.right, 7);
	rect.bottom = 8;
	EXPECT_EQ(rect.bottom, 8);
}