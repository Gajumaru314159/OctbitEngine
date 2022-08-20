//***********************************************************
//! @file
//! @brief		Box のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Geometory/Box.h>

using namespace ob;

TEST(Box, Construct) {

    // サイズ / 中心
    {
        Vec3 center(1.f, 2.f, 3.f);
        Vec3 size(4.f, 5.f, 6.f);
        Box box(size, center);
        EXPECT_EQ(box.size, size);
        EXPECT_EQ(box.center, center);
    }

    // 点群を含む最小のBox
    {
        Vec3 points[] = {
            Vec3(0,0,3),
            Vec3(2,0,3),
            Vec3(3,2,4),
            Vec3(-4,0,0),
            Vec3(0,-4,3),
        };
        Box box(points);

        EXPECT_EQ(box.size, Vec3(7, 6, 4));
        EXPECT_EQ(box.center, Vec3(-0.5f, -1, 2));
    }
}


TEST(Box, Operator) {
    {
        Box box(Vec3::One);
        box += Vec3(4, 4, 4);
        EXPECT_EQ(box.center, Vec3(1.75f, 1.75f, 1.75f));
        EXPECT_EQ(box.size, Vec3(4.5f, 4.5f, 4.5f));
    }
}


TEST(Box, Getter) {
    {
        Box box(Vec3(4, 2, 6), Vec3(2, 1, 0));
        EXPECT_EQ(box.min(), Vec3(4, 2, 3));
        EXPECT_EQ(box.max(), Vec3(0, 0, -3));
    }
}

TEST(Box, IsEmpty) {
    {
        Box boxA(Vec3::Zero);
        Box boxB(Vec3::One, Vec3::Zero);
        EXPECT_TRUE(boxA.empty());
        EXPECT_FALSE(boxB.empty());
    }
}