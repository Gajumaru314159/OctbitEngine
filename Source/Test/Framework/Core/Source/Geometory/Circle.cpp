//***********************************************************
//! @file
//! @brief		Capsule のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Geometry/Capsule.h>

using namespace ob::core;

TEST(Capsule, DefaultConstructor)
{
    Capsule capsule;
}

TEST(Capsule, ConstructorWithPoints)
{
    Vec3 pos1(1.0f, 2.0f, 3.0f);
    Vec3 pos2(4.0f, 5.0f, 6.0f);
    f32 radius = 2.5f;
    Capsule capsule(pos1, pos2, radius);
    EXPECT_EQ(capsule.pos1, pos1);
    EXPECT_EQ(capsule.pos2, pos2);
    EXPECT_EQ(capsule.radius, radius);
}

TEST(Capsule, ConstructorWithCenterAndQuat)
{
    Vec3 center(1.0f, 2.0f, 3.0f);
    f32 height = 5.0f;
    f32 radius = 2.0f;
    Quat quat(0.0f, 0.0f, 0.0f, 1.0f);
    Capsule capsule(center, height, radius, quat);
    EXPECT_EQ(capsule.pos1, Vec3(1.0f, 4.5f, 3.0f));
    EXPECT_EQ(capsule.pos2, Vec3(1.0f, -0.5f, 3.0f));
    EXPECT_EQ(capsule.radius, radius);
}

TEST(Capsule, ConstructorWithCenterAndDirection)
{
    Vec3 center(1.0f, 2.0f, 3.0f);
    f32 height = 5.0f;
    f32 radius = 2.0f;
    Vec3 direction(0.0f, 1.0f, 0.0f);
    Capsule capsule(center, height, radius, direction);
    EXPECT_EQ(capsule.pos1, Vec3(1.0f, 4.5f, 3.0f));
    EXPECT_EQ(capsule.pos2, Vec3(1.0f, -0.5f, 3.0f));
    EXPECT_EQ(capsule.radius, radius);
}

TEST(Capsule, Height)
{
    Vec3 pos1(1.0f, 2.0f, 1.0f);
    Vec3 pos2(1.0f, 5.0f, 1.0f);
    f32 radius = 2.5f;
    Capsule capsule(pos1, pos2, radius);
    EXPECT_FLOAT_EQ(capsule.height(), 8.0f);
}

TEST(Capsule, MinHeight)
{
    Vec3 pos1(1.0f, 2.0f, 3.0f);
    Vec3 pos2(1.0f, 5.0f, 3.0f);
    f32 radius = 2.5f;
    Capsule capsule(pos1, pos2, radius);
    EXPECT_FLOAT_EQ(capsule.minHeight(), 3.0f);
}

TEST(Capsule, Direction)
{
    Vec3 pos1(1.0f, 2.0f, 3.0f);
    Vec3 pos2(4.0f, 5.0f, 6.0f);
    f32 radius = 2.5f;
    Capsule capsule(pos1, pos2, radius);
    EXPECT_EQ(capsule.direction(), Vec3(0.57735f, 0.57735f, 0.57735f));
}

TEST(Capsule, Volume)
{
    Vec3 pos1(1.0f, 2.0f, 3.0f);
    Vec3 pos2(1.0f, 5.0f, 3.0f);
    f32 radius = 2.5f;
    Capsule capsule(pos1, pos2, radius);
    EXPECT_FLOAT_EQ(capsule.volume(), radius * radius * Math::PI * 3.0f + 4.0f / 3.0f * radius * radius * radius * Math::PI);
}