//***********************************************************
//! @file
//! @brief		レンダーパス実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "RenderPassImpl.h"
#include <Plugins/DirectX12RHI/Device/DeviceImpl.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>

namespace ob::rhi::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    RenderPassImpl::RenderPassImpl(DeviceImpl& rDevice, const RenderPassDesc& desc)
        : m_desc(desc)
    {
        
    }

    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    RenderPassImpl::~RenderPassImpl() {

    }

    //@―---------------------------------------------------------------------------
    //! @brief  定義取得
    //@―---------------------------------------------------------------------------
    const RenderPassDesc& RenderPassImpl::desc()const {
        return m_desc; 
    }

    //@―---------------------------------------------------------------------------
    //! @brief  妥当なオブジェクトか
    //@―---------------------------------------------------------------------------
    bool RenderPassImpl::isValid()const {
        return true;
    }

    //@―---------------------------------------------------------------------------
    //! @brief      名前を取得
    //@―---------------------------------------------------------------------------
    const String& RenderPassImpl::getName()const {
        return m_desc.name;
    }

}// namespace ob::rhi::dx12