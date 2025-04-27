//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Plugins/DirectX12RHI/Buffer/DirectX12Buffer.h>
#include <Plugins/DirectX12RHI/DirectX12RHI.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>

namespace ob::rhi {

	//! @brief  BufferState を D3D12_RESOURCE_STATESに変換
	static D3D12_RESOURCE_STATES Convert(BufferState value) {
		switch (value)
		{
		case BufferState::Common:					return D3D12_RESOURCE_STATE_COMMON;
		case BufferState::Vertex:					return D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
		case BufferState::Index:					return D3D12_RESOURCE_STATE_INDEX_BUFFER;
		case BufferState::Constant:					return D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
		case BufferState::UnorderedAccess:			return D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		case BufferState::ShaderResource:			return D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE; // TODO ステートが必要か
		case BufferState::IndirectArgument:			return D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;
		case BufferState::CopySource:				return D3D12_RESOURCE_STATE_COPY_SOURCE;
		case BufferState::CopyDest:					return D3D12_RESOURCE_STATE_COPY_DEST;
		}
		LOG_ERROR("不正なバッファ状態です。");
		return {};
	}

	//! @brief  BufferFlags を D3D12_RESOURCE_FLAGS に変換	
	static D3D12_RESOURCE_FLAGS Convert(BufferFlags value) {
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;
		if (!value.has(BufferFlag::ShaderResource)) flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
		if (value.has(BufferFlag::UnorderedAccess))flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

		// D3D12では使用しない
		// if (value.has(BufferFlag::CopySource));
		// if (value.has(BufferFlag::CopyDest));
		// if (value.has(BufferFlag::Vertex));
		// if (value.has(BufferFlag::Index));
		// if (value.has(BufferFlag::Constant));
		// if (value.has(BufferFlag::IndirectArgument));

		return flags;
	}


	//! @brief  コンストラクタ
	//! 
	//! @param desc バッファ定義
	DirectX12Buffer::DirectX12Buffer(DirectX12RHI& rDevice, const BufferDesc& desc)
		: m_device(rDevice)
		, m_desc(desc)
	{

		if (!m_desc.isValid()) return;

		HRESULT result;

		// リソースの生成
		D3D12_HEAP_PROPERTIES heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		UINT64 alignment = 0; // PlacedBuffer対応時に設定
		D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON; // D3D12はパフォーマンス上暗黙的な状態遷移を使用するのでCommonを利用する
		D3D12_RESOURCE_FLAGS flags = Convert(m_desc.flags);

		D3D12_RESOURCE_DESC resdesc = CD3DX12_RESOURCE_DESC::Buffer(m_desc.size,flags, alignment);

		// TODO ReadBack対応
		// if(m_desc.state == BufferState::CopyDesc) {
		//	state = D3D12_RESOURCE_STATE_COPY_DEST;
		//	heapprop.Type = D3D12_HEAP_TYPE_READBACK;
		//	
		
		// ByteAddressBufferはSRVのFormatをR32_TYPELESSにする必要がある?
		// resdesc.Format = DXGI_FORMAT_R32_TYPELESS;

		ComPtr<ID3D12Resource> buffer;
		result = rDevice.getNative()->CreateCommittedResource(&heapprop,D3D12_HEAP_FLAG_NONE,&resdesc,state,nullptr,IID_PPV_ARGS(buffer.GetAddressOf()));

		if (FAILED(result))
		{
			Utility::OutputFatalLog(result, "ID3D12Device::CreateCommittedResource()");
			return;
		}

		m_resource = buffer;
		Utility::SetName(m_resource.Get(), getName());

		manage();
	}


	//! @brief  コンストラクタ
	//! 
	//! @param desc バッファ定義
	//! @param data 初期化データ
	DirectX12Buffer::DirectX12Buffer(DirectX12RHI& rDevice, const BufferDesc& desc, const Blob& blob)
		: DirectX12Buffer(rDevice,desc)
	{
		if (!isValid())return; 
		// TODO
		OB_NOTIMPLEMENTED();

		manage();
	}


	//! @brief  妥当な状態か
	bool DirectX12Buffer::isValid()const {
		return m_resource;
	}


	//! @brief      名前を取得
	const String& DirectX12Buffer::getName()const {
		return m_desc.name;
	}


	//! @brief  定義を取得
	const BufferDesc& DirectX12Buffer::getDesc()const {
		return m_desc;
	}


	//! @brief      バッファを更新
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	void DirectX12Buffer::update(size_t size, const void* pData, size_t offset) {
		updateDirect(size,pData,offset);
	}


	//! @brief      バッファを更新
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	void DirectX12Buffer::update(const CopyFunc& func) {
		// TODO 間接更新
		updateDirect(func);
	}


	//! @brief      バッファを更新(直接更新)
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	void DirectX12Buffer::updateDirect(size_t size, const void* data, size_t offset) {
		if (data == nullptr) return;
		m_device.getBufferUploader().add(BlobView(data, size), m_resource, offset);
	}


	//! @brief      バッファを更新(直接更新)
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	void DirectX12Buffer::updateDirect(const CopyFunc& func){
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
	void DirectX12Buffer::createCBV(D3D12_CPU_DESCRIPTOR_HANDLE handle)const {

		D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
		desc.BufferLocation = m_resource->GetGPUVirtualAddress();
		desc.SizeInBytes = (UINT)m_desc.size;

		m_device.getNative()->CreateConstantBufferView(&desc, handle);

	}


	//! @brief      SRVを生成
	void DirectX12Buffer::createSRV(D3D12_CPU_DESCRIPTOR_HANDLE handle)const {

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
	void DirectX12Buffer::createUAV(D3D12_CPU_DESCRIPTOR_HANDLE handle)const {

		bool isStructuredBuffer = 0 < m_desc.stride;

		D3D12_UNORDERED_ACCESS_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		desc.Buffer.FirstElement = 0;
		desc.Buffer.NumElements = m_desc.size;
		desc.Buffer.StructureByteStride = m_desc.stride;
		desc.Buffer.CounterOffsetInBytes = 0; // 何？
		desc.Buffer.Flags = isStructuredBuffer ? D3D12_BUFFER_UAV_FLAG_NONE : D3D12_BUFFER_UAV_FLAG_RAW;

		// TODO DXGI_FORMAT_R32_TYPELESS対応
		OB_NOTIMPLEMENTED();

		// TODO pCounterResource の調査
		m_device.getNative()->CreateUnorderedAccessView(m_resource.Get(),nullptr, &desc, handle);

	}

}// ob::rhi 