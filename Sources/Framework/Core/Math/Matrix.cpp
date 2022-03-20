//***********************************************************
//! @file
//! @brief 行列
//! @author Gajumaru
//***********************************************************
#include <Framework/Core/Math/Matrix.h>

#include <Framework/Core/Math/Mathf.h>
#include <Framework/Core/Math/Vector/include.h>
#include <Framework/Core/Math/Quaternion.h>
#include <Framework/Core/Math/Rotation.h>
#include <Framework/Core/Geometory/Frustum.h>

namespace ob::core {

    //==============================================================================
    // Matrix
    //==============================================================================

    const s32 Matrix::COL = 4;
    const s32 Matrix::ROW = 4;



    //@―---------------------------------------------------------------------------
    //! @brief Affine から初期化
    //@―---------------------------------------------------------------------------
    Matrix& Matrix::operator = (const Affine& other) {
        // TODO アフィン行列の変換
        //Matrix mtx;
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 等価演算子
    //@―---------------------------------------------------------------------------
    bool Matrix::operator == (const Matrix& v) const {
        for (u32 y = 0; y < ROW; y++)for (u32 x = 0; x < COL; x++) {
            if (Mathf::IsNearEquals(m[y][x], v.m[y][x]) == false)return false;
        }
        return true;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 否等価演算子 
    //@―---------------------------------------------------------------------------
    bool Matrix::operator != (const Matrix& v) const {
        return !(operator==(v));
    }


    //@―---------------------------------------------------------------------------
    //! @brief 行列演算子
    //@―---------------------------------------------------------------------------
    Matrix Matrix::operator * (const Matrix& other) const {
        Matrix result(EForceInit::Force);
        result.m[0][0] = 0;
        result.m[1][1] = 0;
        result.m[2][2] = 0;
        result.m[3][3] = 0;

        for (s32 x = 0; x < COL; x++) {
            for (s32 y = 0; y < ROW; y++) {
                for (s32 i = 0; i < ROW; i++) {
                    result.m[y][x] += m[i][x] * other.m[y][i];
                }
            }
        }

        return result;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 行列演算代入演算子 
    //@―---------------------------------------------------------------------------
    Matrix Matrix::operator *= (const Matrix& other) {
        Matrix result(EForceInit::Force);
        result.m[0][0] = 0;
        result.m[1][1] = 0;
        result.m[2][2] = 0;
        result.m[3][3] = 0;

        for (s32 x = 0; x < COL; x++) {
            for (s32 y = 0; y < ROW; y++) {
                for (s32 i = 0; i < ROW; i++) {
                    result.m[y][x] += m[i][x] * other.m[y][i];
                }
            }
        }

        (*this) = result;
        return result;
    }


    //@―---------------------------------------------------------------------------
    //! @brief Quat 合成演算子
    //@―---------------------------------------------------------------------------
    Matrix Matrix::operator * (const Quat& other) const {
        Matrix mtx = *this;
        return mtx *= other.getMatrix();
    }


    //@―---------------------------------------------------------------------------
    //! @brief Quat 合成代入演算子
    //@―---------------------------------------------------------------------------
    Matrix Matrix::operator *= (const Quat& other) {
        (*this) *= other.getMatrix();
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief Vec3 合成代入演算子
    //@―---------------------------------------------------------------------------
    Vec3 Matrix::operator* (const Vec3& v)const {
        return Vec3(
            v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0],
            v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1],
            v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2]);
    }

#pragma warning( disable : 6385 )
    //@―---------------------------------------------------------------------------
    //! @brief 行列の行を取得
    //! 
    //! @details 存在しない行を取得しようとした場合はエラー
    //! @param index 行インデックス
    //@―---------------------------------------------------------------------------
    Vec4 Matrix::getColumn(const s32 index)const {
        OB_CHECK_ASSERT(0 <= index && index < 4);
        return Vec4(m[0][index], m[1][index], m[2][index], m[3][index]);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 行列の列を取得
    //! 
    //! @details 存在しない列を取得しようとした場合はエラー
    //! @param index 列インデックス
    //@―---------------------------------------------------------------------------
    Vec4 Matrix::getRow(const s32 index)const {
        OB_CHECK_ASSERT(0 <= index && index < 4);
        return Vec4(m[index][0], m[index][1], m[index][2], m[index][3]);
    }
#pragma warning( default : 6385)


    //@―---------------------------------------------------------------------------
    //! @brief 行を設定
    //! 
    //! @param index 行インデックス
    //! @param column 行ベクトル
    //@―---------------------------------------------------------------------------
    void Matrix::setColumn(const s32 index, const Vec4& column) {
        if (index < 0 || COL <= index)return;
        m[0][index] = column.x;
        m[1][index] = column.y;
        m[2][index] = column.z;
        m[3][index] = column.w;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 列を設定
    //! 
    //! @param index 列インデックス
    //! @param row 列ベクトル
    //@―---------------------------------------------------------------------------
    void Matrix::setRow(const s32 index, const Vec4& column) {
        if (index < 0 || ROW <= index)return;
        m[index][0] = column.x;
        m[index][1] = column.y;
        m[index][2] = column.z;
        m[index][3] = column.w;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 行列値を設定
    //@―---------------------------------------------------------------------------
    void Matrix::set(const f32 m00, const f32 m01, const f32 m02, const f32 m03,
        const f32 m10, const f32 m11, const f32 m12, const f32 m13,
        const f32 m20, const f32 m21, const f32 m22, const f32 m23,
        const f32 m30, const f32 m31, const f32 m32, const f32 m33) {
        m[0][0] = m00;
        m[0][1] = m01;
        m[0][2] = m02;
        m[0][3] = m03;
        m[1][0] = m10;
        m[1][1] = m11;
        m[1][2] = m12;
        m[1][3] = m13;
        m[2][0] = m20;
        m[2][1] = m21;
        m[2][2] = m22;
        m[2][3] = m23;
        m[3][0] = m30;
        m[3][1] = m31;
        m[3][2] = m32;
        m[3][3] = m33;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 行列に平行移動行列を乗算
    //! 
    //! @param position 移動量
    //@―---------------------------------------------------------------------------
    void Matrix::translate(const Vec3& trans) {
        translate(trans.x, trans.y, trans.z);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 行列に回転行列を乗算
    //! 
    //! @param x X軸の回転量
    //! @param y Y軸の回転量
    //! @param z Z軸の回転量
    //@―---------------------------------------------------------------------------
    void Matrix::rotate(const f32 x, const f32 y, const f32 z) {
        const f32 sr = Mathf::SinD(x);
        const f32 cr = Mathf::CosD(x);
        const f32 sp = Mathf::SinD(y);
        const f32 cp = Mathf::CosD(y);
        const f32 sy = Mathf::SinD(z);
        const f32 cy = Mathf::CosD(z);

        Matrix mat(
            cy * cp, sy * cp, -sp, 0,
            cy * sp * sr - sy * cr, sy * sp * sr + cy * cr, sp * sr, 0,
            cy * sp * cr + sy * sr, sy * sp * cr - cy * sr, cp * cr, 0,
            0, 0, 0, 1);
        (*this) *= mat;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 行列に回転行列を乗算
    //! 
    //! @param eulerAngles 回転量
    //@―---------------------------------------------------------------------------
    void Matrix::rotate(const Rot& rotation) {
        rotate(rotation.roll, rotation.pitch, rotation.yaw);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 行列に回転行列を乗算
    //! 
    //! @param eulerAngles 回転量
    //@―---------------------------------------------------------------------------
    void Matrix::rotate(const Vec3& eulerAngles) {
        rotate(eulerAngles.x, eulerAngles.y, eulerAngles.z);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 行列に回転行列を乗算
    //! 
    //! @param rotation 回転量
    //@―---------------------------------------------------------------------------
    void Matrix::rotate(const Quat& quat) {
        (*this) *= quat.getMatrix();
    }


    //@―---------------------------------------------------------------------------
    //! @brief 行列に拡大縮小行列を乗算
    //! 
    //! @param scale 拡大縮小量
    //@―---------------------------------------------------------------------------
    void Matrix::scale(const Vec3& scale) {
        this->scale(scale.x, scale.y, scale.z);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 行列式の計算
    //@―---------------------------------------------------------------------------
    f32 Matrix::getDeterminant()const {
        f32 ret = 0;
        for (s32 i = 0; i < ROW; i++) {
            f32 tmp = 1;
            f32 tmp2 = 1;
            for (s32 j = 0; j < COL; j++) {
                tmp *= m[(i + j) % COL][j];
                tmp2 *= m[(i + j) % COL][COL - j - 1];
            }
            ret += tmp - tmp2;
        }
        return ret;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 逆行列の計算
    //! 
    //! @detail 逆行列が計算できない場合は単位行列を返す
    //@―---------------------------------------------------------------------------
    bool Matrix::getInverse(Matrix& dest)const {
        f32 det = getDeterminant();
        if (Mathf::Abs(det) <= Mathf::EPSILON)return false;

        f32 invDet = 1.0f / det;
        Matrix temp;
        temp.m[0][0] = invDet * (m[1][1] * m[2][2] * m[3][3] + m[1][2] * m[2][3] * m[3][1] + m[1][3] * m[2][1] * m[3][2] - m[1][1] * m[2][3] * m[3][2] - m[1][2] * m[2][1] * m[3][3] - m[1][3] * m[2][2] * m[3][1]);
        temp.m[0][1] = invDet * (m[0][1] * m[2][3] * m[3][2] + m[0][2] * m[2][1] * m[3][3] + m[0][3] * m[2][2] * m[3][1] - m[0][1] * m[2][2] * m[3][3] - m[0][2] * m[2][3] * m[3][1] - m[0][3] * m[2][1] * m[3][2]);
        temp.m[0][2] = invDet * (m[0][1] * m[1][2] * m[3][3] + m[0][2] * m[1][3] * m[3][1] + m[0][3] * m[1][1] * m[3][2] - m[0][1] * m[1][3] * m[3][2] - m[0][2] * m[1][1] * m[3][3] - m[0][3] * m[1][2] * m[3][1]);
        temp.m[0][3] = invDet * (m[0][1] * m[1][3] * m[2][2] + m[0][2] * m[1][1] * m[2][3] + m[0][3] * m[1][2] * m[2][1] - m[0][1] * m[1][2] * m[2][3] - m[0][2] * m[1][3] * m[2][1] - m[0][3] * m[1][1] * m[2][2]);

        temp.m[1][0] = invDet * (m[1][0] * m[2][3] * m[3][2] + m[1][2] * m[2][0] * m[3][3] + m[1][3] * m[2][2] * m[3][0] - m[1][0] * m[2][2] * m[3][3] - m[1][2] * m[2][3] * m[3][0] - m[1][3] * m[2][0] * m[3][2]);
        temp.m[1][1] = invDet * (m[0][0] * m[2][2] * m[3][3] + m[0][2] * m[2][3] * m[3][0] + m[0][3] * m[2][0] * m[3][2] - m[0][0] * m[2][3] * m[3][2] - m[0][2] * m[2][0] * m[3][3] - m[0][3] * m[2][2] * m[3][0]);
        temp.m[1][2] = invDet * (m[0][0] * m[1][3] * m[3][2] + m[0][2] * m[1][0] * m[3][3] + m[0][3] * m[1][2] * m[3][0] - m[0][0] * m[1][2] * m[3][3] - m[0][2] * m[1][3] * m[3][0] - m[0][3] * m[1][0] * m[3][2]);
        temp.m[1][3] = invDet * (m[0][0] * m[1][2] * m[2][3] + m[0][2] * m[1][3] * m[2][0] + m[0][3] * m[1][0] * m[2][2] - m[0][0] * m[1][3] * m[2][2] - m[0][2] * m[1][0] * m[2][3] - m[0][3] * m[1][2] * m[2][0]);

        temp.m[2][0] = invDet * (m[1][0] * m[2][1] * m[3][3] + m[1][1] * m[2][3] * m[3][0] + m[1][3] * m[2][0] * m[3][1] - m[1][0] * m[2][3] * m[3][1] - m[1][1] * m[2][0] * m[3][3] - m[1][3] * m[2][1] * m[3][0]);
        temp.m[2][1] = invDet * (m[0][0] * m[2][3] * m[3][1] + m[0][1] * m[2][0] * m[3][3] + m[0][3] * m[2][1] * m[3][0] - m[0][0] * m[2][1] * m[3][3] - m[0][1] * m[2][3] * m[3][0] - m[0][3] * m[2][0] * m[3][1]);
        temp.m[2][2] = invDet * (m[0][0] * m[1][1] * m[3][3] + m[0][1] * m[1][3] * m[3][0] + m[0][3] * m[1][0] * m[3][1] - m[0][0] * m[1][3] * m[3][1] - m[0][1] * m[1][0] * m[3][3] - m[0][3] * m[1][1] * m[3][0]);
        temp.m[2][3] = invDet * (m[0][0] * m[1][3] * m[2][1] + m[0][1] * m[1][0] * m[2][3] + m[0][3] * m[1][1] * m[2][0] - m[0][0] * m[1][1] * m[2][3] - m[0][1] * m[1][3] * m[2][0] - m[0][3] * m[1][0] * m[2][1]);

        temp.m[3][0] = invDet * (m[1][0] * m[2][2] * m[3][1] + m[1][1] * m[2][0] * m[3][2] + m[1][2] * m[2][1] * m[3][0] - m[1][0] * m[2][1] * m[3][2] - m[1][1] * m[2][2] * m[3][0] - m[1][2] * m[2][0] * m[3][1]);
        temp.m[3][1] = invDet * (m[0][0] * m[2][1] * m[3][2] + m[0][1] * m[2][2] * m[3][0] + m[0][2] * m[2][0] * m[3][1] - m[0][0] * m[2][2] * m[3][1] - m[0][1] * m[2][0] * m[3][2] - m[0][2] * m[2][1] * m[3][0]);
        temp.m[3][2] = invDet * (m[0][0] * m[1][2] * m[3][1] + m[0][1] * m[1][0] * m[3][2] + m[0][2] * m[1][1] * m[3][0] - m[0][0] * m[1][1] * m[3][2] - m[0][1] * m[1][2] * m[3][0] - m[0][2] * m[1][0] * m[3][1]);
        temp.m[3][3] = invDet * (m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] + m[0][2] * m[1][0] * m[2][1] - m[0][0] * m[1][2] * m[2][1] - m[0][1] * m[1][0] * m[2][2] - m[0][2] * m[1][1] * m[2][0]);

        dest = temp;

        return true;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      転置行列の計算
    //! 
    //! @note       直交行列の転置行列は逆行列となる。
    //@―---------------------------------------------------------------------------
    Matrix Matrix::getTranspose()const {
        Matrix ret;
        for (s32 i = 0; i < COL; i++) {
            for (s32 j = 0; j < ROW; j++) {
                ret.m[i][j] = m[j][i];
            }
        }
        return ret;
    }

    //@―---------------------------------------------------------------------------
    //! @brief 行列から回転量を計算
    //@―---------------------------------------------------------------------------
    Vec3 Matrix::getTrans()const {
        return Vec3(m[0][3], m[1][3], m[2][3]);
    }

    //@―---------------------------------------------------------------------------
    //! @brief 行列から回転量を計算
    //@―---------------------------------------------------------------------------
    Rot Matrix::getRot()const {
        return getQuat().getRot();
    }

    //@―---------------------------------------------------------------------------
    //! @brief 行列から拡大量を計算
    //@―---------------------------------------------------------------------------
    Vec3 Matrix::getScale()const {
        Vec3 sxr = (*this) * Vec3(1.0f, 0.0f, 0.0f);
        Vec3 syr = (*this) * Vec3(0.0f, 1.0f, 0.0f);
        Vec3 szr = (*this) * Vec3(0.0f, 0.0f, 1.0f);
        return Vec3(sxr.length(), syr.length(), szr.length());
    }


    //@―---------------------------------------------------------------------------
    //! @brief 行列から回転量を計算
    //@―---------------------------------------------------------------------------
    Quat Matrix::getQuat()const {
        // TODO 修正

        // 最大成分を検索
        f32 elem[4]; // 0:x, 1:y, 2:z, 3:w
        elem[0] = m[0][0] - m[1][1] - m[2][2] + 1.0f;
        elem[1] = -m[0][0] + m[1][1] - m[2][2] + 1.0f;
        elem[2] = -m[0][0] - m[1][1] + m[2][2] + 1.0f;
        elem[3] = m[0][0] + m[1][1] + m[2][2] + 1.0f;

        s32 biggestIndex = 0;
        for (s32 i = 1; i < 4; i++) {
            if (elem[i] > elem[biggestIndex])
                biggestIndex = i;
        }

        if (elem[biggestIndex] < 0.0f)
            return Quat(); // 引数の行列に間違いあり

        // 最大要素の値を算出
        Quat quat;
        f32* q[4] = { &quat.x, &quat.y, &quat.z, &quat.w };
        f32 v = sqrtf(elem[biggestIndex]) * 0.5f;
        *q[biggestIndex] = v;
        f32 mult = 0.25f / v;

        switch (biggestIndex) {
        case 0: // x
            *q[1] = (m[1][2] + m[2][1]) * mult;
            *q[2] = (m[3][1] + m[1][3]) * mult;
            *q[3] = (m[2][3] - m[3][2]) * mult;
            break;
        case 1: // y
            *q[0] = (m[1][2] + m[2][1]) * mult;
            *q[2] = (m[2][3] + m[3][2]) * mult;
            *q[3] = (m[3][1] - m[1][3]) * mult;
            break;
        case 2: // z
            *q[0] = (m[3][1] + m[1][3]) * mult;
            *q[1] = (m[2][3] + m[3][2]) * mult;
            *q[3] = (m[1][2] - m[2][1]) * mult;
            break;
        case 3: // w
            *q[0] = (m[2][3] - m[3][2]) * mult;
            *q[1] = (m[3][1] - m[1][3]) * mult;
            *q[2] = (m[1][2] - m[2][1]) * mult;
            break;
        }
        return quat;
    }





    //==============================================================================
    // MatrixHelper
    //==============================================================================

    //@―---------------------------------------------------------------------------
    //! @brief 視錐台からビュー行列の生成
    //@―---------------------------------------------------------------------------
    Matrix MatrixHelper::CreateFrustum(Frustum frustum) {

        Matrix ret(
            2 * frustum.zNear / (frustum.right - frustum.left), 0, (frustum.right + frustum.left) / (frustum.right - frustum.left), 0,
            0, 2 * frustum.zNear / (frustum.top - frustum.bottom), (frustum.top + frustum.bottom) / (frustum.top - frustum.bottom), 0,
            0, 0, -(frustum.zFar + frustum.zNear) / (frustum.zFar - frustum.zNear), -2 * frustum.zFar * frustum.zNear / (frustum.zFar - frustum.zNear),
            0, 0, -1, 0
        );
        return ret;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 透視投影行列の生成
    //@―---------------------------------------------------------------------------
    Matrix MatrixHelper::CreatePerspective(const f32 fov, const f32 aspect, const f32 zNear, const f32 zFar) {
        Matrix matrix;
        f32 f = 1.0f / Mathf::Tan(Mathf::Degrees(fov * 0.5f));
        f32 dz = zFar - zNear;
        matrix.m[0][0] = f / aspect;
        matrix.m[1][1] = f;
        matrix.m[2][2] = (zFar + zNear) / dz;
        matrix.m[2][3] = -2.0f * zFar * zNear / dz;
        matrix.m[3][2] = 1.0f;
        matrix.m[3][3] = 0.0f;
        return matrix;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  平行投影行列の生成
    //@―---------------------------------------------------------------------------
    Matrix MatrixHelper::CreateOrtho(const f32 left, const f32 right, const f32 bottom, const f32 top, const f32 zNear, const f32 zFar) {
        Matrix ret(
            2 / (right - left), 0, 0, -(right + left) / (right - left),
            0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
            0, 0, -2 / (zFar - zNear), -(zFar + zNear) / (zFar - zNear),
            0, 0, 0, 1
        );
        return ret;
    }







}// namespace ob