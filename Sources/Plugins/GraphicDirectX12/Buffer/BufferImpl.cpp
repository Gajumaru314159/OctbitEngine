//***********************************************************
//! @file
//! @brief		コマンドキュー
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Plugins/GraphicDirectX12/Buffer/BufferImpl.h>
#include <Framework/Graphic/Private/Device.h>
#include <Plugins/GraphicDirectX12/Device/DeviceImpl.h>
#include <Plugins/GraphicDirectX12/Utility/Utility.h>
#include <Plugins/GraphicDirectX12/Utility/TypeConverter.h>

namespace ob::graphic::dx12 {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//! 
	//! @param desc バッファ定義
	//@―---------------------------------------------------------------------------
	BufferImpl::BufferImpl(DeviceImpl& rDevice, const BufferDesc& desc)
		: m_desc(desc)
	{
		HRESULT result;
		// リソースの生成
		D3D12_HEAP_PROPERTIES heapprop = {};
		// TODO UploadResource作成
		heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC resdesc = {};
		resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resdesc.Width = desc.bufferSize;
		resdesc.Height = 1;
		resdesc.DepthOrArraySize = 1;
		resdesc.MipLevels = 1;
		resdesc.Format = DXGI_FORMAT_UNKNOWN;
		resdesc.SampleDesc.Count = 1;
		resdesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


		ComPtr<ID3D12Resource> buffer;

		//UPLOAD(確保は可能)
		result = rDevice.getNativeDevice()->CreateCommittedResource(
			&heapprop,
			D3D12_HEAP_FLAG_NONE,
			&resdesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(buffer.ReleaseAndGetAddressOf()));

		if (FAILED(result))
		{
			Utility::outputFatalLog(result, TC("ID3D12Device::CreateCommittedResource()"));
			return;
		}

		m_buffer = buffer;

	}


	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//! 
	//! @param desc バッファ定義
	//! @param data 初期化データ
	//@―---------------------------------------------------------------------------
	BufferImpl::BufferImpl(DeviceImpl& rDevice, const BufferDesc& desc, const Blob& blob)
		: m_desc(desc)
	{

	}


	//@―---------------------------------------------------------------------------
	//! @brief  妥当な状態か
	//@―---------------------------------------------------------------------------
	bool BufferImpl::isValid()const {
		return m_buffer;
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
		OB_NOTIMPLEMENTED();
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
		result = m_buffer->Map(0, nullptr, (void**)&ptr);
		if (FAILED(result))
		{
			Utility::outputFatalLog(result, TC("ID3D12Resource::Map()"));
			return;
		}
		memcpy_s(ptr+offset, (s64)m_desc.bufferSize-offset, pData, size);
		m_buffer->Unmap(0, nullptr);

	}

}// ob::graphic::dx12 