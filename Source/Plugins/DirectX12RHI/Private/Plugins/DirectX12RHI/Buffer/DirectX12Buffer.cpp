//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Plugins/DirectX12RHI/Buffer/DirectX12Buffer.h>
#include <Plugins/DirectX12RHI/DirectX12Device.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Buffer/SmallBufferAllocator.h>

namespace ob::rhi {

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
	DirectX12Buffer::DirectX12Buffer(DirectX12Device& device, const BufferDesc& desc)
		: m_device(device)
		, m_desc(desc)
	{
		if (!m_desc.isValid()) return;

		// SmallBufferAllocatorを使用できるかチェック
		auto canUseSmallAllocator =
			(desc.flags & BufferFlag::UnorderedAccess) &&
			(1 < BitOp::GetBitCount(static_cast<u32>(desc.flags))) &&
			(desc.size > 65536);

		// SmallBufferAllocatorを試す
		if (canUseSmallAllocator) {
			auto& allocator = device.getSmallBufferAllocator(desc.flags.get_enum());
			size_t alignment = SmallBufferAllocator::GetAlignmentFromUsage(desc);
			auto allocation = allocator.allocate(desc.size, alignment);
			
			if (allocation.resource) {
				// サブアロケーション成功
				m_resource = allocation.resource;
				m_allocation = allocation;
				return;
			}
		}

		// SmallBufferAllocatorを使用しない、またはサブアロケーションに失敗した場合は個別リソース生成
		if (!m_resource) {
			// フォールバック：従来の個別リソース生成
			HRESULT result;

			// リソースの生成
			D3D12_HEAP_PROPERTIES heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			UINT64 alignment = 0; // PlacedBuffer対応時に設定
			D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON; // D3D12はパフォーマンス上暗黙的な状態遷移を使用するのでCommonを利用する
			D3D12_RESOURCE_FLAGS flags = Convert(m_desc.flags);

			D3D12_RESOURCE_DESC resdesc = CD3DX12_RESOURCE_DESC::Buffer(m_desc.size,flags, alignment);

			// TODO ReadBack対応

			ComPtr<ID3D12Resource> buffer;
			result = device.getNative()->CreateCommittedResource(&heapprop,D3D12_HEAP_FLAG_NONE,&resdesc,state,nullptr,IID_PPV_ARGS(buffer.GetAddressOf()));

			if (FAILED(result))
			{
				Utility::OutputFatalLog(result, "ID3D12Device::CreateCommittedResource()");
				return;
			}

			m_resource = buffer;
			Utility::SetName(m_resource.Get(), m_desc.name);
		}

		if (desc.flags & BufferFlag::Constant) {
			device.allocateHandle(DescriptorHeapType::CBV_SRV_UAV, m_handle, 1);
			createCBV(m_handle.getCpuHandle());
			m_viewDesc.type = BufferViewType::ConstantBuffer;
		} else if (desc.flags & BufferFlag::ShaderResource) {
			device.allocateHandle(DescriptorHeapType::CBV_SRV_UAV, m_handle, 1);
			createSRV(m_handle.getCpuHandle());
			m_viewDesc.type = BufferViewType::ByteAddressBuffer;
		} else if (desc.flags & BufferFlag::UnorderedAccess) {
			device.allocateHandle(DescriptorHeapType::CBV_SRV_UAV, m_handle, 1);
			createUAV(m_handle.getCpuHandle());
			m_viewDesc.type = BufferViewType::RWByteAddressBuffer;
		} else {
			m_viewDesc.type = BufferViewType::None;
		}

		manage();
	}


	//! @brief  デストラクタ
	DirectX12Buffer::~DirectX12Buffer() {
		// サブアロケーションの場合は解放処理を行う
		if (m_allocation.allocator) {
			m_allocation.allocator->free(m_allocation);
			m_allocation.allocator = nullptr;
		}
	}


	//! @brief  コンストラクタ
	DirectX12Buffer::DirectX12Buffer(DirectX12Device& device, const BufferDesc& desc, const Blob& blob)
		: DirectX12Buffer(device,desc)
	{
		update(blob.size(), blob.data(), 0);
	}

	//! @brief  コンストラクタ
	DirectX12Buffer::DirectX12Buffer(DirectX12Device& device, const BufferViewDesc& desc)
		: m_device(device)
		, m_viewDesc(desc)
		, m_allocation(nullptr)
	{
		auto base = desc.base.cast<DirectX12Buffer>();

		if (base == nullptr) {
			LOG_ERROR("ベースバッファが指定されていません");
			return;
		}

		m_desc = base->m_desc;

		if (!m_desc.isValid()) return;

		device.allocateHandle(DescriptorHeapType::CBV_SRV_UAV, m_handle, 1);

		auto flags = m_desc.flags;

		switch (desc.type) {
		case BufferViewType::ConstantBuffer:
			if (flags & BufferFlag::Constant) {
				createCBV(m_handle.getCpuHandle());
			}
			break;
		case BufferViewType::Buffer:
		case BufferViewType::StructuredBuffer:
		case BufferViewType::ByteAddressBuffer:
			if (flags & BufferFlag::ShaderResource) {
				createSRV(m_handle.getCpuHandle());
			}
			break;
		case BufferViewType::RWBuffer:
		case BufferViewType::RWStructuredBuffer:
		case BufferViewType::RWByteAddressBuffer:
			if (flags & BufferFlag::UnorderedAccess) {
				createUAV(m_handle.getCpuHandle());
			}
			break;
		default:
			OB_ABORT("未実装のBufferViewType");
			break;
		}

		m_resource = base->m_resource;
		m_allocation = base->m_allocation;
		m_allocation.allocator = nullptr; // ベースバッファに解放を任せる

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
		m_device.getBufferUploader().add(BlobView(data, size), m_resource, offset + m_allocation.offset);
	}


	//! @brief      バッファを更新(直接更新)
	//! 
	//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
	void DirectX12Buffer::updateDirect(const CopyFunc& func){
		if (!func) return;

		m_device.getBufferUploader().add(func, m_desc.size, m_resource, m_allocation.offset);
	}


	//! @brief      CBVを生成
	void DirectX12Buffer::createCBV(D3D12_CPU_DESCRIPTOR_HANDLE handle)const {

		OB_ASSERT(m_desc.flags & BufferFlag::Constant, "定数バッファを生成するには、BufferFlag::Constantフラグが必要です。 [name={}]", m_desc.name);

		D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
		desc.BufferLocation = m_resource->GetGPUVirtualAddress() + m_allocation.offset;
		desc.SizeInBytes = static_cast<UINT>(m_desc.size);

		m_device.getNative()->CreateConstantBufferView(&desc, handle);

	}


	//! @brief      SRVを生成
	void DirectX12Buffer::createSRV(D3D12_CPU_DESCRIPTOR_HANDLE handle)const {

		OB_ASSERT(m_desc.flags & BufferFlag::ShaderResource, "SRVを生成するには、BufferFlag::ShaderResourceフラグが必要です。 [name={}]", m_desc.name);

		bool isStructuredBuffer = 0 < m_desc.stride;

		D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = isStructuredBuffer ? DXGI_FORMAT_UNKNOWN : DXGI_FORMAT_R32_TYPELESS;
		desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		desc.Buffer.FirstElement = static_cast<UINT64>(m_allocation.offset / (isStructuredBuffer ? m_desc.stride : 4));
		desc.Buffer.NumElements = gsl::narrow<UINT>(isStructuredBuffer ? m_desc.size/m_desc.stride : m_desc.size/4);
		desc.Buffer.StructureByteStride = isStructuredBuffer ? m_desc.stride : 0;
		desc.Buffer.Flags = isStructuredBuffer ? D3D12_BUFFER_SRV_FLAG_NONE : D3D12_BUFFER_SRV_FLAG_RAW;

		m_device.getNative()->CreateShaderResourceView(m_resource.Get(), &desc, handle);

	}


	//! @brief      UAVを生成
	void DirectX12Buffer::createUAV(D3D12_CPU_DESCRIPTOR_HANDLE handle)const {

		OB_ASSERT(m_desc.flags & BufferFlag::UnorderedAccess, "UAVを生成するには、BufferFlag::UnorderedAccessフラグが必要です。 [name={}]", m_desc.name);

		bool isStructuredBuffer = 0 < m_desc.stride;

		// TODO DXGI_FORMAT_R32_TYPELESS対応
		if (m_resource->GetDesc().Format != DXGI_FORMAT_R32_TYPELESS) {
			LOG_ERROR("リソースがDXGI_FORMAT_R32_TYPELESSではないためUAVの生成ができません");
			return;
		}

		D3D12_UNORDERED_ACCESS_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		desc.Buffer.FirstElement = static_cast<UINT64>(m_allocation.offset / (isStructuredBuffer ? m_desc.stride : 4));
		desc.Buffer.StructureByteStride = m_desc.stride;
		desc.Buffer.NumElements = gsl::narrow<UINT>(isStructuredBuffer ? m_desc.size / m_desc.stride : m_desc.size / 4);
		desc.Buffer.CounterOffsetInBytes = 0; // 何？
		desc.Buffer.Flags = isStructuredBuffer ? D3D12_BUFFER_UAV_FLAG_NONE : D3D12_BUFFER_UAV_FLAG_RAW;

		// TODO pCounterResource の調査
		m_device.getNative()->CreateUnorderedAccessView(m_resource.Get(),nullptr, &desc, handle);

	}

}// ob::rhi 