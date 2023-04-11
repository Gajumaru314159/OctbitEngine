//***********************************************************
//! @file
//! @brief		コマンドキュー
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Plugins/DirectX12RHI/Buffer/BufferImpl.h>
#include <Plugins/DirectX12RHI/Module/DirectX12RHI.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>

namespace ob::rhi::dx12 {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//! 
	//! @param desc バッファ定義
	//@―---------------------------------------------------------------------------
	BufferImpl::BufferImpl(DirectX12RHI& rDevice, const BufferDesc& desc)
		: m_device(rDevice)
		, m_desc(desc)
	{
		m_desc.bufferSize = align_up(std::max<size_t>(m_desc.bufferSize, 1), 256);

		HRESULT result;

		// リソースの生成
		D3D12_HEAP_PROPERTIES heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);		
		D3D12_RESOURCE_DESC resdesc = CD3DX12_RESOURCE_DESC::Buffer(m_desc.bufferSize);

		ComPtr<ID3D12Resource> buffer;
		result = rDevice.getNative()->CreateCommittedResource(
			&heapprop,
			D3D12_HEAP_FLAG_NONE,
			&resdesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(buffer.ReleaseAndGetAddressOf()));

		if (FAILED(result))
		{
			Utility::OutputFatalLog(result, TC("ID3D12Device::CreateCommittedResource()"));
			return;
		}

		m_resource = buffer;
		Utility::SetName(m_resource.Get(), getName());
	}


	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//! 
	//! @param desc バッファ定義
	//! @param data 初期化データ
	//@―---------------------------------------------------------------------------
	BufferImpl::BufferImpl(DirectX12RHI& rDevice, const BufferDesc& desc, const Blob& blob)
		: m_device(rDevice)
		, m_desc(desc)
	{

	}


	//@―---------------------------------------------------------------------------
	//! @brief  妥当な状態か
	//@―---------------------------------------------------------------------------
	bool BufferImpl::isValid()const {
		return m_resource;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      名前を取得
	//@―---------------------------------------------------------------------------
	const String& BufferImpl::getName()const {
		return m_desc.name;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  定義を取得
	//@―---------------------------------------------------------------------------
	const BufferDesc& BufferImpl::getDesc()const {
		return m_desc;
	}


	//@―---------------------------------------------------------------------------
	//! @brief      バッファを更新
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	//@―---------------------------------------------------------------------------
	void BufferImpl::update(size_t size, const void* pData, size_t offset) {
		updateDirect(size,pData,offset);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      バッファを更新
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	//@―---------------------------------------------------------------------------
	void BufferImpl::update(const CopyFunc& func) {
		// TODO 間接更新
		updateDirect(func);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      バッファを更新(直接更新)
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	//@―---------------------------------------------------------------------------
	void BufferImpl::updateDirect(size_t size, const void* pData, size_t offset) {
		if (pData == nullptr) return;

		HRESULT result;
		byte* ptr = nullptr;
		result = m_resource->Map(0, nullptr, (void**)&ptr);
		if (FAILED(result))
		{
			Utility::OutputFatalLog(result, TC("ID3D12Resource::Map()"));
			return;
		}
		memcpy_s(ptr+offset, (s64)m_desc.bufferSize-offset, pData, size);
		m_resource->Unmap(0, nullptr);

	}


	//@―---------------------------------------------------------------------------
	//! @brief      バッファを更新(直接更新)
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	//@―---------------------------------------------------------------------------
	void BufferImpl::updateDirect(const CopyFunc& func){
		if (!func)
			return;

		HRESULT result;
		byte* ptr = nullptr;
		result = m_resource->Map(0, nullptr, (void**)&ptr);
		if (FAILED(result))
		{
			Utility::OutputFatalLog(result, TC("ID3D12Resource::Map()"));
			return;
		}

		func(ptr);

		m_resource->Unmap(0, nullptr);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      定数バッファ―ビューを生成
	//@―---------------------------------------------------------------------------
	void BufferImpl::createCBV(D3D12_CPU_DESCRIPTOR_HANDLE handle)const {

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_resource->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = (UINT)m_desc.bufferSize;

		m_device.getNative()->CreateConstantBufferView(&cbvDesc, handle);

	}

}// ob::rhi::dx12 