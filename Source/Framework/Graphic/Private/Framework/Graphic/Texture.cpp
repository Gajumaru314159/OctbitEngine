﻿//***********************************************************
//! @file
//! @brief		テクスチャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/Texture.h>
#include <Framework/Graphic/Interface/ITexture.h>
#include <Framework/Graphic/Device.h>

namespace ob::graphic {

    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(Texture);

#define CHECK_IMPL() OB_CHECK_ASSERT(m_pImpl,"未初期化のTextureへアクセス")

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //! 
    //! @param desc テクスチャ定義
    //! @param name オブジェクト名
    //@―---------------------------------------------------------------------------
    Texture::Texture(const TextureDesc& desc) {
        m_pImpl = Device::Get()->createTexture(desc);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "テクスチャの生成に失敗");
            release();
        }
    }

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //! 
    //! @param desc テクスチャ定義
    //! @param name オブジェクト名
    //@―---------------------------------------------------------------------------
    Texture::Texture(BlobView blob){
        m_pImpl = Device::Get()->createTexture(blob);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "テクスチャの生成に失敗");
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャ定義を取得
    //@―---------------------------------------------------------------------------
    const TextureDesc& Texture::desc()const {
        CHECK_IMPL();
        return m_pImpl->getDesc();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャ・サイズを取得
    //@―---------------------------------------------------------------------------
    Size Texture::size()const {
        CHECK_IMPL();
        return m_pImpl->getDesc().size;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャ・フォーマットを取得
    //@―---------------------------------------------------------------------------
    auto Texture::format()const->TextureFormat {
        CHECK_IMPL();
        return m_pImpl->getDesc().format;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      ミップマップを持っているか
    //@―---------------------------------------------------------------------------
    s32 Texture::mipLevels()const {
        CHECK_IMPL();
        return m_pImpl->getDesc().mipLevel;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャタイプを取得
    //@―---------------------------------------------------------------------------
    TextureType Texture::type()const {
        CHECK_IMPL();
        return m_pImpl->getDesc().type;
    }

}// namespace ob::graphic