//***********************************************************
//! @file
//! @brief		デバイス
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/Device.h>
#include <Framework/Graphic/System.h>
#include <Framework/Graphic/SwapChain.h>
#include <Framework/Graphic/CommandList.h>
#include <Framework/Graphic/RootSignature.h>
#include <Framework/Graphic/PipelineState.h>
#include <Framework/Graphic/Shader.h>
#include <Framework/Graphic/Texture.h>
#include <Framework/Graphic/RenderTarget.h>
#include <Framework/Graphic/Buffer.h>
#include <Framework/Graphic/DescriptorTable.h>

namespace ob::graphic
{

    //@―---------------------------------------------------------------------------
    //! @brief  デバイスを取得
    //@―---------------------------------------------------------------------------
    IDevice* Device::Get() {
        auto pDevice = System::Get().getDevice();
        OB_CHECK_ASSERT(pDevice, "ob::graphic::Systemが初期化されていないためデバイスの取得に失敗しました。");
        return pDevice;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  SwapChain の実装を取得
    //@―---------------------------------------------------------------------------
    const ISwapChain* Device::GetImpl(const SwapChain& obj) {
        OB_CHECK_ASSERT(obj.m_pImpl, "空のSwapChainにアクセスしました。");
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  CommandList の実装を取得
    //@―---------------------------------------------------------------------------
    const ICommandList* Device::GetImpl(const CommandList& obj) {
        OB_CHECK_ASSERT(obj.m_pImpl, "空のCommandListにアクセスしました。");
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  RootSignature の実装を取得
    //@―---------------------------------------------------------------------------
    const IRootSignature* Device::GetImpl(const RootSignature& obj) {
        OB_CHECK_ASSERT(obj.m_pImpl, "空のRootSignatureにアクセスしました。");
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  PipelineState の実装を取得
    //@―---------------------------------------------------------------------------
    const IPipelineState* Device::GetImpl(const PipelineState& obj) {
        OB_CHECK_ASSERT(obj.m_pImpl, "空のPipelineStateにアクセスしました。");
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  Shader の実装を取得
    //@―---------------------------------------------------------------------------
    const IShader* Device::GetImpl(const Shader& obj) {
        OB_CHECK_ASSERT(obj.m_pImpl, "空のShaderにアクセスしました。");
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  Texture の実装を取得
    //@―---------------------------------------------------------------------------
    const ITexture* Device::GetImpl(const Texture& obj) {
        OB_CHECK_ASSERT(obj.m_pImpl, "空のTextureにアクセスしました。");
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  RenderTarget の実装を取得
    //@―---------------------------------------------------------------------------
    const IRenderTarget* Device::GetImpl(const RenderTarget& obj) {
        OB_CHECK_ASSERT(obj.m_pImpl, "空のRenderTargetにアクセスしました。");
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  RenderTarget の実装を取得
    //@―---------------------------------------------------------------------------
    const IBuffer* Device::GetImpl(const Buffer& obj) {
        OB_CHECK_ASSERT(obj.m_pImpl, "空のBufferにアクセスしました。");
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  DescriptorTable の実装を取得
    //@―---------------------------------------------------------------------------
    const IDescriptorTable* Device::GetImpl(const DescriptorTable& obj) {
        OB_CHECK_ASSERT(obj.m_pImpl, "空のDescriptorTableにアクセスしました。");
        return obj.m_pImpl;
    }

}// namespace ob