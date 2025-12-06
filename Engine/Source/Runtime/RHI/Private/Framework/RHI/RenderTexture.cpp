//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/RHI.h>

namespace ob::rhi {

    //! @brief          生成
    Ref<RenderTexture> RenderTexture::Create(const RenderTextureDesc& desc) {
        if (auto device = Device::Get()) {
            return device->createRenderTexture(desc);;
        }
        return nullptr;
    }

    bool RenderTexture::Supports(TextureFormat format) {
        if (auto device = Device::Get()) {
            return device->supportsForRenderTexture(format);
        }
        return false;
    }

}