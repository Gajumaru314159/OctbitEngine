﻿//***********************************************************
//! @file
//! @brief		ボックス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Vector/include.h>

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief      ボックス
    //! 
    //! @details    ボックスは内部的に中心座標とサイズで管理されます。
    //! @note       size が負の数の時は未定義動作です。
    //@―---------------------------------------------------------------------------
    struct OB_API Box {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief          デフォルトコンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        Box()noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          コンストラクタ( ゼロ初期化 )
        //@―---------------------------------------------------------------------------
        Box(EForceInit)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          コンストラクタ(サイズ/中心指定)
        //@―---------------------------------------------------------------------------
        Box(const Vec3& size, const Vec3& center = Vec3::zero)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          コンストラクタ(点群指定)
        //! 
        //! @details        指定された点群をすべて含む最小のボックスを構築する。
        //!                 要素数が1以下の時は reset() が呼び出される。
        //@―---------------------------------------------------------------------------
        Box(const gsl::span<Vec3>& points)noexcept;


        //===============================================================
        //  オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief          等価演算子
        //@―---------------------------------------------------------------------------
        bool operator==(const Box& other)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          否等価演算子
        //@―---------------------------------------------------------------------------
        bool operator!=(const Box& other)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          加算演算子
        //! 
        //! @details        ポイントを含むようにボックスを拡大する。
        //!                 元のボックスが空の場合は何もしない。
        //! @param point    ポイント
        //@―---------------------------------------------------------------------------
        Box  operator+(const Vec3& point)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          加算演算子
        //! 
        //! @details        ポイントを含むようにボックスを拡大する。
        //!                 元のボックスが空の場合は何もしない。
        //! @param point    ポイント
        //@―---------------------------------------------------------------------------
        Box& operator+=(const Vec3& point)noexcept;


        //===============================================================
        //  セッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief          中心とサイズから Box を設定
        //! 
        //! @details        点群を含む最小の Box に設定する
        //@―---------------------------------------------------------------------------
        void set(const Vec3& center, const Vec3& size = Vec3::zero)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          点群から Box を設定
        //! 
        //! @details        点群を含む最小の Box に設定する。
        //!                 要素数が0の時は reset() が呼び出される。
        //@―---------------------------------------------------------------------------
        void set(const gsl::span<Vec3>& points)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          構造体をゼロ初期化する
        //@―---------------------------------------------------------------------------
        void reset()noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          size を絶対値に更新
        //@―---------------------------------------------------------------------------
        void normalize()noexcept;

        //@―---------------------------------------------------------------------------
        //! @brief          size を絶対値に更新
        //@―---------------------------------------------------------------------------
        void setStartEnd(const Vec3& start, const Vec3& end)noexcept;


        //===============================================================
        //  ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      ボックスの起点となる頂点を取得
        //! 
        //! @see        start()
        //@―---------------------------------------------------------------------------
        Vec3 start()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      start() と対角となる頂点を取得
        //! 
        //! @see        start()
        //@―---------------------------------------------------------------------------
        Vec3 end()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      体積を取得
        //@―---------------------------------------------------------------------------
        f32 volume()const noexcept;


        //===============================================================
        //  判定
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      サイズが0
        //@―---------------------------------------------------------------------------
        bool isEmpty()const noexcept;


    public:

        //@―---------------------------------------------------------------------------
        //! @brief      補間
        //! 
        //! @param a    開始
        //! @param b    終了
        //! @param t    補間係数
        //! @return     t=0のときa、t=1の時bを返す。
        //@―---------------------------------------------------------------------------
        static Box Lerp(const Box& a,const Box& b,f32 t)noexcept;


    public:

        Vec3    center;     //!< ボックスの中心座標
        Vec3    size;       //!< ボックスのサイズ

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief          デフォルトコンストラクタ(初期化なし)
    //@―---------------------------------------------------------------------------
    inline Box::Box()noexcept {}


    //@―---------------------------------------------------------------------------
    //! @brief          コンストラクタ( ゼロ初期化 )
    //@―---------------------------------------------------------------------------
    inline Box::Box(EForceInit)noexcept {
        reset();
    }


    //@―---------------------------------------------------------------------------
    //! @brief          コンストラクタ(サイズ/中心指定)
    //@―---------------------------------------------------------------------------
    inline Box::Box(const Vec3& size, const Vec3& center)noexcept {
        set(size, center);
    }


    //@―---------------------------------------------------------------------------
    //! @brief          等価演算子
    //@―---------------------------------------------------------------------------
    inline bool Box::operator==(const Box& other)const noexcept {
        return (center == other.center) && (size == other.size);
    }


    //@―---------------------------------------------------------------------------
    //! @brief          否等価演算子
    //@―---------------------------------------------------------------------------
    inline bool Box::operator!=(const Box& other)const noexcept {
        return !(*this == other);
    }


    //@―---------------------------------------------------------------------------
    //! @brief          加算演算子
    //! 
    //! @details        ポイントを含むようにボックスを拡大する。
    //!                 元のボックスが空の場合は何もしない。
    //! @param point    ポイント
    //@―---------------------------------------------------------------------------
    inline Box Box::operator+(const Vec3& point)const noexcept {
        return Box(*this) += point;
    }


    //@―---------------------------------------------------------------------------
    //! @brief          加算演算子
    //! 
    //! @details        ポイントを含むようにボックスを拡大する。
    //! @param point    ポイント
    //@―---------------------------------------------------------------------------
    inline Box& Box::operator+=(const Vec3& point)noexcept {
        if (isEmpty()) {
            return *this;
        }

        Vec3 min = center - size * 0.5f;
        Vec3 max = center + size * 0.5f;

        min = Vec3::Min(min, point);
        max = Vec3::Max(max, point);

        center = (min + max) * 0.5f;
        size = max - min;

        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      中心とサイズから Box を設定
    //! 
    //! @detail     点群を含む最小の Box に設定する
    //@―---------------------------------------------------------------------------
    inline void Box::set(const Vec3& size, const Vec3& center)noexcept {
        this->size = size;
        this->center = center;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      構造体をゼロ初期化する
    //@―---------------------------------------------------------------------------
    inline void Box::reset()noexcept {
        center.setZero();
        size.setZero();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      size を絶対値に更新
    //@―---------------------------------------------------------------------------
    inline void Box::normalize()noexcept {
        size = Vec3::Abs(size);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      2点を指定して Box を設定
    //@―---------------------------------------------------------------------------
    inline void Box::setStartEnd(const Vec3& start, const Vec3& end) noexcept {
        center = (start + end) * 0.5f;
        size = Vec3::Abs(start - end);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      ボックスの起点となる頂点を取得
    //! 
    //! @see        GetEnd()
    //@―---------------------------------------------------------------------------
    inline Vec3 Box::start()const noexcept {
        return center + size * 0.5f;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      GetStart() と対角となる頂点を取得
    //! 
    //! @see        GetStart()
    //@―---------------------------------------------------------------------------
    inline Vec3 Box::end()const noexcept {
        return center - size * 0.5f;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      体積を取得
    //@―---------------------------------------------------------------------------
    inline f32 Box::volume()const noexcept {
        return size.x * size.y * size.z;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      サイズが0
    //@―---------------------------------------------------------------------------
    inline bool Box::isEmpty()const noexcept {
        return size.isZero();
    }

    //@―---------------------------------------------------------------------------
    //! @brief      補間
    //! 
    //! @param a    開始
    //! @param b    終了
    //! @param t    補間係数
    //! @return     t=0のときa、t=1の時bを返す。
    //@―---------------------------------------------------------------------------
    inline Box Box::Lerp(const Box& a, const Box& b, f32 t)noexcept {
        return Box(Vec3::Lerp(a.size, b.size, t), Vec3::Lerp(a.center, b.center, t));
    }

    //! @endcond
}// namespcae ob