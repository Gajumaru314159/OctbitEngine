//***********************************************************
//! @file
//! @brief		クラスコピーの禁止イディオム
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief		クラスコピーの禁止
    //! 
    //! @details	このクラスをprivate継承すると外部からのコピーコンストラクタと代入演
    //!				算子の呼び出しが禁止される。
    //@―---------------------------------------------------------------------------
    class Noncopyable {
    protected:
        //! @cond
        Noncopyable() {}
        ~Noncopyable() {}
    private:
        Noncopyable(const Noncopyable&) = delete;
        Noncopyable& operator=(const Noncopyable&) = delete;
        //! @endvond
    };

}