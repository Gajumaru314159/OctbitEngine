//***********************************************************
//! @file
//! @brief		レンダーパス実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "FrameBufferImpl.h"
#include <Plugins/DirectX12RHI/Device/DeviceImpl.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>

namespace ob::rhi::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    FrameBufferImpl::FrameBufferImpl(DeviceImpl& rDevice, const FrameBufferDesc& desc)
        : m_desc(desc)
    {
        auto& a = m_desc.renderPass.desc().attachments;
        auto& b = m_desc.attachments;

        bool ok = std::equal(a.begin(), a.end(), b.begin(),
            [&](const AttachmentDesc& ai,const RenderTexture& bi) {
                return ai.format == bi.format();
            }
        );

        OB_ASSERT(ok, "RenderPassのアタッチメントフォーマットに不一致");

    }

    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    FrameBufferImpl::~FrameBufferImpl() {

    }


    //@―---------------------------------------------------------------------------
    //! @brief  妥当なオブジェクトか
    //@―---------------------------------------------------------------------------
    bool FrameBufferImpl::isValid()const {
        return true;
    }

}// namespace ob::rhi::dx12