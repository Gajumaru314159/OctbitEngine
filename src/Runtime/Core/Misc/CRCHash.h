//***********************************************************
//! @file
//! @brief		CRCハッシュ関数
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief ハッシュ値の計算を行うクラス
    //@―---------------------------------------------------------------------------
    class OB_API CRCHash {
    public:

        static uint32_t compute(const char* str, int len = -1);			// 指定した文字列のハッシュ値を求める
        static uint32_t compute(const wchar_t* str, int len = -1);		// 指定した文字列のハッシュ値を求める
        static uint32_t compute(const char16_t* str, int len = -1);		// 指定した文字列のハッシュ値を求める
        static uint32_t compute(const char32_t* str, int len = -1);		// 指定した文字列のハッシュ値を求める

    };

    //! @cond
    namespace detail {
        struct CRCHashInternal {
            static const unsigned* GetCRCTable();
        };

    } // namespace detail
    //! @endcond

} // namespace ob
