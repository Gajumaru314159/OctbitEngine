//***********************************************************
//! @file
//! @brief		Vector テスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Math/Vectors.h>
#include <Framework/Core/Math/IntVector4.h>
#include <Framework/Core/Math/Quaternion.h>

using namespace ob;

TEST(Vector, Constructor) {
    Vec4    vf(4.0f, 5.0f, 6.0f, 7.0f);
    IntVec4 vi(1, 2, 3, 4);

    Vec4    vi2f(vi);
    IntVec4 vf2i(vf);

    ASSERT_EQ(vi2f, Vec4(1.0f, 2.0f, 3.0f, 4.0f));
    ASSERT_TRUE(vf2i== IntVec4(4,5,6,7));
}


TEST(Vector, OpEq) {
    Vec2 v1(1.0f, 2.0f);
    Vec2 v2(1.0f, 2.0f);
    ASSERT_EQ(v1, v2);
}

TEST(Vector, OpAdd) {
    Vec2 v1(0.0f, 0.0f);
    Vec2 v2(1.0f, 2.0f);
    v1 += v2;
    ASSERT_EQ(v1, v2);
}

TEST(Vector, Op) {
    // 等価演算子
    ASSERT_EQ(Vec3(1, 2, 3), Vec3(1, 2, 3));
    ASSERT_NE(Vec3(1, 2, 3), Vec3(3, 2, 1));


    // 加算(ベクトル)
    ASSERT_EQ(Vec3(1, 2, 3) + Vec3(2, 4, 1), Vec3(3, 6, 4));
    // 加算(スカラー)
    ASSERT_EQ(Vec3(1, 2, 3) + 3, Vec3(4, 5, 6));

    // 減算(ベクトル)
    ASSERT_EQ(Vec3(1, 2, 3) - Vec3(2, 4, 1), Vec3(-1, -2, 2));
    // 減算(スカラー)
    ASSERT_EQ(Vec3(1, 2, 3) - 3, Vec3(-2, -1, 0));

    // 乗算(ベクトル)
    ASSERT_EQ(Vec3(1, 2, 3) * Vec3(2, 4, 1), Vec3(2, 8, 3));
    // 乗算(スカラー)
    ASSERT_EQ(Vec3(1, 2, 3) * 3, Vec3(3, 6, 9));
    // 乗算(スカラー)
    ASSERT_EQ(3 * Vec3(1, 2, 3), Vec3(3, 6, 9));

    // 除算(ベクトル)
    ASSERT_EQ(Vec3(1, 2, 3) / Vec3(2, 4, 1), Vec3(0.5f, 0.5f, 3.0f));
    // 除算(スカラー)
    ASSERT_EQ(Vec3(1, 2, 3) / 3, Vec3(1.0f / 3.0f, 2.0f / 3.0f, 3.0f / 3.0f));
    // 除算(スカラー)
    ASSERT_EQ(1 / Vec3(1, 2, 3), Vec3(1.0f / 1.0f, 1 / 2.0f, 1 / 3.0f));


    // 加算代入(ベクトル)
    ASSERT_EQ(Vec3(1, 2, 3) += Vec3(2, 4, 1), Vec3(3, 6, 4));
    // 加算代入(スカラー)
    ASSERT_EQ(Vec3(1, 2, 3) += 3, Vec3(4, 5, 6));

    // 減算代入(ベクトル)
    ASSERT_EQ(Vec3(1, 2, 3) -= Vec3(2, 4, 1), Vec3(-1, -2, 2));
    // 減算代入(スカラー)
    ASSERT_EQ(Vec3(1, 2, 3) -= 3, Vec3(-2, -1, 0));

    // 乗算代入(ベクトル)
    ASSERT_EQ(Vec3(1, 2, 3) *= Vec3(2, 4, 1), Vec3(2, 8, 3));
    // 乗算代入(スカラー)
    ASSERT_EQ(Vec3(1, 2, 3) *= 3, Vec3(3, 6, 9));

    // 除算代入(ベクトル)
    ASSERT_EQ(Vec3(1, 2, 3) / Vec3(2, 4, 1), Vec3(0.5f, 0.5f, 3.0f));
    // 除算代入(スカラー)
    ASSERT_EQ(Vec3(1, 2, 3) /= 3, Vec3(1.0f / 3.0f, 2.0f / 3.0f, 3.0f / 3.0f));


    // 単項マイナス
    ASSERT_EQ(-Vec3(1, 2, 3), Vec3(-1, -2, -3));
}


TEST(Vector, set) {
    Vec3 v;
    v.set(1, 2, 3);
    ASSERT_EQ(v, Vec3(1, 2, 3));

    Vec3 v2;
    v2.set(v);
    ASSERT_EQ(v2, v);

    v.setZero();
    ASSERT_EQ(v, Vec3::Zero);

    v.setAll(1);
    ASSERT_EQ(v, Vec3(1, 1, 1));
}


TEST(Vector, get) {
    Vec3 v(1, -4, 3);
    ASSERT_EQ(v.maxComponent(), 3);

    ASSERT_EQ(v.maxAbsComponent(), 4);

    ASSERT_EQ(v.minComponent(), -4);

    ASSERT_EQ(v.minAbsComponent(), 1);

    Vec3 v2(3, 4, 0);
    ASSERT_EQ(v2.length(), 5);
    ASSERT_EQ(v2.sqrLength(), 25);

    Vec3 v3(100, 0, 0);
    ASSERT_EQ(v3.unitVec(), Vec3(1, 0, 0));
}


TEST(Vector, Convert) {
    Vec3 v(100, 0, 0);
    v.normalize();
    ASSERT_EQ(v, Vec3(1, 0, 0));

    Vec3 dir;
    f32 len;
    Vec3 v2(2, 2, 0);
    v2.toDirectionAndLength(dir, len);
    ASSERT_EQ(dir, Vec3(1, 1, 0).unitVec());
    ASSERT_EQ(len, Math::Sqrt(8));
}


TEST(Vector, Transform) {
    Vec3 v(1, 0, 0);
    v.translate(1, 2, 3);
    ASSERT_EQ(v, Vec3(2, 2, 3));

    v.translate(Vec3(1, 2, 3));
    ASSERT_EQ(v, Vec3(3, 4, 6));

    v.set(1, 2, 3);

    v.rotateZ(90);
    ASSERT_EQ(v, Vec3(-2, 1, 3));

    v.rotateX(90);
    ASSERT_EQ(v, Vec3(-2, -3, 1));

    v.rotateY(90);
    ASSERT_EQ(v, Vec3(1, -3, 2));

    v.set(1, 2, 3);
    v.rotate(90, 90, 90);
    ASSERT_EQ(v, Vec3(1, -3, 2));

    v.scale(2);
    ASSERT_EQ(v, Vec3(2, -6, 4));

    v.scale(0, 1, 2);
    ASSERT_EQ(v, Vec3(0, -6, 8));

}


TEST(Vector, Judge) {
    Vec3 v(1, 0, 0);
    ASSERT_TRUE(v.isUnit());

    Vec3 v2(0.91f, 0, 0);
    ASSERT_TRUE(v.equals(v2, 0.1f));


    ASSERT_FALSE(v.allComponentsEqual());
    v.set(3, 3, 3);
    ASSERT_TRUE(v.allComponentsEqual());

    ASSERT_FALSE(v.isZero());
    v *= 0;
    ASSERT_TRUE(v.isZero());

    ASSERT_FALSE(v.isNaN());
    v /= 0;
    ASSERT_TRUE(v.isNaN());
}


TEST(Vector, Static) {
    Vec3 v(1, -10, -3);
    ASSERT_EQ(Vec3::Abs(v), Vec3(1, 10, 3));

    Vec3 a(3, 5, 3);
    Vec3 b(5, 2, 4);
    ASSERT_EQ(Vec3::Dot(a, b), 37);

    ASSERT_EQ(Vec3::Cross(a, b), Vec3(14, 3, -19));

    a.set(2, 0, 0);
    b.set(5, 4, 0);
    ASSERT_EQ(Vec3::Dist(a, b), 5);
    ASSERT_EQ(Vec3::MDist(a, b), 7);

    ASSERT_EQ(Vec3::DistXY(a, b), 5);
    ASSERT_EQ(Vec3::DistXZ(a, b), 3);
    ASSERT_EQ(Vec3::DistYZ(a, b), 4);

    ASSERT_EQ(Vec3::Lerp(a, b, 0.5f), Vec3(3.5f, 2.0f, 0));

    a.set(1, 0, 0);
    b.set(0, 1, 0);
    ASSERT_EQ(Vec3::Slerp(a, b, 0.5f), Vec3(Math::CosD(45), Math::SinD(45), 0));

    a.set(3, 5, 3);
    b.set(5, 2, 4);
    ASSERT_EQ(Vec3::Max(a, b), Vec3(5, 5, 4));
    ASSERT_EQ(Vec3::Min(a, b), Vec3(3, 2, 3));

    a.set(1, 0, 0);
    b.set(0, 1, 0);
    ASSERT_EQ(Vec3::Angle(a, b), 90.0f);

    Vec3 f(1, 1, 1);
    Vec3 n(-1, 0, 0);
    ASSERT_EQ(Vec3::Mirror(f, n), Vec3(-1, 1, 1));

}