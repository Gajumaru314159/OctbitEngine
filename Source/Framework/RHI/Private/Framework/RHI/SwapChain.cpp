//***********************************************************
//! @file
//! @brief		スワップ・チェーン
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/SwapChain.h>
#include <Framework/RHI/Device.h>
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/ISwapchain.h>
#include <Framework/RHI/ITexture.h>

namespace ob::rhi {

    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(SwapChain);

#define CHECK_IMPL() OB_CHECK_ASSERT(m_pImpl,"未初期化のSwapChainへアクセス")

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    SwapChain::SwapChain(const SwapchainDesc& desc) {
        m_pImpl = Device::Get()->createSwapChain(desc);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "スワップチェインの生成に失敗");
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  定義を取得
    //@―---------------------------------------------------------------------------
    const SwapchainDesc& SwapChain::getDesc()const {
        CHECK_IMPL();
        return m_pImpl->getDesc();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  バックバッファの数を取得
    //@―---------------------------------------------------------------------------
    s32 SwapChain::getBackBufferCount()const {
        CHECK_IMPL();
        return m_pImpl->getDesc().bufferCount;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  VSyncが有効か
    //@―---------------------------------------------------------------------------
    s32 SwapChain::isVSyncEnabled()const {
        CHECK_IMPL();
        return m_pImpl->getDesc().vsync;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  HDRが有効か
    //@―---------------------------------------------------------------------------
    s32 SwapChain::isHdrEnabled()const {
        CHECK_IMPL();
        return m_pImpl->getDesc().hdr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  バックバッファのサイズを変更
    //@―---------------------------------------------------------------------------
    bool SwapChain::resizeBackBuffer(const Size& size) {
        CHECK_IMPL();
        return m_pImpl->resizeBackBuffer(size);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      更新
    //! 
    //! @details    表示するテクスチャを次のバックバッファにします。
    //@―---------------------------------------------------------------------------
    void SwapChain::update() {
        CHECK_IMPL();
        m_pImpl->update();
    }

}// namespace ob::rhi