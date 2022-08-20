//***********************************************************
//! @file
//! @brief		Quat のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Math/Vectors.h>
#include <Framework/Core/Math/Quaternion.h>
#include <Framework/Core/Math/Rotation.h>

using namespace ob;

TEST(Quat, Construct) {

    Quat q1(90, 90, 0);
    ASSERT_EQ(q1, Quat(0.5f, 0.5f, -0.5f, 0.5f));

    Quat q2(Rot(90, 90, 0));
    ASSERT_EQ(q2, Quat(0.5f, 0.5f, -0.5f, 0.5f));

    Quat q3(Vec3(1, 0, 0),90);
    ASSERT_EQ(q3, Quat(Math::SinD(45), 0, 0, Math::CosD(45)));
}

TEST(Quat, Operator) {
    Quat qx(90, 0, 0);
    Quat qy(0, 90, 0);
    Quat qz(0, 0, 45);

    Vec3 v(1, 2, 3);

    // グローバルだとZXY回転
    // ローカルだとYXZ回転
    Quat q = qy * qx * qz;
    ASSERT_EQ(q * v, Vec3(2.12132001f, -3.0f, 0.707107067f));

    Quat q2=Quat::Identity;
    q2 *= qy;
    q2 *= qx;
    q2 *= qz;
    ASSERT_EQ(q2 * v, Vec3(2.12132001f, -3.0f, 0.707107067f));

    Quat q3 = Quat::Identity;
    q3 *= qy;
    q3 *= qx;
    q3 /= qx;
    q3 /= qy;
    ASSERT_EQ(q3, Quat::Identity);

}

TEST(Quat, Rotate) {
    Quat qx(90, 0, 0);
    Quat qy(0, 90, 0);
    Quat qz(0, 0, 45);

    Quat q1 = qy * qx * qz;
    Quat q2 = Quat::Identity;
    q2.rotateZ(45);
    q2.rotateX(90);
    q2.rotateY(90);
    ASSERT_EQ(q1, q2);
    Quat q2_2 = Quat::Identity;
    q2_2.rotate(90, 90, 45);
    ASSERT_EQ(q2, q2_2);


    Quat q3 = qy * qx * qz;
    Quat q4 = Quat::Identity;
    q4.rotateLocalY(90);
    q4.rotateLocalX(90);
    q4.rotateLocalZ(45);
    ASSERT_EQ(q3, q4);
    Quat q4_2 = Quat::Identity;
    q4_2.rotateLocal(90, 90, 45);
    ASSERT_EQ(q4, q4_2);
}

TEST(Quat, Get) {
    Quat q(90, 90, 0);

    ASSERT_EQ(q.front(), Vec3::Down);
    ASSERT_EQ(q.back(), Vec3::Up);
    ASSERT_EQ(q.up(), Vec3::Right);
    ASSERT_EQ(q.down(), Vec3::Left);
    ASSERT_EQ(q.right(), Vec3::Back);
    ASSERT_EQ(q.left(), Vec3::Front);

    ASSERT_EQ(q.axis(), Vec3(1, 1, -1).unitVec());
    f32 ang = q.angle();
    ASSERT_EQ(q.angle(), 120.0f);

}

TEST(Quat, GetRotation) {
    Quat q(30, 90, 0);

    Rot r = q.toRot();
    ASSERT_EQ(r, Rot(30, 90, 0));
}
