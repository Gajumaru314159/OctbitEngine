//***********************************************************
//! @file
//! @brief		テクスチャ
//! @author		Gajumaru
//***********************************************************
#include "RenderTexture.h"
#include <Framework/Graphic/Interface/IRenderTexture.h>
#include <Framework/Graphic/System.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    RenderTexture::RenderTexture()
        : m_pImpl(nullptr) {
    }


    //@―---------------------------------------------------------------------------
    //! @brief  Texture を取得
    //@―---------------------------------------------------------------------------
    Texture RenderTexture::getTexture(s32 index) {
        if (m_pImpl == nullptr) {
            return Texture();
        }
        
        auto pTexture=m_pImpl->getTexture(index);
        if (pTexture == nullptr) {
            return Texture();
        }

        return Texture(*pTexture);
    }

}// namespace ob::graphic