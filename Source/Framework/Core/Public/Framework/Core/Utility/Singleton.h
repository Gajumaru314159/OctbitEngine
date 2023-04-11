//***********************************************************
//! @file
//! @brief		シングルトン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Reflection/TypeId.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief          シングルトン・パターン
    //@―---------------------------------------------------------------------------
    template<class T>
    class Singleton {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      インスタンスを取得
        //@―---------------------------------------------------------------------------
        static T* Get()noexcept {
            return s_instance;
        }

        //@―---------------------------------------------------------------------------
        //! @brief      デストラクタ
        //@―---------------------------------------------------------------------------
        virtual ~Singleton() {
            s_instance = nullptr;
        }

    protected:

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //@―---------------------------------------------------------------------------
        Singleton() {
            OB_ASSERT(s_instance == nullptr, "{}は既に生成されています。", TypeId::Get<T>().name());
            s_instance = reinterpret_cast<T*>(this);
        }

    private:

        //! @cond  
        Singleton(const Singleton&) = delete;
        Singleton(Singleton&&) = delete;
        Singleton& operator = (const Singleton&) = delete;
        Singleton& operator = (Singleton&&) = delete;
        //! @endcond  

        static T* s_instance;

    };

    template<class T>
    T* Singleton<T>::s_instance = nullptr;

}// namespcae ob