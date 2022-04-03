//***********************************************************
//! @file
//! @brief		テクスチャ
//! @author		Gajumaru
//***********************************************************
#include "Texture.h"
#include <Framework/Graphic/Interface/ITexture.h>
#include <Framework/Graphic/Private/Device.h>

namespace ob::graphic {

    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(Texture);

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //! 
    //! @param desc テクスチャ定義
    //! @param name オブジェクト名
    //@―---------------------------------------------------------------------------
    Texture::Texture(const TextureDesc& desc, StringView name) {
        m_pImpl = Device::Get()->createTexture(desc, name);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_ERROR_EX("Graphic", "テクスチャの生成に失敗");
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャ定義を取得
    //@―---------------------------------------------------------------------------
    const TextureDesc& Texture::desc()const {
        OB_CHECK_ASSERT_EXPR(m_pImpl != nullptr);
        return m_pImpl->getDesc();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャ・サイズを取得
    //@―---------------------------------------------------------------------------
    Size Texture::size()const {
        OB_CHECK_ASSERT_EXPR(m_pImpl != nullptr);
        return m_pImpl->getDesc().size;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャ・フォーマットを取得
    //@―---------------------------------------------------------------------------
    auto Texture::format()const->TextureFormat {
        OB_CHECK_ASSERT_EXPR(m_pImpl != nullptr);
        return m_pImpl->getDesc().format;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      ミップマップを持っているか
    //@―---------------------------------------------------------------------------
    s32 Texture::mipLevels()const {
        OB_CHECK_ASSERT_EXPR(m_pImpl != nullptr);
        return m_pImpl->getDesc().mipLevel;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャタイプを取得
    //@―---------------------------------------------------------------------------
    TextureType Texture::type()const {
        OB_CHECK_ASSERT_EXPR(m_pImpl != nullptr);
        return m_pImpl->getDesc().type;
    }

}// namespace ob::graphic