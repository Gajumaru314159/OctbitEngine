//***********************************************************
//! @file
//! @brief		スワップ・チェーン
//! @author		Gajumaru
//***********************************************************
#include "Swapchain.h"
#include <Framework/Graphic/System.h>
#include <Framework/Graphic/Interface/ISwapchain.h>

namespace ob::graphic {

    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(SwapChain);

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    SwapChain::SwapChain()
        :m_pImpl(nullptr) {
    }


    //@―---------------------------------------------------------------------------
    //! @brief  バックバッファの数を取得
    //@―---------------------------------------------------------------------------
    s32 SwapChain::getBackBufferCount()const {
        OB_REQUIRE(m_impl != nullptr);
        return m_pImpl->getBackBufferCount();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  描画先テクスチャを取得
    //@―---------------------------------------------------------------------------
    ITexture* SwapChain::getRederTexture(s32 index)const {
        OB_REQUIRE(m_impl != nullptr);
        return m_pImpl->getRederTexture(index);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  VSyncが有効か
    //@―---------------------------------------------------------------------------
    s32 SwapChain::isVSyncEnabled()const {
        OB_REQUIRE(m_impl != nullptr);
        return m_pImpl->isVSyncEnabled();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  HDRが有効か
    //@―---------------------------------------------------------------------------
    s32 SwapChain::isHdrEnabled()const {
        OB_REQUIRE(m_impl != nullptr);
        return m_pImpl->isHdrEnabled();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  バックバッファのサイズを変更
    //@―---------------------------------------------------------------------------
    bool SwapChain::resizeBackBuffer(const Size& size) {
        OB_REQUIRE(m_impl != nullptr);
        return m_pImpl->resizeBackBuffer(size);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      更新
    //! 
    //! @details    表示するテクスチャを次のバックバッファにします。
    //@―---------------------------------------------------------------------------
    void SwapChain::update() {
        OB_REQUIRE(m_impl!=nullptr);
        m_pImpl->update();
    }

}// namespace ob::graphic