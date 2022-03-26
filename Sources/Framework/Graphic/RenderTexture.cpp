//***********************************************************
//! @file
//! @brief		テクスチャ
//! @author		Gajumaru
//***********************************************************
#include "RenderTexture.h"
#include <Framework/Graphic/Interface/IRenderTexture.h>
#include <Framework/Graphic/System.h>
#include <Framework/Graphic/Private/Device.h>

namespace ob::graphic {

    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(RenderTexture);


    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //! 
    //! @param targets  各レンダ―ターゲットの定義
    //! @param depth    デプス・ステンシル・テクスチャの定義
    //! @param name     オブジェクト名
    //@―---------------------------------------------------------------------------
    RenderTexture::RenderTexture(const gsl::span<TextureDesc> targets, const TextureDesc& depth, StringView name) {
        m_pImpl = Device::Get()->createRenderTexture(targets, depth, name);
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