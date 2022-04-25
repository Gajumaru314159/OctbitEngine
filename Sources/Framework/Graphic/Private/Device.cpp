//***********************************************************
//! @file
//! @brief		デバイス
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/Private/Device.h>
#include <Framework/Graphic/System.h>
#include <Framework/Graphic/SwapChain.h>
#include <Framework/Graphic/RootSignature.h>
#include <Framework/Graphic/PipelineState.h>
#include <Framework/Graphic/Shader.h>
#include <Framework/Graphic/Texture.h>
#include <Framework/Graphic/RenderTexture.h>

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
    const ISwapChain* Device::GetImpl(const SwapChain& obj) {
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  RootSignature の実装を取得
    //@―---------------------------------------------------------------------------
    const IRootSignature* Device::GetImpl(const RootSignature& obj) {
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  PipelineState の実装を取得
    //@―---------------------------------------------------------------------------
    const IPipelineState* Device::GetImpl(const PipelineState& obj) {
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  Shader の実装を取得
    //@―---------------------------------------------------------------------------
    const IShader* Device::GetImpl(const Shader& obj) {
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  Texture の実装を取得
    //@―---------------------------------------------------------------------------
    const ITexture* Device::GetImpl(const Texture& obj) {
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  RenderTexture の実装を取得
    //@―---------------------------------------------------------------------------
    const IRenderTexture* Device::GetImpl(const RenderTexture& obj) {
        return obj.m_pImpl;
    }

}// namespace ob