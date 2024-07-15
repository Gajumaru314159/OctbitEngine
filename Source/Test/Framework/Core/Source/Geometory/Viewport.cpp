//***********************************************************
//! @file
//! @brief		Capsule のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Geometry/Viewport.h>

using namespace ob::core;

TEST(Viewport, Constructor) {
	// デフォルトコンストラクタ
	{
		Viewport vp;
		EXPECT_EQ(vp.width(), 0);
		EXPECT_EQ(vp.height(), 0);
	}
	// 要素指定(ltrb)
	{
		f32 left = 1;
		f32 top = 2;
		f32 right = 3;
		f32 bottom = 4;
		Viewport vp(left, top, right, bottom);
		EXPECT_EQ(vp.left, left);
		EXPECT_EQ(vp.top, top);
		EXPECT_EQ(vp.right, right);
		EXPECT_EQ(vp.bottom, bottom);
	}
	// center,size指定
	{
		Vec2 center(1, 2);
		Vec2 size(3, 4);
		Viewport vp(center, size);
	}
}

TEST(Viewport, Getter) {
	// width
	{
		Viewport vp(1, 2, 3, 4);
		EXPECT_EQ(vp.width(), 2);
	}
	// height
	{
		Viewport vp(1, 2, 3, 4);
		EXPECT_EQ(vp.height(), 2);
	}
	// depth
	{
		Viewport vp(1, 2, 3, 4,0,10);
		EXPECT_EQ(vp.depth(), 10);
	}
	// aspect
	{
		Viewport vp(1, 2, 3, 4);
		EXPECT_EQ(vp.aspect(), 1.0f);
	}
}