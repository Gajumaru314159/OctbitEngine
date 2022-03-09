//***********************************************************
//! @file
//! @brief		Matrix ‚ÌƒeƒXƒg
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Math/Matrix.h>
#include <Framework/Core/Math/Vector/include.h>
#include <Framework/Core/Math/Quaternion.h>

using namespace ob;


TEST(Matrix, OperatorSet) {
    Matrix mtxA(
        0, 1, 0, 0,
        1, 0, 0, 0,
        0, 0, 0, 2,
        0, 2, 0, 3);

    Matrix mtxB = mtxA;
    EXPECT_EQ(mtxA, mtxB);
}


TEST(Matrix, OperatorMul) {
    Matrix mtxA(
        0, 1, 0, 0,
        1, 0, 0, 0,
        0, 0, 0, 2,
        0, 2, 0, 3);

    Matrix mtxB(EForceInit::Force);
    mtxB = mtxA * mtxB;
    EXPECT_EQ(mtxA, mtxB);
}

TEST(Matrix, OperatorMulSet) {
    Matrix mtxA(
        0, 1, 0, 0,
        1, 0, 0, 0,
        0, 0, 0, 2,
        0, 2, 0, 3);

    Matrix mtxB(EForceInit::Force);
    mtxB *= mtxA;
    EXPECT_EQ(mtxA, mtxB);
}

TEST(Matrix, GetColumn) {
    Matrix mtxA(
        0, 1, 0, 0,
        1, 0, 0, 0,
        0, 0, 0, 2,
        0, 2, 0, 3);

    Vec4 vec = mtxA.GetColumn(1);
    EXPECT_EQ(vec, Vec4(1, 0, 0, 2));
}

TEST(Matrix, GetRow) {
    Matrix mtxA(
        0, 1, 0, 0,
        1, 0, 0, 0,
        0, 0, 0, 2,
        0, 2, 0, 3);

    Vec4 vec = mtxA.GetRow(3);
    EXPECT_EQ(vec, Vec4(0, 2, 0, 3));
}

TEST(Matrix, Translate) {
    Matrix mtx;
    mtx.SetRow(3, Vec4(1, 2, 3, 1));

    Matrix mtx2;
    mtx2.Translate(1, 2, 3);
    EXPECT_EQ(mtx2, mtx2);
}

TEST(Matrix, Rotate) {
    Matrix mtx(EForceInit::Force);
    mtx.Rotate(0.0f, 90.0f, 0.0f);

    Matrix mtx2(
        0, 0, -1, 0,
        0, 1, 0, 0,
        1, 0, 0, 0,
        0, 0, 0, 1);

    Vec3 v(1, 2, 3);
    auto a = mtx*v;
    auto a2 = mtx2*v;
    EXPECT_EQ(mtx*v, mtx2*v);
}