﻿//***********************************************************
//! @file
//! @brief		Capsule のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Geometory/Capsule.h>

using namespace ob;

TEST_GROUP(Capsule) {
};

TEST(Capsule, Construct) {

    // 始点 / 終点
    {
        Vec3 pos1(1, 2, 3);
        Vec3 pos2(2, 3, 4);
        f32 radius = 2.f;
        Capsule capsule(pos1, pos2, radius);
        CHECK_TRUE(capsule.pos1 == pos1);
        CHECK_TRUE(capsule.pos2 == pos2);
        CHECK_TRUE(capsule.radius == radius);
    }

    // 中心 / 高さ / 回転
    {
        Vec3 pos(0, 2, 3);
        f32 height = 4.f;
        f32 radius = 2.f;
        Quat quat(90, 0, 0);
        Capsule capsule(pos, height, radius, quat);
        CHECK_TRUE(capsule.pos1 == Vec3(0 , 2 , 5));
        CHECK_TRUE(capsule.pos2 == Vec3(0 , 2 , 1));
        CHECK_TRUE(capsule.radius == radius);
    }

    // 中心 / 高さ / 方向
    {
        Vec3 pos(0, 2, 3);
        f32 height = 4.f;
        f32 radius = 2.f;
        Vec3 direction(1, 0, 0);
        Capsule capsule(pos, height, radius, direction);
        CHECK_TRUE(capsule.pos1 == Vec3(2, 2, 3));
        CHECK_TRUE(capsule.pos2 == Vec3(-2, 2, 3));
        CHECK_TRUE(capsule.radius == radius);
    }
}


TEST(Capsule, Getter) {
    // 高さ
    {
        Vec3 pos1(1, 2, 3);
        Vec3 pos2(2, 3, 4);
        f32 radius = 2.f;
        Capsule capsule(pos1, pos2, radius);

        CHECK_TRUE(capsule.height() == Vec3::Dist(pos1, pos2) + radius);
        CHECK_TRUE(capsule.minHeight() == Vec3::Dist(pos1,pos2));
    }

    // 中心 / 高さ / 回転
    {
        Capsule capsule(Vec3(0,3,3),Vec3(0,5,2),1.0f);
        CHECK_TRUE(capsule.direction() == Vec3(0, 2, -1).unitVec());
    }
}