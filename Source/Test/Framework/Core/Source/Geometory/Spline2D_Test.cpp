//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <gtest/gtest.h>
#include <Framework/Core/Geometry/Spline2D.h>

using namespace ob::core;

TEST(Spline2D, BasicConstruction) {
    Spline2D spline;
    EXPECT_TRUE(spline.empty());
    EXPECT_EQ(spline.size(), 0);
}

TEST(Spline2D, LinearInterpolation) {
    Spline2D spline;
    spline.points = {Vec2(0.0f, 0.0f), Vec2(10.0f, 10.0f)};
    
    Vec2 start = spline.position(0.0f);
    Vec2 middle = spline.position(0.5f);
    Vec2 end = spline.position(1.0f);
    
    EXPECT_TRUE(start.equals(Vec2(0.0f, 0.0f), 0.001f));
    EXPECT_TRUE(middle.equals(Vec2(5.0f, 5.0f), 0.001f));
    EXPECT_TRUE(end.equals(Vec2(10.0f, 10.0f), 0.001f));
}

TEST(Spline2D, CatmullRomSpline) {
    Spline2D spline;
    spline.points = {Vec2(0.0f, 0.0f), Vec2(1.0f, 1.0f), Vec2(2.0f, 0.0f), Vec2(3.0f, 1.0f)};
    
    // 制御点を通ることを確認
    Vec2 p1 = spline.position(0.0f);
    Vec2 p2 = spline.position(1.0f/3.0f);
    Vec2 p3 = spline.position(2.0f/3.0f);
    Vec2 p4 = spline.position(1.0f);
    
    EXPECT_TRUE(p1.equals(Vec2(0.0f, 0.0f), 0.001f));
    EXPECT_TRUE(p2.equals(Vec2(1.0f, 1.0f), 0.001f));
    EXPECT_TRUE(p3.equals(Vec2(2.0f, 0.0f), 0.001f));
    EXPECT_TRUE(p4.equals(Vec2(3.0f, 1.0f), 0.001f));
}

TEST(Spline2D, VelocityCalculation) {
    Spline2D spline;
    spline.points = {Vec2(0.0f, 0.0f), Vec2(10.0f, 0.0f)};
    
    Vec2 velocity = spline.velocity(0.5f);
    EXPECT_GT(velocity.length(), 0.0f);
}

TEST(Spline2D, LengthCalculation) {
    Spline2D spline;
    spline.points = {Vec2(0.0f, 0.0f), Vec2(3.0f, 4.0f)}; // 3-4-5三角形
    
    f32 length = spline.length(100);
    EXPECT_NEAR(length, 5.0f, 0.001f); // 直線なので正確に5.0
}

TEST(Spline2D, CurvatureCalculation) {
    Spline2D spline;
    spline.points = {Vec2(0.0f, 0.0f), Vec2(1.0f, 1.0f), Vec2(2.0f, 0.0f)};
    
    f32 curvature = spline.curvature(0.5f);
    EXPECT_GE(curvature, 0.0f); // 曲率は非負
}
