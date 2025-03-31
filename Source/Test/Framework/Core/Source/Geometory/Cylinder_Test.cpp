//***********************************************************
//! @file
//! @brief		Capsule のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Geometry/Cylinder.h>

using namespace ob::core;

TEST(Cylinder, Constructor)
{
    // デフォルトコンストラクタ
    {
        Cylinder cylinder;
    }
	// 始点、終点、半径指定
	{
		Vec3 pos1(1.0f, 2.0f, 3.0f);
		Vec3 pos2(4.0f, 5.0f, 6.0f);
		f32 radius = 2.5f;
		Cylinder cylinder(pos1, pos2, radius);
		EXPECT_EQ(cylinder.pos1, pos1);
		EXPECT_EQ(cylinder.pos2, pos2);
		EXPECT_EQ(cylinder.radius, radius);
	}
	// 中心、高さ、半径、クォータニオン指定
	{
		Vec3 center(1.0f, 2.0f, 3.0f);
		f32 height = 5.0f;
		f32 radius = 2.0f;
		Quat quat(0.0f, 0.0f, 0.0f, 1.0f);
		Cylinder cylinder(center, height, radius, quat);
		EXPECT_EQ(cylinder.pos1, Vec3(1.0f, 4.5f, 3.0f));
		EXPECT_EQ(cylinder.pos2, Vec3(1.0f, -0.5f, 3.0f));
		EXPECT_EQ(cylinder.radius, radius);
	}
	// 中心、高さ、半径、方向指定
	{
		Vec3 center(1.0f, 2.0f, 3.0f);
		f32 height = 5.0f;
		f32 radius = 2.0f;
		Vec3 direction(0.0f, 1.0f, 0.0f);
		Cylinder cylinder(center, height, radius, direction);
		EXPECT_EQ(cylinder.pos1, Vec3(1.0f, 4.5f, 3.0f));
		EXPECT_EQ(cylinder.pos2, Vec3(1.0f, -0.5f, 3.0f));
		EXPECT_EQ(cylinder.radius, radius);
	}
}

TEST(Cylinder, Getter) {
	// height
	{
		Vec3 pos1(1.0f, 2.0f, 3.0f);
		Vec3 pos2(1.0f, 5.0f, 3.0f);
		f32 radius = 2.5f;
		Cylinder cylinder(pos1, pos2, radius);
		EXPECT_FLOAT_EQ(cylinder.height(), 3.0f);
	}
	// direction
	{
		Vec3 pos1(1.0f, 2.0f, 3.0f);
		Vec3 pos2(1.0f, 5.0f, 3.0f);
		f32 radius = 2.5f;
		Cylinder cylinder(pos1, pos2, radius);
		EXPECT_EQ(cylinder.direction(), Vec3(0.0f, 1.0f, 0.0f));
	}
	// volume
	{
		Vec3 pos1(1.0f, 2.0f, 3.0f);
		Vec3 pos2(1.0f, 5.0f, 3.0f);
		f32 radius = 2.5f;
		Cylinder cylinder(pos1, pos2, radius);
		EXPECT_FLOAT_EQ(cylinder.volume(), radius * radius * Math::PI * 3.0f);
	}
}