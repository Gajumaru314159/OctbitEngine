//***********************************************************
//! @file
//! @brief		Matrix �̃e�X�g
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Math/Matrix.h>
#include <Framework/Core/Math/Vectors.h>
using namespace ob;

TEST_GROUP(Matrix) {
};

TEST(Matrix, OperatorSet) {
    Matrix mtxA(
        0, 1, 0, 0,
        1, 0, 0, 0,
        0, 0, 0, 2,
        0, 2, 0, 3);

    Matrix mtxB = mtxA;
    CHECK_TRUE(mtxA == mtxB);
}


TEST(Matrix, OperatorMul) {
    Matrix mtxA(
        0, 1, 0, 0,
        1, 0, 0, 0,
        0, 0, 0, 2,
        0, 2, 0, 3);

    Matrix mtxB = Matrix::Identity;
    mtxB = mtxA * mtxB;
    CHECK_TRUE(mtxA == mtxB);
}

TEST(Matrix, OperatorMulSet) {
    Matrix mtxA(
        0, 1, 0, 0,
        1, 0, 0, 0,
        0, 0, 0, 2,
        0, 2, 0, 3);

    Matrix mtxB = Matrix::Identity;
    mtxB *= mtxA;
    CHECK_TRUE(mtxA == mtxB);
}

TEST(Matrix, GetColumn) {
    Matrix mtxA(
        0, 1, 0, 0,
        1, 0, 0, 0,
        0, 0, 0, 2,
        0, 2, 0, 3);

    Vec4 vec = mtxA.getColumn(1);
    CHECK_TRUE(vec == Vec4(1, 0, 0, 2));
}

TEST(Matrix, GetRow) {
    Matrix mtxA(
        0, 1, 0, 0,
        1, 0, 0, 0,
        0, 0, 0, 2,
        0, 2, 0, 3);

    Vec4 vec = mtxA.getRow(3);
    CHECK_TRUE(vec == Vec4(0, 2, 0, 3));
}

TEST(Matrix, Translate) {
    Matrix mtx;
    mtx.setRow(3, Vec4(1, 2, 3, 1));

    Matrix mtx2;
    mtx2.translate(1, 2, 3);
    CHECK_TRUE(mtx2 == mtx2);
}

TEST(Matrix, Rotate) {
    Matrix mtx = Matrix::Identity;
    mtx.rotate(0.0f, 90.0f, 0.0f);

    Matrix mtx2(
        0, 0, 1, 0,
        0, 1, 0, 0,
        -1, 0, 0, 0,
        0, 0, 0, 1);

    Vec3 v(1, 2, 3);
    auto a = mtx*v;
    auto a2 = mtx2*v;
    CHECK_TRUE(mtx*v == mtx2*v);
}