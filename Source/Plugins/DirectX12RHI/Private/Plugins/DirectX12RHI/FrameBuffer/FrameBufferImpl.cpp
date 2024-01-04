//***********************************************************
//! @file
//! @brief		レンダーパス実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "FrameBufferImpl.h"
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/RenderPass.h>
#include <Plugins/DirectX12RHI/DirectX12RHI.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>

namespace ob::rhi::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    FrameBufferImpl::FrameBufferImpl(DirectX12RHI& rDevice, const FrameBufferDesc& desc)
        : m_desc(desc)
    {
        if (!m_desc.renderPass) {
            LOG_ERROR("FraeBuffer {} のRenderPassが空です",desc.name);
            return;
        }

        auto d = m_desc.renderPass.get();// ->desc();
        auto& a = d->desc().attachments;
        auto& b = m_desc.attachments;

        bool ok = std::equal(a.begin(), a.end(), b.begin(),
            [&](const AttachmentDesc& ai, const Ref<RenderTexture>& bi) {
                if (!bi)return false;
                return ai.format == bi->format();
            }
        );

        if (ok == false) {
            LOG_ERROR("FraeBuffer {} のRenderPassのアタッチメントフォーマットに不一致",desc.name);
            return;
        }

        m_isValid = true;
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
        return m_isValid;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      名前を取得
    //@―---------------------------------------------------------------------------
    const String& FrameBufferImpl::getName()const {
        return m_desc.name;
    }

}