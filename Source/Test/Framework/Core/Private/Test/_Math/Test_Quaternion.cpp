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
    CHECK_EQUAL(Quat(EForceInit::Force), Quat::identity);

    Quat q1(90, 90, 0);
    CHECK_EQUAL(q1, Quat(0.5f, 0.5f, -0.5f, 0.5f));

    Quat q2(Vec3(90, 90, 0));
    CHECK_EQUAL(q2, Quat(0.5f, 0.5f, -0.5f, 0.5f));

    Quat q3(90, Vec3(1, 0, 0));
    CHECK_EQUAL(q3, Quat(Math::SinD(45), 0, 0, Math::CosD(45)));
}

TEST(Quat, Operator) {
    Quat qx(90, 0, 0);
    Quat qy(0, 90, 0);
    Quat qz(0, 0, 45);

    Vec3 v(1, 2, 3);

    // グローバルだとZXY回転
    // ローカルだとYXZ回転
    Quat q = qy * qx * qz;
    CHECK_EQUAL(q * v, Vec3(2.12132001f, -3.0f, 0.707107067f));

    Quat q2=Quat::identity;
    q2 *= qy;
    q2 *= qx;
    q2 *= qz;
    CHECK_EQUAL(q2 * v, Vec3(2.12132001f, -3.0f, 0.707107067f));

    Quat q3 = Quat::identity;
    q3 *= qy;
    q3 *= qx;
    q3 /= qx;
    q3 /= qy;
    CHECK_EQUAL(q3, Quat::identity);

}

TEST(Quat, Rotate) {
    Quat qx(90, 0, 0);
    Quat qy(0, 90, 0);
    Quat qz(0, 0, 45);

    Quat q1 = qy * qx * qz;
    Quat q2 = Quat::identity;
    q2.rotateZ(45);
    q2.rotateX(90);
    q2.rotateY(90);
    CHECK_EQUAL(q1, q2);
    Quat q2_2 = Quat::identity;
    q2_2.rotate(90, 90, 45);
    CHECK_EQUAL(q2, q2_2);


    Quat q3 = qy * qx * qz;
    Quat q4 = Quat::identity;
    q4.rotateLocalY(90);
    q4.rotateLocalX(90);
    q4.rotateLocalZ(45);
    CHECK_EQUAL(q3, q4);
    Quat q4_2 = Quat::identity;
    q4_2.rotateLocal(90, 90, 45);
    CHECK_EQUAL(q4, q4_2);
}

TEST(Quat, Get) {
    Quat q(90, 90, 0);

    CHECK_EQUAL(q.getFront(), Vec3::Down);
    CHECK_EQUAL(q.getBack(), Vec3::Up);
    CHECK_EQUAL(q.getUp(), Vec3::Right);
    CHECK_EQUAL(q.getDown(), Vec3::Left);
    CHECK_EQUAL(q.getRight(), Vec3::Back);
    CHECK_EQUAL(q.getLeft(), Vec3::Front);

    CHECK_EQUAL(q.getAxis(), Vec3(1, 1, -1).unitVec());
    f32 ang = q.getAngle();
    CHECK_EQUAL(q.getAngle(), 120.0f);

}

TEST(Quat, GetRotation) {
    Quat q(30, 90, 0);

    Rot r = q.getRot();
    CHECK_EQUAL(r, Rot(30, 90, 0));
}
