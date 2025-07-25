//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Geometry/Margin.h>

using namespace ob::core;

TEST(Margin, Constructor) {
	// デフォルトコンストラクタ
	{
		Margin margin;
	}
	// 要素指定
	{
		f32 left = 1.0f;
		f32 top = 2.0f;
		f32 right = 3.0f;
		f32 bottom = 4.0f;
		Margin margin(left, top, right, bottom);
		EXPECT_EQ(margin.left, left);
		EXPECT_EQ(margin.top, top);
		EXPECT_EQ(margin.right, right);
		EXPECT_EQ(margin.bottom, bottom);
	}
}

TEST(Margin, Getter) {
	// tl
	{
		Margin margin(1.0f, 2.0f, 3.0f, 4.0f);
		EXPECT_EQ(margin.tl(), Vec2(1.0f, 2.0f));
	}
	// tr
	{
		Margin margin(1.0f, 2.0f, 3.0f, 4.0f);
		EXPECT_EQ(margin.tr(), Vec2(3.0f, 2.0f));
	}
	// bl
	{
		Margin margin(1.0f, 2.0f, 3.0f, 4.0f);
		EXPECT_EQ(margin.bl(), Vec2(1.0f, 4.0f));
	}
	// br
	{
		Margin margin(1.0f, 2.0f, 3.0f, 4.0f);
		EXPECT_EQ(margin.br(), Vec2(3.0f, 4.0f));
	}
	// horizontal
	{
		Margin margin(1.0f, 2.0f, 3.0f, 4.0f);
		EXPECT_FLOAT_EQ(margin.horizontal(), 4.0f);
	}
	// vertical
	{
		Margin margin(1.0f, 2.0f, 3.0f, 4.0f);
		EXPECT_FLOAT_EQ(margin.vertical(), 6.0f);
	}
	// size
	{
		Vec2 parent(10.0f, 20.0f);
		Margin margin(1.0f, 2.0f, 3.0f, 4.0f);
		EXPECT_EQ(margin.size(parent), Vec2(6.0f,14.0f));
	}
	// rect
	{
		Rect rect(1.0f, 2.0f, 13.0f, 14.0f);
		Margin margin(1.0f, 2.0f, 3.0f, 4.0f);
		EXPECT_EQ(margin.rect(rect),Rect(2.0f, 4.0f,10.0f, 10.0f));
	}
}

TEST(Margin, Member) {
	Margin margin(1.0f, 2.0f, 3.0f, 4.0f);
	margin.left = 5.0f;
	margin.top = 6.0f;
	margin.right = 7.0f;
	margin.bottom = 8.0f;
	EXPECT_EQ(margin.left, 5.0f);
	EXPECT_EQ(margin.top, 6.0f);
	EXPECT_EQ(margin.right, 7.0f);
	EXPECT_EQ(margin.bottom, 8.0f);
}