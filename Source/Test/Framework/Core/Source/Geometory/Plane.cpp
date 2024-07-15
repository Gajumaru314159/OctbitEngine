//***********************************************************
//! @file
//! @brief		Capsule のテスト
//! @author		Gajumaru
//***********************************************************
#include <gtest/gtest.h>
#include <Framework/Core/Geometry/Plane.h>

using namespace ob::core;

TEST(Plane, Constructor) {
	// デフォルトコンストラクタ
	{
		Plane plane;
	}
	// 平面の方程式の係数
	{
		f32 a = 1.0f;
		f32 b = 2.0f;
		f32 c = 3.0f;
		f32 d = 4.0f;
		Plane plane(a, b, c, d);
		EXPECT_EQ(plane.normal, Vec3(a, b, c).normalize());
	}
	// 法線、距離指定
	{
		Vec3 normal(1.0f, 2.0f, 3.0f);
		f32 distance = 4.0f;
		Plane plane(normal, distance);
		EXPECT_EQ(plane.normal, normal);
		EXPECT_EQ(plane.distance, distance);
	}
	// 平面上の一点と法線
	{
		Vec3 pos(1.0f, 2.0f, 3.0f);
		Vec3 normal(4.0f, 5.0f, 6.0f);
		Plane plane(pos, normal);
		EXPECT_EQ(plane.normal, normal.normalize());
	}
	// 3点から平面を生成
	{
		Vec3 pos1(1.0f, 2.0f, 3.0f);
		Vec3 pos2(4.0f, 5.0f, 6.0f);
		Vec3 pos3(7.0f, 8.0f, 9.0f);
		Plane plane(pos1, pos2, pos3);
		EXPECT_EQ(plane.normal, Vec3(0.0f, 0.0f, 0.0f));
		EXPECT_EQ(plane.distance, 0.0f);
	}
}

TEST(Plane, Flip) {
	// flipped
	{
		Plane plane(1.0f, 2.0f, 3.0f, 4.0f);
		Plane flipped = plane.flipped();
		EXPECT_EQ(flipped.normal, -plane.normal);
		EXPECT_FLOAT_EQ(flipped.distance, plane.distance);
	}
	// flip
	{
		Plane plane1(1.0f, 2.0f, 3.0f, 4.0f);
		Plane plane2(1.0f, 2.0f, 3.0f, 4.0f);
		plane1.flip();
		plane1.flip();
		EXPECT_EQ(plane1,plane2);
	}
}

TEST(Plane, Member) {
	Plane plane(1.0f, 2.0f, 3.0f, 4.0f);
	plane.normal = Vec3(5.0f, 6.0f, 7.0f);
	plane.distance = 8.0f;
	EXPECT_EQ(plane.normal, Vec3(5.0f, 6.0f, 7.0f));
	EXPECT_FLOAT_EQ(plane.distance, 8.0f);
}