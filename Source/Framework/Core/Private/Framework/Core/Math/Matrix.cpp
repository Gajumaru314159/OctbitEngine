//***********************************************************
//! @file
//! @brief 行列
//! @author Gajumaru
//***********************************************************
#include <Framework/Core/Math/Matrix.h>

#include <Framework/Core/Math/Math.h>
#include <Framework/Core/Math/Vectors.h>
#include <Framework/Core/Math/Quaternion.h>
#include <Framework/Core/Math/Rotation.h>
#include <Framework/Core/Geometory/Frustum.h>
#include <Framework/Core/Log/Assertion.h>

namespace ob::core {

	const Matrix Matrix::Identity = Matrix(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);


	//@―---------------------------------------------------------------------------
	//! @brief 等価演算子
	//@―---------------------------------------------------------------------------
	bool Matrix::operator == (const Matrix& v) const {
		for (u32 y = 0; y < ROW; y++)for (u32 x = 0; x < COL; x++) {
			if (Math::IsNearEquals(m[y][x], v.m[y][x]) == false)return false;
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
		return Matrix(*this)*=other;
	}


	//@―---------------------------------------------------------------------------
	//! @brief 行列演算代入演算子 
	//@―---------------------------------------------------------------------------
	Matrix& Matrix::operator *= (const Matrix& other) {

		Matrix result(
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0);

		for (s32 x = 0; x < COL; x++) {
			for (s32 y = 0; y < ROW; y++) {
				for (s32 i = 0; i < ROW; i++) {
					result.m[y][x] += m[y][i] * other.m[i][x];
				}
			}
		}

		return *this = result;
	}


	//@―---------------------------------------------------------------------------
	//! @brief Quat 合成演算子
	//@―---------------------------------------------------------------------------
	Matrix Matrix::operator * (const Quat& other) const {
		return other.toMatrix() *= other.toMatrix();
	}


	//@―---------------------------------------------------------------------------
	//! @brief Quat 合成代入演算子
	//@―---------------------------------------------------------------------------
	Matrix& Matrix::operator *= (const Quat& other) {
		return *this *= other.toMatrix();
	}


	//@―---------------------------------------------------------------------------
	//! @brief Vec3 合成代入演算子
	//@―---------------------------------------------------------------------------
	Vec3 Matrix::operator* (const Vec3& v)const {
		return Vec3(
			v.x * m00 + v.y * m01 + v.z * m02 + m03,
			v.x * m10 + v.y * m11 + v.z * m12 + m13,
			v.x * m20 + v.y * m21 + v.z * m22 + m23);
	}

#pragma warning( disable : 6385 )
	//@―---------------------------------------------------------------------------
	//! @brief 行列の行を取得
	//! 
	//! @details 存在しない行を取得しようとした場合はエラー
	//! @param index 行インデックス
	//@―---------------------------------------------------------------------------
	Vec4 Matrix::getColumn(const s32 index)const {
		OB_CHECK_ASSERT_EXPR(0 <= index && index < 4);
		return Vec4(m[0][index], m[1][index], m[2][index], m[3][index]);
	}


	//@―---------------------------------------------------------------------------
	//! @brief 行列の列を取得
	//! 
	//! @details 存在しない列を取得しようとした場合はエラー
	//! @param index 列インデックス
	//@―---------------------------------------------------------------------------
	Vec4 Matrix::getRow(const s32 index)const {
		OB_CHECK_ASSERT_EXPR(0 <= index && index < 4);
		return Vec4(m[index][0], m[index][1], m[index][2], m[index][3]);
	}
#pragma warning( default : 6385)


	//@―---------------------------------------------------------------------------
	//! @brief 行を設定
	//! 
	//! @param index 行インデックス
	//! @param column 行ベクトル
	//@―---------------------------------------------------------------------------
	Matrix& Matrix::setColumn(s32 index, Vec4 column) {
		if (index < 0 || COL <= index)return *this;
		m[0][index] = column.x;
		m[1][index] = column.y;
		m[2][index] = column.z;
		m[3][index] = column.w;
		return *this;
	}


	//@―---------------------------------------------------------------------------
	//! @brief 列を設定
	//! 
	//! @param index 列インデックス
	//! @param row 列ベクトル
	//@―---------------------------------------------------------------------------
	Matrix& Matrix::setRow(s32 index, Vec4 column) {
		if (index < 0 || ROW <= index)return *this;
		m[index][0] = column.x;
		m[index][1] = column.y;
		m[index][2] = column.z;
		m[index][3] = column.w;
		return *this;
	}

	//@―---------------------------------------------------------------------------
	//! @brief 行列から回転量を計算
	//@―---------------------------------------------------------------------------
	Vec3 Matrix::getTrans()const {
		return Vec3(m03, m13, m23);
	}


	//@―---------------------------------------------------------------------------
	//! @brief 行列から回転量を計算
	//@―---------------------------------------------------------------------------
	Rot Matrix::getRot()const {
		return getQuat().toRot();
	}


	//@―---------------------------------------------------------------------------
	//! @brief 行列から拡大量を計算
	//@―---------------------------------------------------------------------------
	Vec3 Matrix::getScale()const {
		Vec3 sxr = (*this) * Vec3(1, 0, 0);
		Vec3 syr = (*this) * Vec3(0, 1, 0);
		Vec3 szr = (*this) * Vec3(0, 0, 1);
		return Vec3(sxr.length(), syr.length(), szr.length());
	}


	//@―---------------------------------------------------------------------------
	//! @brief 行列から回転量を計算
	//@―---------------------------------------------------------------------------
	Quat Matrix::getQuat()const {

		// 最大成分を検索
		f32 elem[4];
		elem[0] = m00 - m11 - m22 + 1;
		elem[1] = -m00 + m11 - m22 + 1;
		elem[2] = -m00 - m11 + m22 + 1;
		elem[3] = m00 + m11 + m22 + 1;

		s32 biggestIndex = 0;
		for (s32 i = 1; i < 4; i++) {
			if (elem[i] > elem[biggestIndex])
				biggestIndex = i;
		}

		if (elem[biggestIndex] < 0.0f) {
			// 引数の行列に間違いあり
			return Quat::Identity; 
		}

		// 最大要素の値を算出
		Quat quat;
		f32* q[4] = { &quat.x, &quat.y, &quat.z, &quat.w };
		f32 v = Math::Sqrt(elem[biggestIndex]) * 0.5f;
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


	//@―---------------------------------------------------------------------------
	//! @brief 行列式の計算
	//@―---------------------------------------------------------------------------
	f32 Matrix::determinant()const {
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
	//! @brief 逆行列を持つか
	//@―---------------------------------------------------------------------------
	bool Matrix::hasInverse()const {
		f32 det = determinant();
		return Math::EPSILON < Math::Abs(det);
	}

	//@―---------------------------------------------------------------------------
	//! @brief 逆行列
	//! 
	//! @detail 逆行列が計算できない場合は単位行列を返す
	//@―---------------------------------------------------------------------------
	Matrix Matrix::inverse()const {
		f32 det = determinant();
		if (Math::Abs(det) <= Math::EPSILON)return Matrix::Identity;

		f32 invDet = 1.0f / det;
		Matrix temp;
		temp.m00 = invDet * (m11 * m22 * m33 + m12 * m23 * m31 + m13 * m21 * m32 - m11 * m23 * m32 - m12 * m21 * m33 - m13 * m22 * m31);
		temp.m01 = invDet * (m01 * m23 * m32 + m02 * m21 * m33 + m03 * m22 * m31 - m01 * m22 * m33 - m02 * m23 * m31 - m03 * m21 * m32);
		temp.m02 = invDet * (m01 * m12 * m33 + m02 * m13 * m31 + m03 * m11 * m32 - m01 * m13 * m32 - m02 * m11 * m33 - m03 * m12 * m31);
		temp.m03 = invDet * (m01 * m13 * m22 + m02 * m11 * m23 + m03 * m12 * m21 - m01 * m12 * m23 - m02 * m13 * m21 - m03 * m11 * m22);
		temp.m10 = invDet * (m10 * m23 * m32 + m12 * m20 * m33 + m13 * m22 * m30 - m10 * m22 * m33 - m12 * m23 * m30 - m13 * m20 * m32);
		temp.m11 = invDet * (m00 * m22 * m33 + m02 * m23 * m30 + m03 * m20 * m32 - m00 * m23 * m32 - m02 * m20 * m33 - m03 * m22 * m30);
		temp.m12 = invDet * (m00 * m13 * m32 + m02 * m10 * m33 + m03 * m12 * m30 - m00 * m12 * m33 - m02 * m13 * m30 - m03 * m10 * m32);
		temp.m13 = invDet * (m00 * m12 * m23 + m02 * m13 * m20 + m03 * m10 * m22 - m00 * m13 * m22 - m02 * m10 * m23 - m03 * m12 * m20);
		temp.m20 = invDet * (m10 * m21 * m33 + m11 * m23 * m30 + m13 * m20 * m31 - m10 * m23 * m31 - m11 * m20 * m33 - m13 * m21 * m30);
		temp.m21 = invDet * (m00 * m23 * m31 + m01 * m20 * m33 + m03 * m21 * m30 - m00 * m21 * m33 - m01 * m23 * m30 - m03 * m20 * m31);
		temp.m22 = invDet * (m00 * m11 * m33 + m01 * m13 * m30 + m03 * m10 * m31 - m00 * m13 * m31 - m01 * m10 * m33 - m03 * m11 * m30);
		temp.m23 = invDet * (m00 * m13 * m21 + m01 * m10 * m23 + m03 * m11 * m20 - m00 * m11 * m23 - m01 * m13 * m20 - m03 * m10 * m21);
		temp.m30 = invDet * (m10 * m22 * m31 + m11 * m20 * m32 + m12 * m21 * m30 - m10 * m21 * m32 - m11 * m22 * m30 - m12 * m20 * m31);
		temp.m31 = invDet * (m00 * m21 * m32 + m01 * m22 * m30 + m02 * m20 * m31 - m00 * m22 * m31 - m01 * m20 * m32 - m02 * m21 * m30);
		temp.m32 = invDet * (m00 * m12 * m31 + m01 * m10 * m32 + m02 * m11 * m30 - m00 * m11 * m32 - m01 * m12 * m30 - m02 * m10 * m31);
		temp.m33 = invDet * (m00 * m11 * m22 + m01 * m12 * m20 + m02 * m10 * m21 - m00 * m12 * m21 - m01 * m10 * m22 - m02 * m11 * m20);

		return temp;
	}


	//@―---------------------------------------------------------------------------
	//! @brief 行列に平行移動行列を乗算
	//! 
	//! @param x X軸方向の移動量
	//! @param y Y軸方向の移動量
	//! @param z Z軸方向の移動量
	//@―---------------------------------------------------------------------------
	Matrix& Matrix::translate(f32 x, f32 y, f32 z) {
		return *this *= Translate(x, y, z);
	}


	//@―---------------------------------------------------------------------------
	//! @brief 行列に平行移動行列を乗算
	//! 
	//! @param position 移動量
	//@―---------------------------------------------------------------------------
	Matrix& Matrix::translate(Vec3 trans) {
		return *this *= Translate(trans);
	}


	//@―---------------------------------------------------------------------------
	//! @brief 行列に回転行列を乗算
	//! 
	//! @param x X軸の回転量
	//! @param y Y軸の回転量
	//! @param z Z軸の回転量
	//@―---------------------------------------------------------------------------
	Matrix& Matrix::rotate(f32 x, f32 y, f32 z) {
		return *this *= Rotate(x, y, z);
	}


	//@―---------------------------------------------------------------------------
	//! @brief 行列に回転行列を乗算
	//! 
	//! @param eulerAngles 回転量
	//@―---------------------------------------------------------------------------
	Matrix& Matrix::rotate(Rot rotation) {
		return *this *= Rotate(rotation);
	}


	//@―---------------------------------------------------------------------------
	//! @brief 行列に回転行列を乗算
	//! 
	//! @param rotation 回転量
	//@―---------------------------------------------------------------------------
	Matrix& Matrix::rotate(Quat quat) {
		return *this *= Rotate(quat);
	}


	//@―---------------------------------------------------------------------------
	//! @brief 行列に拡大縮小行列を乗算
	//! 
	//! @param scale 拡大縮小量
	//@―---------------------------------------------------------------------------
	Matrix& Matrix::scale(Vec3 scale) {
		return *this *= Scale(scale);
	}

	//@―---------------------------------------------------------------------------
	//! @brief 行列に拡大縮小行列を乗算
	//! 
	//! @param x X軸方向の拡大縮小量
	//! @param y Y軸方向の拡大縮小量
	//! @param z Z軸方向の拡大縮小量
	//@―---------------------------------------------------------------------------
	Matrix& Matrix::scale(f32 x, f32 y, f32 z) {
		return *this *= Scale(x, y, z);
	}


	//@―---------------------------------------------------------------------------
	//! @brief 回転行列
	//! 
	//! @param x X軸の回転量
	//! @param y Y軸の回転量
	//! @param z Z軸の回転量
	//@―---------------------------------------------------------------------------
	Matrix Matrix::Rotate(f32 x, f32 y, f32 z) {
		const f32 sr = Math::SinD(x);
		const f32 cr = Math::CosD(x);
		const f32 sp = Math::SinD(y);
		const f32 cp = Math::CosD(y);
		const f32 sy = Math::SinD(z);
		const f32 cy = Math::CosD(z);

		return Matrix(
			cy * cp, sy * cp, -sp, 0,
			cy * sp * sr - sy * cr, sy * sp * sr + cy * cr, sp * sr, 0,
			cy * sp * cr + sy * sr, sy * sp * cr - cy * sr, cp * cr, 0,
			0, 0, 0, 1);
	}




	//==============================================================================
	// MatrixHelper
	//==============================================================================

	//@―---------------------------------------------------------------------------
	//! @brief 視錐台からビュー行列の生成
	//@―---------------------------------------------------------------------------
	Matrix MatrixHelper::CreateFrustum(Frustum frustum) {
		return Matrix(
			2 * frustum.zNear / (frustum.right - frustum.left), 0, (frustum.right + frustum.left) / (frustum.right - frustum.left), 0,
			0, 2 * frustum.zNear / (frustum.top - frustum.bottom), (frustum.top + frustum.bottom) / (frustum.top - frustum.bottom), 0,
			0, 0, -(frustum.zFar + frustum.zNear) / (frustum.zFar - frustum.zNear), -2 * frustum.zFar * frustum.zNear / (frustum.zFar - frustum.zNear),
			0, 0, -1, 0
		);
	}


	//@―---------------------------------------------------------------------------
	//! @brief 透視投影行列の生成
	//@―---------------------------------------------------------------------------
	Matrix MatrixHelper::CreatePerspective(const f32 fov, const f32 aspect, const f32 zNear, const f32 zFar) {
		Matrix matrix = Matrix::Identity;
		f32 f = 1.0f / Math::Tan(Math::Degrees(fov * 0.5f));
		f32 dz = zFar - zNear;
		matrix.m00 = f;
		matrix.m11 = f * aspect;
		matrix.m22 = (zFar + zNear) / dz;
		matrix.m33 = 0.0f;

		matrix.m32 = 1.0f;
		matrix.m23 = -2.0f * zFar * zNear / dz;
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