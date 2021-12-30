//***********************************************************
//! @file
//! @brief		Quat のテスト
//! @author		Gajumaru
//***********************************************************
#include <Runtime/Core/Math/Vector/include.h>
#include <Runtime/Core/Math/Quaternion.h>

using namespace ob;


TEST(Quat, Construct) {
    EXPECT_EQ(Quat(EForceInit::Force), Quat::identity);

    Quat q1(90, 90, 0);
    EXPECT_EQ(q1, Quat(0.5f, 0.5f, -0.5f, 0.5f));

    Quat q2(Vec3(90, 90, 0));
    EXPECT_EQ(q2, Quat(0.5f, 0.5f, -0.5f, 0.5f));

    Quat q3(90, Vec3(1, 0, 0));
    EXPECT_EQ(q3, Quat(Mathf::SinD(45), 0, 0, Mathf::CosD(45)));
}

TEST(Quat, Operator) {
    Quat qx(90, 0, 0);
    Quat qy(0, 90, 0);
    Quat qz(0, 0, 45);

    Vec3 v(1, 2, 3);

    // グローバルだとZXY回転
    // ローカルだとYXZ回転
    Quat q = qy * qx * qz;
    EXPECT_EQ(q * v, Vec3(2.12132001f, -3.0f, 0.707107067f));

    Quat q2=Quat::identity;
    q2 *= qy;
    q2 *= qx;
    q2 *= qz;
    EXPECT_EQ(q2 * v, Vec3(2.12132001f, -3.0f, 0.707107067f));

    Quat q3 = Quat::identity;
    q3 *= qy;
    q3 *= qx;
    q3 /= qx;
    q3 /= qy;
    EXPECT_EQ(q3, Quat::identity);

}

TEST(Quat, Rotate) {
    Quat qx(90, 0, 0);
    Quat qy(0, 90, 0);
    Quat qz(0, 0, 45);

    Quat q1 = qy * qx * qz;
    Quat q2 = Quat::identity;
    q2.RotateZ(45);
    q2.RotateX(90);
    q2.RotateY(90);
    EXPECT_EQ(q1, q2);
    Quat q2_2 = Quat::identity;
    q2_2.Rotate(90, 90, 45);
    EXPECT_EQ(q2, q2_2);


    Quat q3 = qy * qx * qz;
    Quat q4 = Quat::identity;
    q4.RotateLocalY(90);
    q4.RotateLocalX(90);
    q4.RotateLocalZ(45);
    EXPECT_EQ(q3, q4);
    Quat q4_2 = Quat::identity;
    q4_2.RotateLocal(90, 90, 45);
    EXPECT_EQ(q4, q4_2);
}

TEST(Quat, Get) {
    Quat q(90, 90, 0);

    EXPECT_EQ(q.GetFront(), Vec3::down);
    EXPECT_EQ(q.GetBack(), Vec3::up);
    EXPECT_EQ(q.GetUp(), Vec3::right);
    EXPECT_EQ(q.GetDown(), Vec3::left);
    EXPECT_EQ(q.GetRight(), Vec3::back);
    EXPECT_EQ(q.GetLeft(), Vec3::front);

    EXPECT_EQ(q.GetAxis(), Vec3(1, 1, -1).GetUnitVec());
    f32 ang = q.GetAngle();
    EXPECT_EQ(q.GetAngle(), 120.0f);

}

TEST(Quat, GetRotation) {
    Quat q(30, 90, 0);

    Vec3 r = q.GetRot();
    EXPECT_EQ(r, Vec3(30, 90, 0));
}
