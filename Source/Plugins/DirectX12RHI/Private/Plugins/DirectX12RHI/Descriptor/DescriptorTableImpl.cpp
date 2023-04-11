//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "DescriptorTableImpl.h"
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/Buffer.h>
#include <Plugins/DirectX12RHI/DirectX12RHI.h>
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
		
		m_elemetns.resize(elementNum);
		for (auto& element : m_elemetns)element = std::make_unique<Element>();
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
		if (!is_in_range(index, m_elemetns)) {
			LOG_ERROR("範囲外エラー。DescriptorTableのインデックスが不正です。[index={}]", index);
			return false;
		}

		m_elemetns[index]->clear();
		m_elemetns[index]->buffer = resource;

		if (auto p = m_elemetns[index]->buffer.cast<BufferImpl>()) {
			auto handle = m_handle.getCpuHandle(index);
			p->createCBV(handle);
		}
		return true;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  テクスチャリソースを設定
	//@―---------------------------------------------------------------------------
	bool DescriptorTableImpl::setResource(s32 index, const Ref<Texture>& resource) {
		if (!is_in_range(index, m_elemetns)) {
			LOG_ERROR("範囲外エラー。DescriptorTableのインデックスが不正です。[index={}]",index);
			return false;
		}

		m_elemetns[index]->clear();
		m_elemetns[index]->texture = resource;
		
		if (auto p = resource.cast<TextureImpl>()) {
			auto handle = m_handle.getCpuHandle(index);
			p->createSRV(handle);
			p->addEventListener(m_elemetns[index]->hTextureUpdate,
				[this,index]() {
					if (auto p = m_elemetns[index]->texture.cast<TextureImpl>()) {
						auto handle = m_handle.getCpuHandle(index);
						p->createSRV(handle);
					}
				}
			);
		}
		return true;
	}
	//bool setResource(s32 index, class Sampler& resource) override{}
	//! @}

}// namespace ob::rhi::dx12