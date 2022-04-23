//***********************************************************
//! @file
//! @brief		フレームバッファ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "FrameBufferImpl.h"
#include <Plugins/GraphicDirectX12/Device/DeviceImpl.h>
#include <Plugins/GraphicDirectX12/Utility/Utility.h>


namespace ob::graphic::dx12 {

	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ
	//@―---------------------------------------------------------------------------
	FrameBufferImpl::FrameBufferImpl(const FrameBufferDesc& desc)
		: m_desc(desc)
	{
	}


	//@―---------------------------------------------------------------------------
	//! @brief  妥当な状態か
	//@―---------------------------------------------------------------------------
	bool FrameBufferImpl::isValid()const {
		return true;
	}

}// ob::graphic::dx12