//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "DescriptorTableImpl.h"
#include <Framework/RHI/Texture.h>
#include <Plugins/DirectX12RHI/Device/DeviceImpl.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorHeap.h>
#include <Plugins/DirectX12RHI/Texture/TextureImpl.h>
#include <Plugins/DirectX12RHI/Buffer/BufferImpl.h>

namespace ob::rhi::dx12
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
	//! @brief      名前を取得
	//@―---------------------------------------------------------------------------
	const String& DescriptorTableImpl::getName()const {
		return m_name;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  リソースを設定
	//@―---------------------------------------------------------------------------
	//! @{
	//bool setResource(s32 index, class Buffer& resource) override{}

	//@―---------------------------------------------------------------------------
	//! @brief  バッファリソースを設定
	//@―---------------------------------------------------------------------------
	bool DescriptorTableImpl::setResource(s32 index,const Ref<Buffer>& resource) {
		if (auto p = resource.cast<BufferImpl>()) {
			auto handle = m_handle.getCpuHandle(index);
			p->createCBV(handle);
		}
		return true;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  テクスチャリソースを設定
	//@―---------------------------------------------------------------------------
	bool DescriptorTableImpl::setResource(s32 index, const Ref<Texture>& resource) {
		if (auto p = resource.cast<TextureImpl>()) {
			auto handle = m_handle.getCpuHandle(index);
			p->createSRV(handle);
		}
		return true;
	}
	//bool setResource(s32 index, class Sampler& resource) override{}
	//! @}


}// namespace ob::rhi::dx12