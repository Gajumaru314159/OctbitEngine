//***********************************************************
//! @file
//! @brief		デバイス
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Device.h>
#include <Framework/RHI/Display.h>
#include <Framework/RHI/CommandList.h>
#include <Framework/RHI/RootSignature.h>
#include <Framework/RHI/PipelineState.h>
#include <Framework/RHI/Shader.h>
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/RHI/RenderPass.h>
#include <Framework/RHI/FrameBuffer.h>

#include <Framework/RHI/IGraphicModule.h>
#include <Framework/Engine/Engine.h>

namespace ob::rhi
{

    //@―---------------------------------------------------------------------------
    //! @brief  デバイスを取得
    //@―---------------------------------------------------------------------------
    IDevice* Device::Get() {
        // 高速取得のためキャッシュ
        static IDevice* pDevice = nullptr;
        if (pDevice == nullptr) {
            if (auto pModule = GEngine->get<GraphicModule>()) {
                pDevice = pModule->getDevice();
            }
        }
        OB_ASSERT(pDevice, "ob::rhi::Systemが初期化されていないためデバイスの取得に失敗しました。");
        return pDevice;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  Display の実装を取得
    //@―---------------------------------------------------------------------------
    const IDisplay* Device::GetImpl(const Display& obj) {
        OB_ASSERT(obj.m_pImpl, "空のDisplayにアクセスしました。");
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  CommandList の実装を取得
    //@―---------------------------------------------------------------------------
    const ICommandList* Device::GetImpl(const CommandList& obj) {
        OB_ASSERT(obj.m_pImpl, "空のCommandListにアクセスしました。");
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  RootSignature の実装を取得
    //@―---------------------------------------------------------------------------
    const IRootSignature* Device::GetImpl(const RootSignature& obj) {
        OB_ASSERT(obj.m_pImpl, "空のRootSignatureにアクセスしました。");
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  PipelineState の実装を取得
    //@―---------------------------------------------------------------------------
    const IPipelineState* Device::GetImpl(const PipelineState& obj) {
        OB_ASSERT(obj.m_pImpl, "空のPipelineStateにアクセスしました。");
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  FrameBuffer の実装を取得
    //@―---------------------------------------------------------------------------
    const IFrameBuffer* Device::GetImpl(const FrameBuffer& obj) {
        OB_ASSERT(obj.m_pImpl, "空のFrameBufferにアクセスしました。");
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  Shader の実装を取得
    //@―---------------------------------------------------------------------------
    const IShader* Device::GetImpl(const Shader& obj) {
        OB_ASSERT(obj.m_pImpl, "空のShaderにアクセスしました。");
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  Texture の実装を取得
    //@―---------------------------------------------------------------------------
    const ITexture* Device::GetImpl(const Texture& obj) {
        OB_ASSERT(obj.m_pImpl, "空のTextureにアクセスしました。");
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  RenderTexture の実装を取得
    //@―---------------------------------------------------------------------------
    const IRenderTexture* Device::GetImpl(const RenderTexture& obj) {
        OB_ASSERT(obj.m_pImpl, "空のRenderTargetにアクセスしました。");
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  RenderTarget の実装を取得
    //@―---------------------------------------------------------------------------
    const IBuffer* Device::GetImpl(const Buffer& obj) {
        OB_ASSERT(obj.m_pImpl, "空のBufferにアクセスしました。");
        return obj.m_pImpl;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  DescriptorTable の実装を取得
    //@―---------------------------------------------------------------------------
    const IDescriptorTable* Device::GetImpl(const DescriptorTable& obj) {
        OB_ASSERT(obj.m_pImpl, "空のDescriptorTableにアクセスしました。");
        return obj.m_pImpl;
    }

}// namespace ob