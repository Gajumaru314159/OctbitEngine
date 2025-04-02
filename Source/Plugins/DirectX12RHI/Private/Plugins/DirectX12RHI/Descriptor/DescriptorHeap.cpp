//***********************************************************
//! @file
//! @brief		デスクリプタ・ヒープ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "DescriptorHeap.h"
#include <Framework/Core/Math/BitOp.h>
#include <Plugins/DirectX12RHI/DirectX12RHI.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorHandle.h>

namespace ob::rhi::dx12 {

	//! @brief          コンストラクタ
	//! 
	//! @param device   デバイス
	//! @param type     アロケート・タイプ
	//! @param capacity 容量
	DescriptorHeap::DescriptorHeap(DirectX12RHI& device, DescriptorHeapType type, s32 capacity)
		: m_mapper(capacity)
		, m_type(type)
	{
		{
			D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
			descHeapDesc.Type = TypeConverter::Convert(type);
			descHeapDesc.NumDescriptors = (UINT)m_mapper.capacity();
			descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			descHeapDesc.NodeMask = 0;
			if (descHeapDesc.Type == D3D12_DESCRIPTOR_HEAP_TYPE_RTV ||
				descHeapDesc.Type == D3D12_DESCRIPTOR_HEAP_TYPE_DSV) {
				descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			}

			m_descriptorSize = device.getNative()->GetDescriptorHandleIncrementSize(descHeapDesc.Type);

			HRESULT result;
			result = device.getNative()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(m_heap.ReleaseAndGetAddressOf()));
			if (FAILED(result)) {
				Utility::OutputFatalLog(result, "ID3D12Device::CreateDescriptorHeap()");
				return;
			}
		}

	}


	//! @brief          デストラクタ
	DescriptorHeap::~DescriptorHeap() {
	}


	//! @brief  名前を設定
	void DescriptorHeap::setName(StringView name) {
		Utility::SetName(m_heap.Get(), name);
	}


	//! @brief          ハンドルをアロケート
	//! 
	//! @param handle   アロケート先ハンドル
	//! @param viewNum  割り当て個数
	void DescriptorHeap::allocateHandle(class DescriptorHandle& handle, s32 size) {

		handle.release();
		if (size <= 0)return;

		ScopeLock lock(m_mutex);

		auto pBlock = m_mapper.allocate(size);
		if (pBlock == nullptr) {
			LOG_FATAL_EX("Graphic", "DescriptorHandleのアロケートに失敗しました。[size={}]", size);
			return;
		}
		const_cast<TLSFBlock*>(pBlock)->pUser = this;
		handle.m_pBlock = pBlock;

	}


	//! @brief          ハンドルを解放
	void DescriptorHeap::releaseHandle(class DescriptorHandle& handle) {

		ScopeLock lock(m_mutex);
		m_mapper.free(handle.m_pBlock);
		handle.m_pBlock = nullptr;

	}


	//! @brief          CPUハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::getCpuHandle(u32 index) {
		OB_ASSERT_RANGE(index, 0, m_mapper.capacity() - 1);
		OB_ASSERT(m_heap,"ヒープが空です。");
		D3D12_CPU_DESCRIPTOR_HANDLE handle = m_heap->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += static_cast<size_t>(index) * m_descriptorSize;
		return handle;
	}


	//! @brief          CPUハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::getGpuHandle(u32 index) {
		OB_ASSERT_RANGE(index, 0, m_mapper.capacity() - 1);
		OB_ASSERT(m_heap, "ヒープが空です。");
		D3D12_GPU_DESCRIPTOR_HANDLE handle = m_heap->GetGPUDescriptorHandleForHeapStart();
		handle.ptr += static_cast<size_t>(index) * m_descriptorSize;
		return handle;
	}


	//! @brief  タイプを取得
	DescriptorHeapType DescriptorHeap::getHeapType()const {
		return m_type;
	}

}