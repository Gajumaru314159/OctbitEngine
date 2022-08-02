﻿//***********************************************************
//! @file
//! @brief		ボックス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Math/Vector3.h>

namespace ob::core {

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
        //! @brief          デフォルトコンストラクタ(ゼロ初期化)
        //@―---------------------------------------------------------------------------
        constexpr Box()noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          コンストラクタ(初期化なし)
        //@―---------------------------------------------------------------------------
        Box(EForceInit)noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          コンストラクタ(サイズ/中心指定)
        //@―---------------------------------------------------------------------------
        constexpr Box(const Vec3& size, const Vec3& center = Vec3::Zero)noexcept;


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
        constexpr bool operator==(const Box& other)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          否等価演算子
        //@―---------------------------------------------------------------------------
        constexpr bool operator!=(const Box& other)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          加算演算子
        //! 
        //! @details        ポイントを含むようにボックスを拡大する。
        //!                 元のボックスが空の場合は何もしない。
        //! @param point    ポイント
        //@―---------------------------------------------------------------------------
        constexpr Box  operator+(const Vec3& point)const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          加算演算子
        //! 
        //! @details        ポイントを含むようにボックスを拡大する。
        //!                 元のボックスが空の場合は何もしない。
        //! @param point    ポイント
        //@―---------------------------------------------------------------------------
        constexpr Box& operator+=(const Vec3& point)noexcept;


        //===============================================================
        //  セッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief          中心とサイズから Box を設定
        //! 
        //! @details        点群を含む最小の Box に設定する
        //@―---------------------------------------------------------------------------
        constexpr void set(const Vec3& center, const Vec3& size = Vec3::Zero)noexcept;


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
        constexpr void reset()noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief          size を絶対値に更新
        //@―---------------------------------------------------------------------------
        constexpr void normalize()noexcept;

        //@―---------------------------------------------------------------------------
        //! @brief          size を絶対値に更新
        //@―---------------------------------------------------------------------------
        constexpr void setStartEnd(const Vec3& start, const Vec3& end)noexcept;


        //===============================================================
        //  ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      ボックスの起点となる頂点を取得
        //! 
        //! @see        start()
        //@―---------------------------------------------------------------------------
        constexpr Vec3 start()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      start() と対角となる頂点を取得
        //! 
        //! @see        start()
        //@―---------------------------------------------------------------------------
        constexpr Vec3 end()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      体積を取得
        //@―---------------------------------------------------------------------------
        constexpr f32 volume()const noexcept;


        //===============================================================
        //  判定
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      サイズが0
        //@―---------------------------------------------------------------------------
        constexpr bool empty()const noexcept;


    public:

        //@―---------------------------------------------------------------------------
        //! @brief      補間
        //! 
        //! @param a    開始
        //! @param b    終了
        //! @param t    補間係数
        //! @return     t=0のときa、t=1の時bを返す。
        //@―---------------------------------------------------------------------------
        static constexpr Box Lerp(const Box& a, const Box& b, f32 t)noexcept;


    public:

        Vec3    center;     //!< ボックスの中心座標
        Vec3    size;       //!< ボックスのサイズ

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief          デフォルトコンストラクタ(ゼロ初期化)
    //@―---------------------------------------------------------------------------
    constexpr Box::Box()noexcept 
    {
    }


    //@―---------------------------------------------------------------------------
    //! @brief          コンストラクタ(初期化なし)
    //@―---------------------------------------------------------------------------
    inline Box::Box(EForceInit)noexcept {
    }


    //@―---------------------------------------------------------------------------
    //! @brief          コンストラクタ(サイズ/中心指定)
    //@―---------------------------------------------------------------------------
    constexpr Box::Box(const Vec3& size, const Vec3& center)noexcept {
        set(size, center);
    }


    //@―---------------------------------------------------------------------------
    //! @brief          等価演算子
    //@―---------------------------------------------------------------------------
    constexpr bool Box::operator==(const Box& other)const noexcept {
        return (center == other.center) && (size == other.size);
    }


    //@―---------------------------------------------------------------------------
    //! @brief          否等価演算子
    //@―---------------------------------------------------------------------------
    constexpr bool Box::operator!=(const Box& other)const noexcept {
        return !(*this == other);
    }


    //@―---------------------------------------------------------------------------
    //! @brief          加算演算子
    //! 
    //! @details        ポイントを含むようにボックスを拡大する。
    //!                 元のボックスが空の場合は何もしない。
    //! @param point    ポイント
    //@―---------------------------------------------------------------------------
    constexpr Box Box::operator+(const Vec3& point)const noexcept {
        return Box(*this) += point;
    }


    //@―---------------------------------------------------------------------------
    //! @brief          加算演算子
    //! 
    //! @details        ポイントを含むようにボックスを拡大する。
    //! @param point    ポイント
    //@―---------------------------------------------------------------------------
    constexpr Box& Box::operator+=(const Vec3& point)noexcept {
        if (empty()) {
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
    constexpr void Box::set(const Vec3& size, const Vec3& center)noexcept {
        this->size = size;
        this->center = center;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      構造体をゼロ初期化する
    //@―---------------------------------------------------------------------------
    constexpr void Box::reset()noexcept {
        center.setZero();
        size.setZero();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      size を絶対値に更新
    //@―---------------------------------------------------------------------------
    constexpr void Box::normalize()noexcept {
        size = Vec3::Abs(size);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      2点を指定して Box を設定
    //@―---------------------------------------------------------------------------
    constexpr void Box::setStartEnd(const Vec3& start, const Vec3& end) noexcept {
        center = (start + end) * 0.5f;
        size = Vec3::Abs(start - end);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      ボックスの起点となる頂点を取得
    //! 
    //! @see        GetEnd()
    //@―---------------------------------------------------------------------------
    constexpr Vec3 Box::start()const noexcept {
        return center + size * 0.5f;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      GetStart() と対角となる頂点を取得
    //! 
    //! @see        GetStart()
    //@―---------------------------------------------------------------------------
    constexpr Vec3 Box::end()const noexcept {
        return center - size * 0.5f;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      体積を取得
    //@―---------------------------------------------------------------------------
    constexpr f32 Box::volume()const noexcept {
        return size.x * size.y * size.z;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      サイズが0
    //@―---------------------------------------------------------------------------
    constexpr bool Box::empty()const noexcept {
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
    constexpr Box Box::Lerp(const Box& a, const Box& b, f32 t)noexcept {
        return Box(Vec3::Lerp(a.size, b.size, t), Vec3::Lerp(a.center, b.center, t));
    }

    //! @endcond
}// namespcae ob