//***********************************************************
//! @file
//! @brief		Vector テスト
//! @author		Gajumaru
//***********************************************************
#include <Runtime/Core/Math/Vector/include.h>
#include <Runtime/Core/Math/Vector/IntVector4.h>
#include <Runtime/Core/Math/Quaternion.h>

using namespace ob;

TEST(Vector, ConstructorArg) {
    Vec2 v(1.0f, 2.0f);
    EXPECT_TRUE(v.x == 1.0f && v.y == 2.0f);

    Vec4    vf(4.0f, 5.0f, 6.0f, 7.0f);
    IntVec4 vi(1, 2, 3, 4);

    Vec4    vi2f(vi);
    IntVec4 vf2i(vf);

    EXPECT_EQ(vi2f, Vec4(1.0f, 2.0f, 3.0f, 4.0f));
    EXPECT_EQ(vf2i, IntVec4(4,5,6,7));
}


TEST(Vector, OpEq) {
    Vec2 v1(1.0f, 2.0f);
    Vec2 v2(1.0f, 2.0f);
    EXPECT_EQ(v1, v2);
}

TEST(Vector, OpAdd) {
    Vec2 v1(0.0f, 0.0f);
    Vec2 v2(1.0f, 2.0f);
    v1 += v2;
    EXPECT_EQ(v1, v2);
}

TEST(Vec3, Op) {
    // 等価演算子
    EXPECT_TRUE(Vec3(1, 2, 3) == Vec3(1, 2, 3));
    EXPECT_FALSE(Vec3(1, 2, 3) == Vec3(3, 2, 1));


    // 加算(ベクトル)
    EXPECT_EQ(Vec3(1, 2, 3) + Vec3(2, 4, 1), Vec3(3, 6, 4));
    // 加算(スカラー)
    EXPECT_EQ(Vec3(1, 2, 3) + 3, Vec3(4, 5, 6));

    // 減算(ベクトル)
    EXPECT_EQ(Vec3(1, 2, 3) - Vec3(2, 4, 1), Vec3(-1, -2, 2));
    // 減算(スカラー)
    EXPECT_EQ(Vec3(1, 2, 3) - 3, Vec3(-2, -1, 0));

    // 乗算(ベクトル)
    EXPECT_EQ(Vec3(1, 2, 3) * Vec3(2, 4, 1), Vec3(2, 8, 3));
    // 乗算(スカラー)
    EXPECT_EQ(Vec3(1, 2, 3) * 3, Vec3(3, 6, 9));
    // 乗算(スカラー)
    EXPECT_EQ(3 * Vec3(1, 2, 3), Vec3(3, 6, 9));

    // 除算(ベクトル)
    EXPECT_EQ(Vec3(1, 2, 3) / Vec3(2, 4, 1), Vec3(0.5f, 0.5f, 3.0f));
    // 除算(スカラー)
    EXPECT_EQ(Vec3(1, 2, 3) / 3, Vec3(1.0f / 3.0f, 2.0f / 3.0f, 3.0f / 3.0f));
    // 除算(スカラー)
    EXPECT_EQ(1 / Vec3(1, 2, 3), Vec3(1.0f / 1.0f, 1 / 2.0f, 1 / 3.0f));


    // 加算代入(ベクトル)
    EXPECT_EQ(Vec3(1, 2, 3) += Vec3(2, 4, 1), Vec3(3, 6, 4));
    // 加算代入(スカラー)
    EXPECT_EQ(Vec3(1, 2, 3) += 3, Vec3(4, 5, 6));

    // 減算代入(ベクトル)
    EXPECT_EQ(Vec3(1, 2, 3) -= Vec3(2, 4, 1), Vec3(-1, -2, 2));
    // 減算代入(スカラー)
    EXPECT_EQ(Vec3(1, 2, 3) -= 3, Vec3(-2, -1, 0));

    // 乗算代入(ベクトル)
    EXPECT_EQ(Vec3(1, 2, 3) *= Vec3(2, 4, 1), Vec3(2, 8, 3));
    // 乗算代入(スカラー)
    EXPECT_EQ(Vec3(1, 2, 3) *= 3, Vec3(3, 6, 9));

    // 除算代入(ベクトル)
    EXPECT_EQ(Vec3(1, 2, 3) / Vec3(2, 4, 1), Vec3(0.5f, 0.5f, 3.0f));
    // 除算代入(スカラー)
    EXPECT_EQ(Vec3(1, 2, 3) /= 3, Vec3(1.0f / 3.0f, 2.0f / 3.0f, 3.0f / 3.0f));


    // 単項マイナス
    EXPECT_EQ(-Vec3(1, 2, 3), Vec3(-1, -2, -3));
}


TEST(Vector, Set) {
    Vec3 v;
    v.Set(1, 2, 3);
    EXPECT_EQ(v, Vec3(1, 2, 3));

    Vec3 v2;
    v2.Set(v);
    EXPECT_EQ(v2, v);

    v.SetZero();
    EXPECT_EQ(v, Vec3::zero);

    v.SetAll(1);
    EXPECT_EQ(v, Vec3(1, 1, 1));
}


TEST(Vector, Get) {
    Vec3 v(1, -4, 3);
    EXPECT_EQ(v.GetMaxComponent(), 3);

    EXPECT_EQ(v.GetMaxAbsComponent(), 4);

    EXPECT_EQ(v.GetMinComponent(), -4);

    EXPECT_EQ(v.GetMinAbsComponent(), 1);

    Vec3 v2(3, 4, 0);
    EXPECT_EQ(v2.GetMag(), 5);
    EXPECT_EQ(v2.GetSqrMag(), 25);

    Vec3 v3(100, 0, 0);
    EXPECT_EQ(v3.GetUnitVec(), Vec3(1, 0, 0));
}


TEST(Vector, Convert) {
    Vec3 v(100, 0, 0);
    v.Normalize();
    EXPECT_EQ(v, Vec3(1, 0, 0));

    Vec3 dir;
    f32 len;
    Vec3 v2(2, 2, 0);
    v2.ToDirectionAndLength(dir, len);
    EXPECT_EQ(dir, Vec3(1, 1, 0).GetUnitVec());
    EXPECT_EQ(len, Mathf::Sqrt(8));
}


TEST(Vector, Transform) {
    Vec3 v(1, 0, 0);
    v.Translate(1, 2, 3);
    EXPECT_EQ(v, Vec3(2, 2, 3));

    v.Translate(Vec3(1, 2, 3));
    EXPECT_EQ(v, Vec3(3, 4, 6));

    v.Set(1, 2, 3);

    v.RotateZ(90);
    EXPECT_EQ(v, Vec3(-2, 1, 3));

    v.RotateX(90);
    EXPECT_EQ(v, Vec3(-2, -3, 1));

    v.RotateY(90);
    EXPECT_EQ(v, Vec3(1, -3, 2));

    v.Set(1, 2, 3);
    v.Rotate(90, 90, 90);
    EXPECT_EQ(v, Vec3(1, -3, 2));

    v.Scale(2);
    EXPECT_EQ(v, Vec3(2, -6, 4));

    v.Scale(0, 1, 2);
    EXPECT_EQ(v, Vec3(0, -6, 8));

}


TEST(Vector, Judge) {
    Vec3 v(1, 0, 0);
    EXPECT_TRUE(v.IsUnit());

    Vec3 v2(0.91f, 0, 0);
    EXPECT_TRUE(v.Equals(v2, 0.1f));


    EXPECT_FALSE(v.AllComponentsEqual());
    v.Set(3, 3, 3);
    EXPECT_TRUE(v.AllComponentsEqual());

    EXPECT_FALSE(v.IsZero());
    v *= 0;
    EXPECT_TRUE(v.IsZero());

    EXPECT_FALSE(v.IsNaN());
    v /= 0;
    EXPECT_TRUE(v.IsNaN());
}


TEST(Vector, Static) {
    Vec3 v(1, -10, -3);
    EXPECT_EQ(Vec3::Abs(v), Vec3(1, 10, 3));

    Vec3 a(3, 5, 3);
    Vec3 b(5, 2, 4);
    EXPECT_EQ(Vec3::Dot(a, b), 37);

    EXPECT_EQ(Vec3::Cross(a, b), Vec3(14, 3, -19));

    a.Set(2, 0, 0);
    b.Set(5, 4, 0);
    EXPECT_EQ(Vec3::Dist(a, b), 5);
    EXPECT_EQ(Vec3::MDist(a, b), 7);

    EXPECT_EQ(Vec3::DistXY(a, b), 5);
    EXPECT_EQ(Vec3::DistXZ(a, b), 3);
    EXPECT_EQ(Vec3::DistYZ(a, b), 4);

    EXPECT_EQ(Vec3::Lerp(a, b, 0.5f), Vec3(3.5f, 2.0f, 0));

    a.Set(1, 0, 0);
    b.Set(0, 1, 0);
    EXPECT_EQ(Vec3::Slerp(a, b, 0.5f), Vec3(Mathf::CosD(45), Mathf::SinD(45), 0));

    a.Set(3, 5, 3);
    b.Set(5, 2, 4);
    EXPECT_EQ(Vec3::Max(a, b), Vec3(5, 5, 4));
    EXPECT_EQ(Vec3::Min(a, b), Vec3(3, 2, 3));

    a.Set(1, 0, 0);
    b.Set(0, 1, 0);
    EXPECT_EQ(Vec3::Angle(a, b), 90.0f);

    Vec3 f(1, 1, 1);
    Vec3 n(-1, 0, 0);
    EXPECT_EQ(Vec3::Mirror(f, n), Vec3(-1, 1, 1));

}