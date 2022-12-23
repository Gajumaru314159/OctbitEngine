//***********************************************************
//! @file
//! @brief		レンダーターゲット
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RenderTarget.h>
#include <Framework/RHI/IRenderTarget.h>
#include <Framework/RHI/Device.h>

namespace ob::rhi {

    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(RenderTarget);

#define CHECK_IMPL() OB_CHECK_ASSERT(m_pImpl,"未初期化のRenderTargetへアクセス")

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //! 
    //! @param targets  各レンダ―ターゲットの定義
    //! @param depth    デプス・ステンシル・テクスチャの定義
    //! @param name     オブジェクト名
    //@―---------------------------------------------------------------------------
    RenderTarget::RenderTarget(const RenderTargetDesc& desc) {
        m_pImpl = nullptr;// Device::Get()->createRenderTarget(desc);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "RenderTargetの生成に失敗");
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief      定義を取得
    //@―---------------------------------------------------------------------------
    const RenderTargetDesc& RenderTarget::getDesc()const {
        CHECK_IMPL();
        return m_pImpl->getDesc();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  Texture を取得
    //@―---------------------------------------------------------------------------
    Texture RenderTarget::getColorTexture(s32 index)const {
        CHECK_IMPL();
        return m_pImpl->getColorTexture(index);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デプステクスチャを取得
    //@―---------------------------------------------------------------------------
    Texture RenderTarget::getDepthTexture()const {
        CHECK_IMPL();
        return m_pImpl->getDepthTexture();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      描画ターゲット枚数を取得
    //! 
    //! @details    デプス・ステンシルテクスチャは含まれません。
    //@―---------------------------------------------------------------------------
    s32 RenderTarget::getColorTextureCount()const {
        CHECK_IMPL();
        return m_pImpl->getColorTextureCount();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デプス・テクスチャを持っているか
    //@―---------------------------------------------------------------------------
    bool RenderTarget::hasDepth()const {
        CHECK_IMPL();
        return !m_pImpl->getDepthTexture().empty();
    }

}// namespace ob::rhi