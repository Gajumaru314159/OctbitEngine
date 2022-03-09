//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Platform/Type/Language.h>
#include <WinNls.h>
namespace ob::platform {
    namespace detail {
        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        Language GetLanguage() {
            LANGID langID = GetSystemDefaultLangID();
            switch (langID) {
            case 0x0436:
                return Language::Afrikaans;
            case 0x3801:
            case 0x3C01:
            case 0x1401:
            case 0x0C01:
            case 0x0801:
            case 0x2C01:
            case 0x3401:
            case 0x3001:
            case 0x1001:
            case 0x1801:
            case 0x2001:
            case 0x4001:
            case 0x0401:
            case 0x2801:
            case 0x1C01:
            case 0x2401:
                return Language::Arabic;
            case 0x042D:
                return Language::Basque;
            case 0x0423:
                return Language::Belarusian;
            case 0x0402:
                return Language::Bulgarian;
            case 0x0403:
                return Language::Catalan;
            case 0x0804:
            case 0x0C04:
            case 0x1004:
            case 0x0404:
                return Language::Chinese;
            case 0x0405:
                return Language::Czech;
            }
            return Language::Unknown;
        }

    }//namespace detail
}// namespace ob::platform