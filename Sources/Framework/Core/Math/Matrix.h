﻿//***********************************************************
//! @file
//! @brief 行列
//! @author Gajumaru
//***********************************************************
#pragma once

namespace ob {

    struct Vec3;
    struct Vec4;
    struct Quat;
    struct Rot;
    struct Affine;
    struct Frustum;




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

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief		デフォルトコンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        Matrix() = default;


        //@―---------------------------------------------------------------------------
        //! @brief		コンストラクタ(単位行列初期化)
        //@―---------------------------------------------------------------------------
        explicit Matrix(EForceInit);


        //@―---------------------------------------------------------------------------
        //! @brief 要素を指定して初期化
        //@―---------------------------------------------------------------------------
        Matrix(f32 m00, f32 m01, f32 m02, f32 m03,
            f32 m10, f32 m11, f32 m12, f32 m13,
            f32 m20, f32 m21, f32 m22, f32 m23,
            f32 m30, f32 m31, f32 m32, f32 m33);


        //===============================================================
        //  オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief Affine から初期化
        //@―---------------------------------------------------------------------------
        Matrix& operator = (const Affine& other);


        //@―---------------------------------------------------------------------------
        //! @brief 等価演算子
        //@―---------------------------------------------------------------------------
        bool    operator == (const Matrix& v) const;


        //@―---------------------------------------------------------------------------
        //! @brief 否等価演算子 
        //@―---------------------------------------------------------------------------
        bool    operator != (const Matrix& v) const;


        //@―---------------------------------------------------------------------------
        //! @brief 行列演算子
        //@―---------------------------------------------------------------------------
        Matrix  operator * (const Matrix& other) const;


        //@―---------------------------------------------------------------------------
        //! @brief 行列演算代入演算子 
        //@―---------------------------------------------------------------------------
        Matrix  operator *= (const Matrix& o); 


        //@―---------------------------------------------------------------------------
        //! @brief Quat 合成演算子
        //@―---------------------------------------------------------------------------
        Matrix  operator * (const Quat& other) const;


        //@―---------------------------------------------------------------------------
        //! @brief Quat 合成代入演算子
        //@―---------------------------------------------------------------------------
        Matrix  operator *= (const Quat& other);


        //@―---------------------------------------------------------------------------
        //! @brief Vec3 合成代入演算子
        //@―---------------------------------------------------------------------------
        Vec3    operator* (const Vec3& v)const;


        //===============================================================
        //  ゲッター / セッター
        //===============================================================
        
        //@―---------------------------------------------------------------------------
        //! @brief          行列の行の取得
        //! 
        //! @details        存在しない行を取得しようとした場合はエラー
        //! @param index    行インデックス
        //@―---------------------------------------------------------------------------
        Vec4 getColumn(s32 index)const;


        //@―---------------------------------------------------------------------------
        //! @brief          行列の列の取得
        //! 
        //! @details        存在しない列を取得しようとした場合はエラー
        //! @param index    列インデックス
        //@―---------------------------------------------------------------------------
        Vec4 getRow(s32 index)const;


        //@―---------------------------------------------------------------------------
        //! @brief          行の設定
        //! 
        //! @param index    行インデックス
        //! @param column   行ベクトル
        //@―---------------------------------------------------------------------------
        void setColumn(s32 index, const Vec4& column);


        //@―---------------------------------------------------------------------------
        //! @brief          列の設定
        //! @param index    列インデックス
        //! @param row      列ベクトル
        //@―---------------------------------------------------------------------------
        void setRow(s32 index, const Vec4& row);


        //@―---------------------------------------------------------------------------
        //! @brief 行列値の設定
        //@―---------------------------------------------------------------------------
        void set(f32 m00, f32 m01, f32 m02, f32 m03,
            f32 m10, f32 m11, f32 m12, f32 m13,
            f32 m20, f32 m21, f32 m22, f32 m23,
            f32 m30, f32 m31, f32 m32, f32 m33);


        //@―---------------------------------------------------------------------------
        //! @brief 行列に単位行列を設定
        //@―---------------------------------------------------------------------------
        void setIdentity();


        //@―---------------------------------------------------------------------------
        //! @brief 行列式の計算
        //@―---------------------------------------------------------------------------
        f32 getDeterminant()const;


        //@―---------------------------------------------------------------------------
        //! @brief              逆行列の取得
        //! 
        //! @param[out] dest    出力先
        //! @retval true        成功
        //! @retval false       失敗
        //@―---------------------------------------------------------------------------
        bool getInverse(Matrix& dest)const;


        //@―---------------------------------------------------------------------------
        //! @brief      転置行列の計算
        //! 
        //! @note       直交行列の転置行列は逆行列となる。
        //@―---------------------------------------------------------------------------
        Matrix	getTranspose()const;


        //@―---------------------------------------------------------------------------
        //! @brief 行列から回転量を計算
        //@―---------------------------------------------------------------------------
        Vec3 getTrans()const;


        //@―---------------------------------------------------------------------------
        //! @brief 行列から回転量を計算
        //@―---------------------------------------------------------------------------
        Rot getRot()const;


        //@―---------------------------------------------------------------------------
        //! @brief 行列から拡大量を計算
        //@―---------------------------------------------------------------------------
        Vec3 getScale()const;


        //@―---------------------------------------------------------------------------
        //! @brief 行列から回転量を計算
        //@―---------------------------------------------------------------------------
        Quat getQuat()const;


        //===============================================================
        //  変換
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief 行列に平行移動行列を乗算
        //! 
        //! @param x X軸方向の移動量
        //! @param y Y軸方向の移動量
        //! @param z Z軸方向の移動量
        //@―---------------------------------------------------------------------------
        void translate(f32 x, f32 y, f32 z);


        //@―---------------------------------------------------------------------------
        //! @brief 行列に平行移動行列を乗算
        //! 
        //! @param position 移動量
        //@―---------------------------------------------------------------------------
        void translate(const Vec3& pos);


        //@―---------------------------------------------------------------------------
        //! @brief 行列に回転行列を乗算
        //! 
        //! @param x X軸の回転量(Degrees)
        //! @param y Y軸の回転量(Degrees)
        //! @param z Z軸の回転量(Degrees)
        //@―---------------------------------------------------------------------------
        void rotate(f32 x, f32 y, f32 z);


        //@―---------------------------------------------------------------------------
        //! @brief 行列に回転行列を乗算
        //! 
        //! @param eulerAngles 回転量
        //@―---------------------------------------------------------------------------
        void rotate(const Vec3& eulerAngles);


        //@―---------------------------------------------------------------------------
        //! @brief 行列に回転行列を乗算
        //! 
        //! @param rotation 回転量
        //@―---------------------------------------------------------------------------
        void rotate(const Rot& rotation);


        //@―---------------------------------------------------------------------------
        //! @brief 行列に回転行列を乗算
        //! 
        //! @param quat 回転量
        //@―---------------------------------------------------------------------------
        void rotate(const Quat& quat);


        //@―---------------------------------------------------------------------------
        //! @brief 行列に拡大縮小行列を乗算
        //! 
        //! @param x X軸方向の拡大縮小量
        //! @param y Y軸方向の拡大縮小量
        //! @param z Z軸方向の拡大縮小量
        //@―---------------------------------------------------------------------------
        void scale(f32 x, f32 y, f32 z);


        //@―---------------------------------------------------------------------------
        //! @brief 行列に拡大縮小行列を乗算
        //! 
        //! @param scale 拡大縮小量
        //@―---------------------------------------------------------------------------
        void scale(const Vec3& scale);


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

        //@―---------------------------------------------------------------------------
        //! @brief 視錐台からビュー行列の生成
        //@―---------------------------------------------------------------------------
        static Matrix CreateFrustum(Frustum desc);


        //@―---------------------------------------------------------------------------
        //! @brief 透視投影行列の生成
        //@―---------------------------------------------------------------------------
        static Matrix CreatePerspective(f32 fov, f32 aspect, f32 zNear, f32 zFar);


        //@―---------------------------------------------------------------------------
        //! @brief  平行投影行列の生成
        //@―---------------------------------------------------------------------------
        static Matrix CreateOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar);

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief		コンストラクタ(単位行列初期化)
    //@―---------------------------------------------------------------------------
    inline Matrix::Matrix(EForceInit) {
        setIdentity();
    }


    //@―---------------------------------------------------------------------------
    //! @brief 要素を指定して初期化
    //@―---------------------------------------------------------------------------
    inline Matrix::Matrix(const f32 m00, const f32 m01, const f32 m02, const f32 m03,
        const f32 m10, const f32 m11, const f32 m12, const f32 m13,
        const f32 m20, const f32 m21, const f32 m22, const f32 m23,
        const f32 m30, const f32 m31, const f32 m32, const f32 m33) {
        set(
            m00, m01, m02, m03,
            m10, m11, m12, m13,
            m20, m21, m22, m23,
            m30, m31, m32, m33);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 行列に単位行列を設定
    //@―---------------------------------------------------------------------------
    inline void Matrix::setIdentity() {
        memset(m, 0, sizeof(m));
        m[0][0] = 1;
        m[1][1] = 1;
        m[2][2] = 1;
        m[3][3] = 1;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 行列に平行移動行列を乗算
    //! 
    //! @param x X軸方向の移動量
    //! @param y Y軸方向の移動量
    //! @param z Z軸方向の移動量
    //@―---------------------------------------------------------------------------
    inline void Matrix::translate(const f32 x, const f32 y, const f32 z) {
        Matrix mat(
            1, 0, 0, x,
            0, 1, 0, y,
            0, 0, 1, z,
            0, 0, 0, 1);
        (*this) *= mat;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 行列に拡大縮小行列を乗算
    //! 
    //! @param x X軸方向の拡大縮小量
    //! @param y Y軸方向の拡大縮小量
    //! @param z Z軸方向の拡大縮小量
    //@―---------------------------------------------------------------------------
    inline void Matrix::scale(const f32 x, const f32 y, const f32 z) {
        Matrix mat(
            x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0,
            0, 0, 0, 1);
        (*this) *= mat;
    }

    //! @endcond
}// namespace ob