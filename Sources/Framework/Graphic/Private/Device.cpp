//***********************************************************
//! @file
//! @brief		デバイス
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/Private/Device.h>
#include <Framework/Graphic/System.h>
#include <Framework/Graphic/SwapChain.h>
#include <Framework/Graphic/Texture.h>
#include <Framework/Graphic/RenderTexture.h>
#include <Framework/Graphic/Shader.h>

namespace ob::graphic
{

    //@―---------------------------------------------------------------------------
    //! @brief  デバイスを取得
    //@―---------------------------------------------------------------------------
    IDevice* Device::Get() {
        auto pDevice = System::Instance().getDevice();
        OB_CHECK_ASSERT(pDevice, "ob::graphic::Systemが初期化されていないためデバイスの取得に失敗しました。");
        return pDevice;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  SwapChain の実装を取得
    //@―---------------------------------------------------------------------------
    ISwapChain* Device::GetImpl(SwapChain& obj) {
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  Texture の実装を取得
    //@―---------------------------------------------------------------------------
    ITexture* Device::GetImpl(Texture& obj) {
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  RenderTexture の実装を取得
    //@―---------------------------------------------------------------------------
    IRenderTexture* Device::GetImpl(RenderTexture& obj) {
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  Shader の実装を取得
    //@―---------------------------------------------------------------------------
    IShader* Device::GetImpl(Shader& obj) {
        return obj.m_pImpl;
    }

}// namespace ob