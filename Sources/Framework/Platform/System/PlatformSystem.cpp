//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "PlatformSystem.h"
#include "Platforms/Windows/PlatformSystemImpl.h"

namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    PlatformSystem::PlatformSystem() {
        // PIMPLでunique_ptrを使用する場合のエラー回避
        // https://qiita.com/false-git@github/items/79bf1b6acc00dc43d173#comment-9071ab88afe79f2cf834
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    PlatformSystem::~PlatformSystem() {
        if (m_implement) {
            shutdown();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  システムを起動する
    //@―---------------------------------------------------------------------------
    bool PlatformSystem::startup() {
        m_implement = std::make_unique<detail::PlatformSystemImpl>();
        return true;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  システムを終了する
    //@―---------------------------------------------------------------------------
    void PlatformSystem::shutdown() {
        if(m_implement)m_implement.reset();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  システムの言語を取得する
    //@―---------------------------------------------------------------------------
    Language PlatformSystem::getSystemLanguage()const {
        if (!m_implement)return Language::Unknown;
        return m_implement->getSystemLanguage();
    }

}// namespace ob::platform

