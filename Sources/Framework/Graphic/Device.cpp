//***********************************************************
//! @file
//! @brief		スワップチェーン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Device.h"
#include <Framework/Graphic/Interface/IDevice.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      スワップチェーンを生成
    //! 
    //! @details    モニターに表示するバッファを複数のバッファから切り替えることで
    //!             表示のちらつきを解決する。
    //@―---------------------------------------------------------------------------
    void Device::createSwapChain(Swapchain& display, SwapchainDesc& desc) {
        display.release();
        display.m_impl = m_impl->createSwapChain(desc);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャを生成
    //@―---------------------------------------------------------------------------
    void Device::createTexture(Texture& texture, const TextureDesc& desc, StringView name) {
        texture.release();
        texture.m_impl = m_impl->createTexture(desc,name);
    }

}// namespace pb::graphic