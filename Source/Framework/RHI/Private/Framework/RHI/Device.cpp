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
    Device* Device::Get() {
        // 高速取得のためキャッシュ
        static Device* pDevice = nullptr;
        if (pDevice == nullptr) {
            if (auto pModule = GEngine->get<GraphicModule>()) {
                pDevice = pModule->getDevice();
            }
        }
        OB_ASSERT(pDevice, "ob::rhi::Systemが初期化されていないためデバイスの取得に失敗しました。");
        return pDevice;
    }

}// namespace ob