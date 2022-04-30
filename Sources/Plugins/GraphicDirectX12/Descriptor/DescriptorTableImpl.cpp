//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "DescriptorTableImpl.h"
#include <Plugins/GraphicDirectX12/Descriptor/DescriptorHeap.h>
#include <Plugins/GraphicDirectX12/Device/DeviceImpl.h>

namespace ob::graphic::dx12
{

	//@―---------------------------------------------------------------------------
	//! @brief              コンストラクタ
	//!
	//! @param type         デスクリプタに設定するリソースの種類
	//! @param elementNum   要素数
	//@―---------------------------------------------------------------------------
	DescriptorTableImpl::DescriptorTableImpl(DescriptorHeap& heap,DescriptorHeapType type, s32 elementNum) {
		heap.allocateHandle(m_handle,elementNum);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  妥当な状態か
	//@―---------------------------------------------------------------------------
	bool DescriptorTableImpl::isValid()const {
		return !m_handle.empty();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  リソースを設定
	//@―---------------------------------------------------------------------------
	//! @{
	//bool setResource(s32 index, class Buffer& resource) override{}
	bool DescriptorTableImpl::setResource(s32 index, class Texture& resource) {
		return true;
	}
	//bool setResource(s32 index, class Sampler& resource) override{}
	//! @}


	//@―---------------------------------------------------------------------------
	//! @brief  リソースのバインドを解除
	//@―---------------------------------------------------------------------------
	void DescriptorTableImpl::clear() {
	}


	//@―---------------------------------------------------------------------------
	//! @brief  指定したインデックスのリソースのバインドを解除
	//@―---------------------------------------------------------------------------
	void DescriptorTableImpl::clearAt(s32 index) {
	}

}// namespace ob::graphic::dx12