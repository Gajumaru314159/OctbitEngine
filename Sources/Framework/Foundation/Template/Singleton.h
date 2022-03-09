﻿//***********************************************************
//! @file
//! @brief		シングルトン・パターン
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief          シングルトン・パターン
    //! 
    //! @details        インスタンスを2つ以上つくるとassertを発生させる。@n
    //!                 インスタンスの生成と破壊はコンストラクタとデストラクタで行わ
    //!                 れる。
    //@―---------------------------------------------------------------------------
    template<class T>
    class Singleton :private Noncopyable, private Nonmovable {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      インスタンスの取得
        //@―---------------------------------------------------------------------------
        static constexpr T& Get()noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      インスタンスのポインタの取得
        //@―---------------------------------------------------------------------------
        static constexpr T* Ptr()noexcept;

    protected:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //@―---------------------------------------------------------------------------
        constexpr Singleton()noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      デストラクタ
        //@―---------------------------------------------------------------------------
        ~Singleton()noexcept;


    private:

        static T* s_pInstance;  //!< インスタンスのポインタ

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    template<class T>
    T* Singleton<T>::s_pInstance = nullptr;

    //@―---------------------------------------------------------------------------
    //! @brief      コンストラクタ
    //@―---------------------------------------------------------------------------
    template<class T>
    constexpr Singleton<T>::Singleton()noexcept {
        assert(s_pInstance == nullptr);
        s_pInstance = static_cast<T*>(this);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      デストラクタ
    //@―---------------------------------------------------------------------------
    template<class T>
    Singleton<T>::~Singleton()noexcept {
        assert(s_pInstance != nullptr);
        s_pInstance = nullptr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      インスタンスの取得
    //@―---------------------------------------------------------------------------
    template<class T>
    constexpr T& Singleton<T>::Get()noexcept {
        assert(s_pInstance != nullptr);
        return *s_pInstance;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      インスタンスのポインタの取得
    //@―---------------------------------------------------------------------------
    template<class T>
    constexpr T* Singleton<T>::Ptr()noexcept {
        assert(s_pInstance != nullptr);
        return s_pInstance;
    }


    //! @endcond
}// namespcae ob