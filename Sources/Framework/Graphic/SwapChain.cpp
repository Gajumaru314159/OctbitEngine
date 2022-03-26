//***********************************************************
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

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    SwapChain::SwapChain(const SwapchainDesc& desc) {
        m_pImpl = Device::Get()->createSwapChain(desc);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  バックバッファの数を取得
    //@―---------------------------------------------------------------------------
    s32 SwapChain::getBackBufferCount()const {
        OB_CHECK_ASSERT(m_pImpl != nullptr);
        return m_pImpl->getBackBufferCount();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  VSyncが有効か
    //@―---------------------------------------------------------------------------
    s32 SwapChain::isVSyncEnabled()const {
        OB_CHECK_ASSERT(m_pImpl != nullptr);
        return m_pImpl->isVSyncEnabled();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  HDRが有効か
    //@―---------------------------------------------------------------------------
    s32 SwapChain::isHdrEnabled()const {
        OB_CHECK_ASSERT(m_pImpl != nullptr);
        return m_pImpl->isHdrEnabled();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  バックバッファのサイズを変更
    //@―---------------------------------------------------------------------------
    bool SwapChain::resizeBackBuffer(const Size& size) {
        OB_CHECK_ASSERT(m_pImpl != nullptr);
        return m_pImpl->resizeBackBuffer(size);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      更新
    //! 
    //! @details    表示するテクスチャを次のバックバッファにします。
    //@―---------------------------------------------------------------------------
    void SwapChain::update(RenderTexture& renderTexture) {
        OB_CHECK_ASSERT(m_pImpl !=nullptr); 
        m_pImpl->update(renderTexture.m_pImpl);
    }

}// namespace ob::graphic