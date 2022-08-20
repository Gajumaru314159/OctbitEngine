//***********************************************************
//! @file
//! @brief		Vector テスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Math/Vectors.h>
#include <Framework/Core/Math/IntVector4.h>
#include <Framework/Core/Math/Quaternion.h>

using namespace ob;

SimpleString StringFrom(const Vec4& quat) {
    return fmt::format("{},{},{},{}", quat.x, quat.y, quat.z,quat.w).c_str();
}

SimpleString StringFrom(const Vec3& quat) {
    return fmt::format("{},{},{}", quat.x, quat.y, quat.z).c_str();
}

SimpleString StringFrom(const Vec2& quat) {
    return fmt::format("{},{}", quat.x, quat.y).c_str();
}

TEST_GROUP(Vector) {};

TEST(Vector, ConstructorArg) {
    Vec2 v(1.0f, 2.0f);
    CHECK_TRUE(v.x == 1.0f && v.y == 2.0f);

    Vec4    vf(4.0f, 5.0f, 6.0f, 7.0f);
    IntVec4 vi(1, 2, 3, 4);

    Vec4    vi2f(vi);
    IntVec4 vf2i(vf);

    CHECK_EQUAL(vi2f, Vec4(1.0f, 2.0f, 3.0f, 4.0f));
    CHECK_TRUE(vf2i== IntVec4(4,5,6,7));
}


TEST(Vector, OpEq) {
    Vec2 v1(1.0f, 2.0f);
    Vec2 v2(1.0f, 2.0f);
    CHECK_EQUAL(v1, v2);
}

TEST(Vector, OpAdd) {
    Vec2 v1(0.0f, 0.0f);
    Vec2 v2(1.0f, 2.0f);
    v1 += v2;
    CHECK_EQUAL(v1, v2);
}

TEST(Vector, Op) {
    // 等価演算子
    CHECK_TRUE(Vec3(1, 2, 3) == Vec3(1, 2, 3));
    CHECK_FALSE(Vec3(1, 2, 3) == Vec3(3, 2, 1));


    // 加算(ベクトル)
    CHECK_EQUAL(Vec3(1, 2, 3) + Vec3(2, 4, 1), Vec3(3, 6, 4));
    // 加算(スカラー)
    CHECK_EQUAL(Vec3(1, 2, 3) + 3, Vec3(4, 5, 6));

    // 減算(ベクトル)
    CHECK_EQUAL(Vec3(1, 2, 3) - Vec3(2, 4, 1), Vec3(-1, -2, 2));
    // 減算(スカラー)
    CHECK_EQUAL(Vec3(1, 2, 3) - 3, Vec3(-2, -1, 0));

    // 乗算(ベクトル)
    CHECK_EQUAL(Vec3(1, 2, 3) * Vec3(2, 4, 1), Vec3(2, 8, 3));
    // 乗算(スカラー)
    CHECK_EQUAL(Vec3(1, 2, 3) * 3, Vec3(3, 6, 9));
    // 乗算(スカラー)
    CHECK_EQUAL(3 * Vec3(1, 2, 3), Vec3(3, 6, 9));

    // 除算(ベクトル)
    CHECK_EQUAL(Vec3(1, 2, 3) / Vec3(2, 4, 1), Vec3(0.5f, 0.5f, 3.0f));
    // 除算(スカラー)
    CHECK_EQUAL(Vec3(1, 2, 3) / 3, Vec3(1.0f / 3.0f, 2.0f / 3.0f, 3.0f / 3.0f));
    // 除算(スカラー)
    CHECK_EQUAL(1 / Vec3(1, 2, 3), Vec3(1.0f / 1.0f, 1 / 2.0f, 1 / 3.0f));


    // 加算代入(ベクトル)
    CHECK_EQUAL(Vec3(1, 2, 3) += Vec3(2, 4, 1), Vec3(3, 6, 4));
    // 加算代入(スカラー)
    CHECK_EQUAL(Vec3(1, 2, 3) += 3, Vec3(4, 5, 6));

    // 減算代入(ベクトル)
    CHECK_EQUAL(Vec3(1, 2, 3) -= Vec3(2, 4, 1), Vec3(-1, -2, 2));
    // 減算代入(スカラー)
    CHECK_EQUAL(Vec3(1, 2, 3) -= 3, Vec3(-2, -1, 0));

    // 乗算代入(ベクトル)
    CHECK_EQUAL(Vec3(1, 2, 3) *= Vec3(2, 4, 1), Vec3(2, 8, 3));
    // 乗算代入(スカラー)
    CHECK_EQUAL(Vec3(1, 2, 3) *= 3, Vec3(3, 6, 9));

    // 除算代入(ベクトル)
    CHECK_EQUAL(Vec3(1, 2, 3) / Vec3(2, 4, 1), Vec3(0.5f, 0.5f, 3.0f));
    // 除算代入(スカラー)
    CHECK_EQUAL(Vec3(1, 2, 3) /= 3, Vec3(1.0f / 3.0f, 2.0f / 3.0f, 3.0f / 3.0f));


    // 単項マイナス
    CHECK_EQUAL(-Vec3(1, 2, 3), Vec3(-1, -2, -3));
}


TEST(Vector, set) {
    Vec3 v;
    v.set(1, 2, 3);
    CHECK_EQUAL(v, Vec3(1, 2, 3));

    Vec3 v2;
    v2.set(v);
    CHECK_EQUAL(v2, v);

    v.setZero();
    CHECK_EQUAL(v, Vec3::Zero);

    v.setAll(1);
    CHECK_EQUAL(v, Vec3(1, 1, 1));
}


TEST(Vector, get) {
    Vec3 v(1, -4, 3);
    CHECK_EQUAL(v.maxComponent(), 3);

    CHECK_EQUAL(v.maxAbsComponent(), 4);

    CHECK_EQUAL(v.minComponent(), -4);

    CHECK_EQUAL(v.minAbsComponent(), 1);

    Vec3 v2(3, 4, 0);
    CHECK_EQUAL(v2.length(), 5);
    CHECK_EQUAL(v2.sqrLength(), 25);

    Vec3 v3(100, 0, 0);
    CHECK_EQUAL(v3.unitVec(), Vec3(1, 0, 0));
}


TEST(Vector, Convert) {
    Vec3 v(100, 0, 0);
    v.normalize();
    CHECK_EQUAL(v, Vec3(1, 0, 0));

    Vec3 dir;
    f32 len;
    Vec3 v2(2, 2, 0);
    v2.toDirectionAndLength(dir, len);
    CHECK_EQUAL(dir, Vec3(1, 1, 0).unitVec());
    CHECK_EQUAL(len, Math::Sqrt(8));
}


TEST(Vector, Transform) {
    Vec3 v(1, 0, 0);
    v.translate(1, 2, 3);
    CHECK_EQUAL(v, Vec3(2, 2, 3));

    v.translate(Vec3(1, 2, 3));
    CHECK_EQUAL(v, Vec3(3, 4, 6));

    v.set(1, 2, 3);

    v.rotateZ(90);
    CHECK_EQUAL(v, Vec3(-2, 1, 3));

    v.rotateX(90);
    CHECK_EQUAL(v, Vec3(-2, -3, 1));

    v.rotateY(90);
    CHECK_EQUAL(v, Vec3(1, -3, 2));

    v.set(1, 2, 3);
    v.rotate(90, 90, 90);
    CHECK_EQUAL(v, Vec3(1, -3, 2));

    v.scale(2);
    CHECK_EQUAL(v, Vec3(2, -6, 4));

    v.scale(0, 1, 2);
    CHECK_EQUAL(v, Vec3(0, -6, 8));

}


TEST(Vector, Judge) {
    Vec3 v(1, 0, 0);
    CHECK_TRUE(v.isUnit());

    Vec3 v2(0.91f, 0, 0);
    CHECK_TRUE(v.equals(v2, 0.1f));


    CHECK_FALSE(v.allComponentsEqual());
    v.set(3, 3, 3);
    CHECK_TRUE(v.allComponentsEqual());

    CHECK_FALSE(v.isZero());
    v *= 0;
    CHECK_TRUE(v.isZero());

    CHECK_FALSE(v.isNaN());
    v /= 0;
    CHECK_TRUE(v.isNaN());
}


TEST(Vector, Static) {
    Vec3 v(1, -10, -3);
    CHECK_EQUAL(Vec3::Abs(v), Vec3(1, 10, 3));

    Vec3 a(3, 5, 3);
    Vec3 b(5, 2, 4);
    CHECK_EQUAL(Vec3::Dot(a, b), 37);

    CHECK_EQUAL(Vec3::Cross(a, b), Vec3(14, 3, -19));

    a.set(2, 0, 0);
    b.set(5, 4, 0);
    CHECK_EQUAL(Vec3::Dist(a, b), 5);
    CHECK_EQUAL(Vec3::MDist(a, b), 7);

    CHECK_EQUAL(Vec3::DistXY(a, b), 5);
    CHECK_EQUAL(Vec3::DistXZ(a, b), 3);
    CHECK_EQUAL(Vec3::DistYZ(a, b), 4);

    CHECK_EQUAL(Vec3::Lerp(a, b, 0.5f), Vec3(3.5f, 2.0f, 0));

    a.set(1, 0, 0);
    b.set(0, 1, 0);
    CHECK_EQUAL(Vec3::Slerp(a, b, 0.5f), Vec3(Math::CosD(45), Math::SinD(45), 0));

    a.set(3, 5, 3);
    b.set(5, 2, 4);
    CHECK_EQUAL(Vec3::Max(a, b), Vec3(5, 5, 4));
    CHECK_EQUAL(Vec3::Min(a, b), Vec3(3, 2, 3));

    a.set(1, 0, 0);
    b.set(0, 1, 0);
    CHECK_EQUAL(Vec3::Angle(a, b), 90.0f);

    Vec3 f(1, 1, 1);
    Vec3 n(-1, 0, 0);
    CHECK_EQUAL(Vec3::Mirror(f, n), Vec3(-1, 1, 1));

}