//***********************************************************
//! @file
//! @brief		テクスチャ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/RenderTexture.h>
#include <Framework/Graphic/Interface/IRenderTexture.h>
#include <Framework/Graphic/System.h>
#include <Framework/Graphic/Private/Device.h>

namespace ob::graphic {

    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(RenderTexture);

#define CHECK_IMPL() OB_CHECK_ASSERT(m_pImpl,"未初期化のRenderTextureへアクセス")

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //! 
    //! @param targets  各レンダ―ターゲットの定義
    //! @param depth    デプス・ステンシル・テクスチャの定義
    //! @param name     オブジェクト名
    //@―---------------------------------------------------------------------------
    RenderTexture::RenderTexture(const gsl::span<TextureDesc> targets, const TextureDesc& depth, StringView name) {
        m_pImpl = Device::Get()->createRenderTexture(targets, depth);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        m_pImpl->setName(name);
        if (!m_pImpl->isValid()) {
            LOG_ERROR_EX("Graphic", "レンダーテクスチャの生成に失敗[name={}]", name);
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  Texture を取得
    //@―---------------------------------------------------------------------------
    Texture RenderTexture::getTexture(s32 index)const {
        CHECK_IMPL();
        return m_pImpl->getTexture(index);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デプステクスチャを取得
    //@―---------------------------------------------------------------------------
    Texture RenderTexture::getDepthStencilTexture()const {
        CHECK_IMPL();
        return m_pImpl->getDepthStencilTexture();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      描画ターゲット枚数を取得
    //! 
    //! @details    デプス・ステンシルテクスチャは含まれません。
    //@―---------------------------------------------------------------------------
    s32 RenderTexture::getTargetNum()const {
        CHECK_IMPL();
        return m_pImpl->getTargetNum();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デプス・テクスチャを持っているか
    //@―---------------------------------------------------------------------------
    bool RenderTexture::hasDepthStencil()const {
        CHECK_IMPL();
        return m_pImpl->getDepthStencilTexture();
    }

}// namespace ob::graphic