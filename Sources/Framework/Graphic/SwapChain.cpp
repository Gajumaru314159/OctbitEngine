﻿//***********************************************************
//! @file
//! @brief		スワップ・チェーン
//! @author		Gajumaru
//***********************************************************
#include "Swapchain.h"
#include <Framework/Graphic/Private/Device.h>
#include <Framework/Graphic/RenderTexture.h>
#include <Framework/Graphic/Interface/ISwapchain.h>
#include <Framework/Graphic/Interface/IRenderTexture.h>

namespace ob::graphic {

    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(SwapChain);

#define CHECK_IMPL() OB_CHECK_ASSERT(m_pImpl,"未初期化のSwapChainへアクセス")

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    SwapChain::SwapChain(const SwapchainDesc& desc, StringView name) {
        m_pImpl = Device::Get()->createSwapChain(desc, name);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_ERROR_EX("Graphic", "スワップチェインの生成に失敗");
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  バックバッファの数を取得
    //@―---------------------------------------------------------------------------
    s32 SwapChain::getBackBufferCount()const {
        CHECK_IMPL();
        return m_pImpl->getBackBufferCount();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  VSyncが有効か
    //@―---------------------------------------------------------------------------
    s32 SwapChain::isVSyncEnabled()const {
        CHECK_IMPL();
        return m_pImpl->isVSyncEnabled();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  HDRが有効か
    //@―---------------------------------------------------------------------------
    s32 SwapChain::isHdrEnabled()const {
        CHECK_IMPL();
        return m_pImpl->isHdrEnabled();
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
    void SwapChain::update(Texture& texture) {
        CHECK_IMPL();
        m_pImpl->update(texture.m_pImpl);
    }

}// namespace ob::graphic