﻿//***********************************************************
//! @file
//! @brief シングルトン
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Core/Base/Fwd.hpp>

namespace ob
{

    //-----------------------------------------------------------
    //! @brief		シングルトンパターンのテンプレート
    //!
    //! @tparam T	シングルトン化するクラス
    //-----------------------------------------------------------
    template <class T>
    class Singleton
    {
    public:

        //-----------------------------------------------------------
        //! @brief		インスタンスの取得
        //! 
        //! @details	唯一のインスタンスが生成されていない場合はアクセス時に生成される。
        //! @return		インスタンスの参照
        //-----------------------------------------------------------
        static T& Instance()
        {
            if (ms_Instance == nullptr)
            {
                ms_Instance = new T();
            }
            return *ms_Instance;
        }


        //-----------------------------------------------------------
        //! @brief インスタンスの明示的削除
        //-----------------------------------------------------------
        static void ResetSingleton()
        {
            if (ms_Instance == nullptr)return;
            delete ms_Instance;
            ms_Instance = nullptr;
        }


        //-----------------------------------------------------------
        //! @brief インスタンスが適切に生成されているかを取得
        //! 
        //! @retval TRUE    インスタンスが存在する
        //! @retval FALSE   インスタンスが存在しない
        //-----------------------------------------------------------
        static b32 Exist()
        {
            return ms_Instance != nullptr;
        }


    protected:

        //-----------------------------------------------------------
        //! @brief デフォルトコンストラクタ
        //-----------------------------------------------------------
        Singleton() {};


        //-----------------------------------------------------------
        //! @brief デストラクタ
        //-----------------------------------------------------------
        virtual ~Singleton() {};

    protected:

        static T* ms_Instance;//!< 唯一のインスタンスのポインタ

    private:

        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;
        Singleton(Singleton&&) = delete;
        Singleton& operator=(Singleton&&) = delete;

    };

    template <class T>
    T* Singleton<T>::ms_Instance = nullptr;

} // namespace ob