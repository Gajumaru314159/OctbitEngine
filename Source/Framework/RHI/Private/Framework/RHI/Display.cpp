//***********************************************************
//! @file
//! @brief		ディスプレイ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Display.h>
#include <Framework/RHI/Device.h>
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/IDisplay.h>
#include <Framework/RHI/ITexture.h>

namespace ob::rhi {

    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(Display);

#define CHECK_IMPL() OB_ASSERT(m_pImpl,"未初期化のDisplayへアクセス")

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    Display::Display(const DisplayDesc& desc) {
        m_pImpl = Device::Get()->createDisplay(desc);
        OB_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "スワップチェインの生成に失敗");
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  定義を取得
    //@―---------------------------------------------------------------------------
    const DisplayDesc& Display::getDesc()const {
        CHECK_IMPL();
        return m_pImpl->getDesc();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  バックバッファの数を取得
    //@―---------------------------------------------------------------------------
    s32 Display::getBackBufferCount()const {
        CHECK_IMPL();
        return m_pImpl->getDesc().bufferCount;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  VSyncが有効か
    //@―---------------------------------------------------------------------------
    s32 Display::isVSyncEnabled()const {
        CHECK_IMPL();
        return m_pImpl->getDesc().vsync;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  HDRが有効か
    //@―---------------------------------------------------------------------------
    s32 Display::isHdrEnabled()const {
        CHECK_IMPL();
        return m_pImpl->getDesc().hdr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  バックバッファのサイズを変更
    //@―---------------------------------------------------------------------------
    bool Display::resizeBackBuffer(const Size& size) {
        CHECK_IMPL();
        return m_pImpl->resizeBackBuffer(size);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      更新
    //! 
    //! @details    表示するテクスチャを次のバックバッファにします。
    //@―---------------------------------------------------------------------------
    void Display::update() {
        CHECK_IMPL();
        m_pImpl->update();
    }

}// namespace ob::rhi