//***********************************************************
//! @file
//! @brief		スワップチェーン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Device.h"
#include <Framework/Graphic/Interface/IDevice.h>

#include <Framework/Graphic/Interface/ISwapChain.h>
#include <Framework/Graphic/Interface/ITexture.h>
#include <Framework/Graphic/Interface/IRenderTexture.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      スワップチェーンを生成
    //! 
    //! @details    モニターに表示するバッファを複数のバッファから切り替えることで
    //!             表示のちらつきを解決する。
    //@―---------------------------------------------------------------------------
    void Device::createSwapChain(SwapChain& display, SwapchainDesc& desc) {
        OB_CHECK_ASSERT(m_impl != nullptr);
        display.release();
        display.m_pImpl = m_impl->createSwapChain(desc);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャを生成
    //@―---------------------------------------------------------------------------
    void Device::createTexture(Texture& texture, const TextureDesc& desc, StringView name) {
        OB_CHECK_ASSERT(m_impl != nullptr);
        texture.release();
        texture.m_pImpl = m_impl->createTexture(desc, name);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      レンダーテクスチャを生成
    //@―---------------------------------------------------------------------------
    void Device::createRenderTexture(RenderTexture& texture, const gsl::span<TextureDesc> targets, const TextureDesc& depth, StringView name) {
        OB_CHECK_ASSERT(m_impl != nullptr);
        texture.release();
        texture.m_pImpl = m_impl->createRenderTexture(targets, depth, name);
    }

}// namespace pb::graphic