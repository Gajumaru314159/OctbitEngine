//***********************************************************
//! @file
//! @brief		テクスチャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/ITexture.h>
#include <Framework/RHI/Device.h>

namespace ob::rhi {

    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(Texture);

#define CHECK_IMPL() OB_ASSERT(m_pImpl,"未初期化のTextureへアクセス")

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //! 
    //! @param desc テクスチャ定義
    //! @param name オブジェクト名
    //@―---------------------------------------------------------------------------
    Texture::Texture(const TextureDesc& desc) {
        m_pImpl = Device::Get()->createTexture(desc);
        OB_ASSERT_EXPR(m_pImpl);
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
        OB_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "テクスチャの生成に失敗");
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャ・サイズを取得
    //@―---------------------------------------------------------------------------
    Size Texture::size()const {
        CHECK_IMPL();
        return m_pImpl->size();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      幅を取得
    //@―---------------------------------------------------------------------------
    s32 Texture::width()const {
        return size().width;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      高さを取得
    //@―---------------------------------------------------------------------------
    s32 Texture::height()const {
        return size().height;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャ・フォーマットを取得
    //@―---------------------------------------------------------------------------
    auto Texture::format()const->TextureFormat {
        CHECK_IMPL();
        return m_pImpl->format();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      ミップマップを持っているか
    //@―---------------------------------------------------------------------------
    s32 Texture::mipLevels()const {
        CHECK_IMPL();
        return m_pImpl->mipLevels();
    }

}// namespace ob::rhi