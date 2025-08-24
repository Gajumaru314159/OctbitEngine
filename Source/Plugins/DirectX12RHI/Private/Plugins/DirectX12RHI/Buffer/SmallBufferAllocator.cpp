//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Plugins/DirectX12RHI/Buffer/SmallBufferAllocator.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>

namespace ob::rhi {

	//! @brief バッファ記述からアライメント要求を取得
	size_t SmallBufferAllocator::GetAlignmentFromUsage(const BufferDesc& desc) {
		switch (desc.state) {
		case BufferState::Vertex:
			return 1; // アライメントなし（頂点バッファ等）
		case BufferState::Index:
			return desc.stride > 0 ? desc.stride : 4; // strideで判定（2 or 4バイト）、strideが0の場合は4バイトとする
		case BufferState::Constant:
			return 256; // 定数バッファ
		case BufferState::ShaderResource:
			return 16; // ByteAddressBuffer
		default:
			return 1; // アライメントなし
		}
	}

	//! @brief BufferChunkコンストラクタ
	SmallBufferAllocator::BufferChunk::BufferChunk(ID3D12Device& device, D3D12_HEAP_TYPE heapType, size_t size)
		: mapper(static_cast<s32>(size))
		, heapType(heapType)
	{
		// リソース記述を設定
		D3D12_RESOURCE_DESC resourceDesc = {};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Alignment = 0; // 0でデフォルトの64KBアライメント
		resourceDesc.Width = size;
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		// ヒーププロパティを設定
		D3D12_HEAP_PROPERTIES heapProps = {};
		heapProps.Type = heapType;
		heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProps.CreationNodeMask = 1;
		heapProps.VisibleNodeMask = 1;

		// リソースを作成
		HRESULT hr = device.CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&resource)
		);

		if (FAILED(hr)) {
			Utility::OutputFatalLog(hr, "SmallBufferAllocator::BufferChunk::CreateCommittedResource");
		}
	}

	//! @brief コンストラクタ
	SmallBufferAllocator::SmallBufferAllocator(ID3D12Device& device, D3D12_HEAP_TYPE heapType, size_t chunkSize)
		: m_device(device)
		, m_heapType(heapType)
		, m_chunkSize(chunkSize)
	{
	}

	//! @brief デストラクタ
	SmallBufferAllocator::~SmallBufferAllocator() {
	}

	//! @brief バッファをアロケート
	BufferAllocation SmallBufferAllocator::allocate(size_t size, size_t alignment) {
		ScopeLock lock(m_spinLock);

		size_t alignedSize = core::align_up(size, alignment);

		BufferAllocation allocation = {};
		allocation.size = size;
		allocation.isSubAllocation = true;
		allocation.allocator = this;

		for (auto& chunk : m_chunks) {

			// チャンクからメモリを確保
			if (auto block = chunk->mapper.allocate(static_cast<s32>(alignedSize))) {

				// アライメント調整されたオフセットを計算
				size_t offset = static_cast<size_t>(block->index);
				if (alignment > 1) {
					offset = (offset + alignment - 1) & ~(alignment - 1);
				}

				allocation.resource = chunk->resource;
				allocation.offset = offset;
				allocation.block = block;

				return allocation;
			}

		}

		// 新しいチャンクを作成
		auto newChunk = createChunk();
		auto chunk = newChunk.get();
		m_chunks.push_back(std::move(newChunk));

		// チャンクからメモリを確保
		const TLSFBlock* block = chunk->mapper.allocate(static_cast<s32>(alignedSize));
		if (!block) {
			LOG_FATAL_EX("Graphic", "SmallBufferAllocatorでのアロケートに失敗しました。[size={}]", alignedSize);
			return {};
		}

		// アライメント調整されたオフセットを計算
		size_t offset = static_cast<size_t>(block->index);
		if (alignment > 1) {
			offset = (offset + alignment - 1) & ~(alignment - 1);
		}

		allocation.resource = chunk->resource;
		allocation.offset = offset;
		allocation.block = block;

		return allocation;
	}

	//! @brief バッファを解放
	void SmallBufferAllocator::free(const BufferAllocation& allocation) {
		if (!allocation.isSubAllocation || !allocation.block) {
			return;
		}

		ScopeLock lock(m_spinLock);

		// 該当するチャンクを検索して解放
		for (auto& chunk : m_chunks) {
			if (chunk->mapper.contains(allocation.block)) {
				chunk->mapper.free(allocation.block);
				return;
			}
		}

		LOG_WARNING_EX("Graphic", "SmallBufferAllocatorで管理されていないブロックの解放が試行されました。");
	}


	//! @brief 新しいチャンクを作成
	UPtr<SmallBufferAllocator::BufferChunk> SmallBufferAllocator::createChunk() {
		return std::make_unique<BufferChunk>(m_device, m_heapType, m_chunkSize);
	}

	//! @brief 指定サイズを確保できるチャンクを検索
	SmallBufferAllocator::BufferChunk* SmallBufferAllocator::findAvailableChunk(size_t alignedSize) {
		for (auto& chunk : m_chunks) {
			// TLSFMapperで確保可能かテスト（実際には確保せずにテストのみ）
			// 簡易的な実装として、十分な空きがありそうなチャンクを返す
			if (chunk->mapper.capacity() >= static_cast<s32>(alignedSize)) {
				return chunk.get();
			}
		}
		return nullptr;
	}

}