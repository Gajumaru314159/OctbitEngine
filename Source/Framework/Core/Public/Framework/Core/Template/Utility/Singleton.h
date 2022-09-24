//***********************************************************
//! @file
//! @brief		シングルトン
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief          シングルトン・パターン
    //! 
    //! @details        解放順は制御できません。解放順に依存関係がある場合は使用できません。
    //@―---------------------------------------------------------------------------
    template<class T>
    class Singleton {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      インスタンスを取得
        //@―---------------------------------------------------------------------------
        static T& Get()noexcept {
            static T instance;
            return instance;
        }

    protected:

        Singleton() = default;
        virtual ~Singleton() = default;

    private:

        //! @cond  
        Singleton(const Singleton&) = delete;
        Singleton(Singleton&&) = delete;
        Singleton& operator = (const Singleton&) = delete;
        Singleton& operator = (Singleton&&) = delete;
        //! @endcond  

    };

}// namespcae ob