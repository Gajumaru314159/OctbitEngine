﻿//***********************************************************
//! @file
//! @brief		テクスチャ
//! @author		Gajumaru
//***********************************************************
#include "Texture.h"
#include <Framework/Graphic/Interface/ITexture.h>
#include <Framework/Graphic/System.h>

namespace ob::graphic
{


    //===============================================================
    // コンストラクタ / デストラクタ
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    Texture::Texture()
        : m_pImpl(nullptr){

    }


    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    Texture::Texture(ITexture& texture) {
        texture.addReference();
        m_pImpl = &texture;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャ・サイズを取得
    //@―---------------------------------------------------------------------------
    Size Texture::size()const {
        OB_REQUIRE(m_pImpl != nullptr);
        return m_pImpl->getDesc().size;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャ・フォーマットを取得
    //@―---------------------------------------------------------------------------
    auto Texture::format()const->TextureFormat {
        OB_REQUIRE(m_pImpl != nullptr);
        return m_pImpl->getDesc().format;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャ使用法を取得
    //@―---------------------------------------------------------------------------
    auto Texture::usage()const->ResourceUsage {
        OB_REQUIRE(m_pImpl != nullptr);
        return m_pImpl->getDesc().usage;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      ミップマップを持っているか
    //@―---------------------------------------------------------------------------
    s32 Texture::mipLevels()const {
        OB_REQUIRE(m_impl != nullptr);
        return m_pImpl->getDesc().mipLevel;
    }

}// namespace ob::graphic