//***********************************************************
//! @file
//! @brief		Capsule のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Geometory/Capsule.h>

using namespace ob;


TEST(Capsule, Construct) {
    {
        Capsule capsule(EForceInit::Force);
        EXPECT_EQ(capsule.pos1, Vec3::Zero);
        EXPECT_EQ(capsule.pos2, Vec3::Zero);
        EXPECT_EQ(capsule.radius, 0.0f);
    }

    // 始点 / 終点
    {
        Vec3 pos1(1, 2, 3);
        Vec3 pos2(2, 3, 4);
        f32 radius = 2.f;
        Capsule capsule(pos1, pos2, radius);
        EXPECT_EQ(capsule.pos1, pos1);
        EXPECT_EQ(capsule.pos2, pos2);
        EXPECT_EQ(capsule.radius, radius);
    }

    // 中心 / 高さ / 回転
    {
        Vec3 pos(0, 2, 3);
        f32 height = 4.f;
        f32 radius = 2.f;
        Quat quat(90, 0, 0);
        Capsule capsule(pos, height, radius, quat);
        EXPECT_EQ(capsule.pos1, Vec3(0, 2, 5));
        EXPECT_EQ(capsule.pos2, Vec3(0, 2, 1));
        EXPECT_EQ(capsule.radius, radius);
    }

    // 中心 / 高さ / 方向
    {
        Vec3 pos(0, 2, 3);
        f32 height = 4.f;
        f32 radius = 2.f;
        Vec3 direction(1, 0, 0);
        Capsule capsule(pos, height, radius, direction);
        EXPECT_EQ(capsule.pos1, Vec3(2, 2, 3));
        EXPECT_EQ(capsule.pos2, Vec3(-2, 2, 3));
        EXPECT_EQ(capsule.radius, radius);
    }
}


TEST(Capsule, Getter) {
    // 高さ
    {
        Vec3 pos1(1, 2, 3);
        Vec3 pos2(2, 3, 4);
        f32 radius = 2.f;
        Capsule capsule(pos1, pos2, radius);

        EXPECT_EQ(capsule.height(), Vec3::Dist(pos1, pos2) + radius);
        EXPECT_EQ(capsule.minHeight(), Vec3::Dist(pos1,pos2));
    }

    // 中心 / 高さ / 回転
    {
        Capsule capsule(Vec3(0,3,3),Vec3(0,5,2),1.0f);
        EXPECT_EQ(capsule.direction(), Vec3(0, 2, -1).unitVec());
    }
}
