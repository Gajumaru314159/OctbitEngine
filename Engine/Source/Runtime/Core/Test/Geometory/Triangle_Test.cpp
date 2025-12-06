//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Geometry/Triangle.h>

using namespace ob::core;

TEST(Triangle, Constructor) {
	// デフォルトコンストラクタ
	{
		Triangle tri;
	}

	// 3点指定
	{
		Vec2 pos1(1, 2);
		Vec2 pos2(4, 6);
		Vec2 pos3(3, 5);
		Triangle tri(pos1, pos2, pos3);
	}
	// 3点指定
	{
		Vec3 pos1(1, 2, 3);
		Vec3 pos2(4, 6, 7);
		Vec3 pos3(3, 5, 6);
		Triangle tri(pos1, pos2, pos3);
	}
}

TEST(Triangle, Equals) {
	// ==
	{
		Triangle tri1(Vec2(1, 2), Vec2(4, 6), Vec2(3, 5));
		Triangle tri2(Vec2(1, 2), Vec2(4, 6), Vec2(3, 5));
		EXPECT_TRUE(tri1 == tri2);
	}
	{
		Triangle tri1(Vec2(1, 2), Vec2(4, 6), Vec2(3, 5));
		Triangle tri2(Vec2(1, 2), Vec2(4, 6), Vec2(3, 6));
		EXPECT_FALSE(tri1 == tri2);
	}
	{
		Triangle tri1(Vec2(1, 2), Vec2(4, 6), Vec2(3, 5));
		Triangle tri2(Vec2(1, 2), Vec2(4, 6), Vec2(4, 5));
		EXPECT_FALSE(tri1 == tri2);
	}
	// !=
	{
		Triangle tri1(Vec2(1, 2), Vec2(4, 6), Vec2(3, 5));
		Triangle tri2(Vec2(1, 2), Vec2(4, 6), Vec2(3, 5));
		EXPECT_FALSE(tri1 != tri2);
	}
	{
		Triangle tri1(Vec2(1, 2), Vec2(4, 6), Vec2(3, 5));
		Triangle tri2(Vec2(1, 2), Vec2(4, 6), Vec2(3, 6));
		EXPECT_TRUE(tri1 != tri2);
	}
	{
		Triangle tri1(Vec2(1, 2), Vec2(4, 6), Vec2(3, 5));
		Triangle tri2(Vec2(1, 2), Vec2(4, 6), Vec2(4, 5));
	}
}

TEST(Triangle, Getter) {
	//  normal
	{
		Triangle tri(Vec3(1, 2, 0), Vec3(4, 6, 0), Vec3(3, 5, 0));
		EXPECT_EQ(tri.normal(), Vec3(0, 0, 1));
	}
	//centerOfGravity
	{
		Triangle tri(Vec2(1, 2), Vec2(4, 6), Vec2(3, 5));
		EXPECT_EQ(tri.centerOfGravity(), Vec3(2.6666666f, 4.3333333f,0.0f));
	}
	// innerGravity
	{
		Triangle tri(Vec2(1, 2), Vec2(4, 6), Vec2(3, 5));
		EXPECT_EQ(tri.innerGravity(), Vec3(3.077559f, 4.9364167f,0.0f));
	}
	// circumcenter
	{
		Triangle tri(Vec2(1, 2), Vec2(4, 6), Vec2(3, 5));
		EXPECT_EQ(tri.circumcenter(), Vec3(12.5f, -3.5f,0.0f));
	}
	// area
	{
		Triangle tri(Vec2(0, 0), Vec2(2, 0), Vec2(0, 2));
		EXPECT_FLOAT_EQ(tri.area(), 2);
	}
	//perimeter
	{
		Triangle tri(Vec2(0, 0), Vec2(1,0), Vec2(0, 1));
		EXPECT_FLOAT_EQ(tri.perimeter(), 1 + 1 + Math::Sqrt(2));
	}
	// flip
	{
		Triangle tri(Vec2(1, 2), Vec2(4, 6), Vec2(3, 5));
		Triangle tri2 = tri;
		tri.flip();
		tri.flip();
		EXPECT_EQ(tri, tri2);
	}
}