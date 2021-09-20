//***********************************************************
//! @file
//! @brief 行列
//! @author Gajumaru
//***********************************************************
#pragma once

namespace ob {

    struct Vec3;
    struct Vec4;
    struct Quat;
    struct Affine;


    //@―---------------------------------------------------------------------------
    //! @brief 視錐台構造体
    //@―---------------------------------------------------------------------------
    struct FrustumDesc {
        f32 left;	//!< 左座標
        f32 right;	//!< 右座標
        f32 bottom;	//!< 下座標
        f32 top;	//!< 上座標
        f32 zNear;	//!< ニアクリッピング面
        f32 zFar;	//!< ファークリッピング面
    };



    //@―---------------------------------------------------------------------------
    //! @brief      行列クラス
    //! 
    //! @details    行列は数学的な定義は行優先、メモリレイアウトは列優先です。<br>
    //!             行列A、B、Cとベクトルvがあるとき、数学表記では<br>
    //!             ABCv=A(B(Cv))<br>
    //!             となりますが、プログラムだと<br>
    //!             ABCv=((AB)C)v<br>
    //!             となり行列同士の計算が先になってしまうため<br>
    //!             vCBA<br>
    //!             というように行優先で記述します。<br>
    //@―---------------------------------------------------------------------------
    struct OB_API Matrix {
    public:

        //@―---------------------------------------------------------------------------
        // コンストラクタ / デストラクタ
        //@―---------------------------------------------------------------------------
        Matrix();                                           // 単位行列に初期化
        Matrix(f32 m00, f32 m01, f32 m02, f32 m03,          // 要素を指定して初期化
            f32 m10, f32 m11, f32 m12, f32 m13,
            f32 m20, f32 m21, f32 m22, f32 m23,
            f32 m30, f32 m31, f32 m32, f32 m33);
        //explicit Matrix(const Affine& affine);

        //@―---------------------------------------------------------------------------
        // オペレータ
        //@―---------------------------------------------------------------------------
        Matrix& operator = (const Affine& other);           // Affine から初期化
        bool    operator == (const Matrix& v) const;        // 等価演算子
        bool    operator != (const Matrix& v) const;        // 否等価演算子
        Matrix  operator * (const Matrix& other) const;     // 行列合成演算子
        Matrix  operator *= (const Matrix& o);              // 行列合成代入演算子
        Matrix  operator * (const Quat& other) const;       // Quat合成演算子
        Matrix  operator *= (const Quat& other);            // Quat合成代入演算子
        Vec3    operator* (const Vec3& v)const;             // Vec3合成代入演算子


        //@―---------------------------------------------------------------------------
        // ゲッター / セッター
        //@―---------------------------------------------------------------------------
        Vec4 GetColumn(s32 index)const;					    // 行の取得
        Vec4 GetRow(s32 index)const;						// 列の取得

        void SetColumn(s32 index, const Vec4& column);	    // 行の設定
        void SetRow(s32 index, const Vec4& row);			// 列の設定

        void Set(f32 m00, f32 m01, f32 m02, f32 m03,		// 行列値の設定
            f32 m10, f32 m11, f32 m12, f32 m13,
            f32 m20, f32 m21, f32 m22, f32 m23,
            f32 m30, f32 m31, f32 m32, f32 m33);

        void SetIdentity();									// 行列に単位行列を設定

        f32		GetDeterminant()const;						// 行列式の計算
        Matrix	GettrueInverse()const;						// 逆行列の計算
        Matrix	GetTranspose()const;						// 転置行列の計算

        Vec3 GetTrans()const;							    // 行列から移動量を取得
        Vec3 Rotation()const;							    // 行列から回転量を取得(Degree)
        Vec3 GetScale()const;							    // 行列から拡大量を取得
        Quat GetQuat()const;							    // 行列から回転量を取得


        //@―---------------------------------------------------------------------------
        // 変換
        //@―---------------------------------------------------------------------------
        Matrix	Inverse()const;							    // 逆行列の計算(アフィン行列で高速に動作)
        void Translate(f32 x, f32 y, f32 z);				// 行列に平行移動行列を乗算
        void Translate(const Vec3& pos);					// 行列に平行移動行列を乗算
        void Rotate(f32 x, f32 y, f32 z);					// 行列に回転行列を乗算(Degree)
        void Rotate(const Vec3& rot);					    // 行列に回転行列を乗算(Degree)
        void Rotate(const Quat& quat);				        // 行列に回転行列を乗算
        void Scale(f32 x, f32 y, f32 z);					// 行列に拡大縮小行列を乗算
        void Scale(const Vec3& scale);					    // 行列に拡大縮小行列を乗算


    public:

        union {
            struct {
                f32 m11, m12, m13, m14;
                f32 m21, m22, m23, m24;
                f32 m31, m32, m33, m34;
                f32 m41, m42, m43, m44;
            };
            f32 m[4][4];//!< 内部データ
        };

    private:

        static const s32 COL;//! 4
        static const s32 ROW;//! 4

    };


    //@―---------------------------------------------------------------------------
    //! @brief 特殊な行列を生成するヘルパークラス
    //@―---------------------------------------------------------------------------
    class MatrixHelper {
    public:

        static Matrix Frustum(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar);   // 視錐台からビュー行列の生成
        static Matrix Frustum(FrustumDesc desc);                                                // 視錐台からビュー行列の生成
        static Matrix Perspective(f32 fov, f32 aspect, f32 zNear, f32 zFar);                    // 透視投影行列の生成
        static Matrix Ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar);     //平行投影行列の生成

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief		単位行列に初期化
    //! @details	行列を単位行列で初期化する。
    //@―---------------------------------------------------------------------------
    inline Matrix::Matrix() {
        SetIdentity();
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素を指定して初期化
    //@―---------------------------------------------------------------------------
    inline Matrix::Matrix(const f32 m00, const f32 m01, const f32 m02, const f32 m03,
        const f32 m10, const f32 m11, const f32 m12, const f32 m13,
        const f32 m20, const f32 m21, const f32 m22, const f32 m23,
        const f32 m30, const f32 m31, const f32 m32, const f32 m33) {
        Set(
            m00, m01, m02, m03,
            m10, m11, m12, m13,
            m20, m21, m22, m23,
            m30, m31, m32, m33);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 行列に単位行列を設定
    //@―---------------------------------------------------------------------------
    inline void Matrix::SetIdentity() {
        memset(m, 0, sizeof(m));
        m[0][0] = 1;
        m[1][1] = 1;
        m[2][2] = 1;
        m[3][3] = 1;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 行列に平行移動行列を乗算
    //! @param x X軸方向の移動量
    //! @param y Y軸方向の移動量
    //! @param z Z軸方向の移動量
    //@―---------------------------------------------------------------------------
    inline void Matrix::Translate(const f32 x, const f32 y, const f32 z) {
        Matrix mat(
            1, 0, 0, x,
            0, 1, 0, y,
            0, 0, 1, z,
            0, 0, 0, 1);
        (*this) *= mat;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 行列に拡大縮小行列を乗算
    //! @param x X軸方向の拡大縮小量
    //! @param y Y軸方向の拡大縮小量
    //! @param z Z軸方向の拡大縮小量
    //@―---------------------------------------------------------------------------
    inline void Matrix::Scale(const f32 x, const f32 y, const f32 z) {
        Matrix mat(
            x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0,
            0, 0, 0, 1);
        (*this) *= mat;
    }

}// namespace ob