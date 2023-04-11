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

#include <Framework/RHI/RHI.h>
#include <Framework/Core/Utility/DI.h>

#include <Framework/RHI/SystemResourceModule.h>

namespace ob::rhi
{
    static RHI* s_instance = nullptr;

    RHI* RHI::Get() {
        return s_instance;
    }

    RHI::RHI() {
        OB_ASSERT(s_instance == nullptr, "{}は既に生成されています。", TypeId::Get<decltype(this)>().name());
        s_instance = this;
    }
    RHI::~RHI() {
        s_instance = nullptr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デバイスを取得
    //@―---------------------------------------------------------------------------
    Device* Device::Get() {

        if (auto rhi = RHI::Get()) {
            return rhi->getDevice();
        }

        return nullptr;
    }

    Device::~Device() = default;

}// namespace ob