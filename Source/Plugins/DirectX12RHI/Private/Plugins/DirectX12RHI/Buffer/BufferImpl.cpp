//***********************************************************
//! @file
//! @brief		コマンドキュー
//! @author		Gajumaru
//***********************************************************
#include <Plugins/DirectX12RHI/Buffer/BufferImpl.h>
#include <Plugins/DirectX12RHI/DirectX12RHI.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>

namespace ob::rhi::dx12 {

	//! @brief バリデート
	static bool IsInvalid(BufferDesc& desc) {

		if (desc.type == BufferType::ConstantBuffer && desc.size % D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT != 0) {
			LOG_WARNING("定数バッファは256の倍数で作成する必要があります。サイズを{}から{}に調整します。 [name={}]", desc.name, desc.size, align_up(desc.size, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT));
			desc.size = align_up(desc.size, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
		}

		if (desc.size == 0) {
			LOG_WARNING("バッファサイズは0より大きくなくてはいけません。サイズを256に設定します。 [name={}]", desc.name);
			desc.size = 256;
		}

		return false;
	}

	//! @brief  コンストラクタ
	//! 
	//! @param desc バッファ定義
	BufferImpl::BufferImpl(DirectX12RHI& rDevice, const BufferDesc& desc)
		: m_device(rDevice)
		, m_desc(desc)
	{

		if (IsInvalid(m_desc))return;

		HRESULT result;

		// リソースの生成
		D3D12_HEAP_PROPERTIES heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		D3D12_RESOURCE_DESC resdesc = CD3DX12_RESOURCE_DESC::Buffer(m_desc.size);

		ComPtr<ID3D12Resource> buffer;
		result = rDevice.getNative()->CreateCommittedResource(&heapprop,D3D12_HEAP_FLAG_NONE,&resdesc,D3D12_RESOURCE_STATE_COMMON,nullptr,IID_PPV_ARGS(buffer.GetAddressOf()));

		if (FAILED(result))
		{
			Utility::OutputFatalLog(result, "ID3D12Device::CreateCommittedResource()");
			return;
		}

		m_resource = buffer;
		Utility::SetName(m_resource.Get(), getName());
	}


	//! @brief  コンストラクタ
	//! 
	//! @param desc バッファ定義
	//! @param data 初期化データ
	BufferImpl::BufferImpl(DirectX12RHI& rDevice, const BufferDesc& desc, const Blob& blob)
		: BufferImpl(rDevice,desc)
	{
		if (!isValid())return;
		// TODO
		OB_NOTIMPLEMENTED();
	}


	//! @brief  妥当な状態か
	bool BufferImpl::isValid()const {
		return m_resource;
	}


	//! @brief      名前を取得
	const String& BufferImpl::getName()const {
		return m_desc.name;
	}


	//! @brief  定義を取得
	const BufferDesc& BufferImpl::getDesc()const {
		return m_desc;
	}


	//! @brief      バッファを更新
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	void BufferImpl::update(size_t size, const void* pData, size_t offset) {
		updateDirect(size,pData,offset);
	}


	//! @brief      バッファを更新
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	void BufferImpl::update(const CopyFunc& func) {
		// TODO 間接更新
		updateDirect(func);
	}


	//! @brief      バッファを更新(直接更新)
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	void BufferImpl::updateDirect(size_t size, const void* data, size_t offset) {
		if (data == nullptr) return;

		m_device.getBufferUploader().add(BlobView(data, size), m_resource, offset);


		/*
		HRESULT result;
		byte* ptr = nullptr;
		result = m_resource->Map(0, nullptr, (void**)&ptr);
		if (FAILED(result))
		{
			Utility::OutputFatalLog(result, "ID3D12Resource::Map()");
			return;
		}
		memcpy_s(ptr+offset, (s64)m_desc.bufferSize-offset, pData, size);
		m_resource->Unmap(0, nullptr);
		*/
	}


	//! @brief      バッファを更新(直接更新)
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	void BufferImpl::updateDirect(const CopyFunc& func){
		if (!func) return;


		m_device.getBufferUploader().add(func, m_desc.size, m_resource, 0);

		return;

		HRESULT result;
		byte* ptr = nullptr;
		result = m_resource->Map(0, nullptr, (void**)&ptr);
		if (FAILED(result))
		{
			Utility::OutputFatalLog(result, "ID3D12Resource::Map()");
			return;
		}

		func(ptr);

		m_resource->Unmap(0, nullptr);
	}


	//! @brief      CBVを生成
	void BufferImpl::createCBV(D3D12_CPU_DESCRIPTOR_HANDLE handle)const {

		D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
		desc.BufferLocation = m_resource->GetGPUVirtualAddress();
		desc.SizeInBytes = (UINT)m_desc.size;

		m_device.getNative()->CreateConstantBufferView(&desc, handle);

	}


	//! @brief      SRVを生成
	void BufferImpl::createSRV(D3D12_CPU_DESCRIPTOR_HANDLE handle)const {

		bool isStructuredBuffer = 0 < m_desc.stride;

		D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		desc.Buffer.FirstElement = 0;
		desc.Buffer.NumElements = m_desc.size/4;
		desc.Buffer.StructureByteStride = isStructuredBuffer ? m_desc.stride : 0;
		desc.Buffer.Flags = isStructuredBuffer ? D3D12_BUFFER_SRV_FLAG_NONE : D3D12_BUFFER_SRV_FLAG_RAW;

		m_device.getNative()->CreateShaderResourceView(m_resource.Get(), &desc, handle);

	}


	//! @brief      UAVを生成
	void BufferImpl::createUAV(D3D12_CPU_DESCRIPTOR_HANDLE handle)const {

		OB_NOTIMPLEMENTED();

		bool isStructuredBuffer = 0 < m_desc.stride;

		D3D12_UNORDERED_ACCESS_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		desc.Buffer.FirstElement = 0;
		desc.Buffer.NumElements = m_desc.size;
		desc.Buffer.StructureByteStride = isStructuredBuffer ? m_desc.stride : 0;
		desc.Buffer.CounterOffsetInBytes = 0; // 何？
		desc.Buffer.Flags = isStructuredBuffer ? D3D12_BUFFER_UAV_FLAG_NONE : D3D12_BUFFER_UAV_FLAG_RAW;

		// TODO pCounterResource の調査
		m_device.getNative()->CreateUnorderedAccessView(m_resource.Get(),nullptr, &desc, handle);

	}

}// ob::rhi::dx12 