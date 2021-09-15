//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob {
    namespace stl {

        //===============================================================
        //! @brief      定数テンプレート
        //! 
        //! @details    コンパイル時定数として扱われる。
        //===============================================================
        template<typename T, T v>
        struct constant {
        public:

            using value_type = T;           //!< T
            using type = constant<T, v>;    //!< constant<T, v>

        public:

            constexpr operator value_type()const noexcept { return value };     // 値の取得
            constexpr value_type operator ()()const noexcept { return value };  // 値の取得

        public:

            static constexpr value_type value = v;  //!< 値

        };


        using true_type = constant<bool, true>;     //!< true
        using false_type = constant<bool, false>;   //!< false

    }






    //===============================================================
    // インライン関数
    //===============================================================

    //-----------------------------------------------------------------------------


}// namespcae ob