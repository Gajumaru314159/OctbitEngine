﻿//***********************************************************
//! @file
//! @brief		スワップ・チェーン
//! @author		Gajumaru
//***********************************************************
#include "Swapchain.h"
#include <Framework/Graphic/System.h>
#include <Framework/Graphic/Interface/ISwapchain.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    Swapchain::Swapchain()
        :m_impl(nullptr) {
    }

    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    Swapchain::~Swapchain() {
        release();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  明示的開放
    //@―---------------------------------------------------------------------------
    void Swapchain::release() {
        if (m_impl) {
            System::ref().requestRelease(m_impl);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  空か
    //@―---------------------------------------------------------------------------
    bool Swapchain::isEmpty()const noexcept {
        return m_impl == nullptr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  バックバッファの数を取得
    //@―---------------------------------------------------------------------------
    s32 Swapchain::getBackBufferCount()const {
        OB_REQUIRE(m_impl != nullptr);
        return m_impl->getBackBufferCount();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  描画先テクスチャを取得
    //@―---------------------------------------------------------------------------
    ITexture* Swapchain::getRederTexture(s32 index)const {
        OB_REQUIRE(m_impl != nullptr);
        return m_impl->getRederTexture(index);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  VSyncが有効か
    //@―---------------------------------------------------------------------------
    s32 Swapchain::isVSyncEnabled()const {
        OB_REQUIRE(m_impl != nullptr);
        return m_impl->isVSyncEnabled();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  HDRが有効か
    //@―---------------------------------------------------------------------------
    s32 Swapchain::isHdrEnabled()const {
        OB_REQUIRE(m_impl != nullptr);
        return m_impl->isHdrEnabled();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  バックバッファのサイズを変更
    //@―---------------------------------------------------------------------------
    bool Swapchain::resizeBackBuffer(const Size& size) {
        OB_REQUIRE(m_impl != nullptr);
        return m_impl->resizeBackBuffer(size);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      更新
    //! 
    //! @details    表示するテクスチャを次のバックバッファにします。
    //@―---------------------------------------------------------------------------
    void Swapchain::update() {
        OB_REQUIRE(m_impl!=nullptr);
        m_impl->update();
    }

}// namespace ob::graphic