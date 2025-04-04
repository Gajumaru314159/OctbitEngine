//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Plugins/DirectX12RHI/Buffer/BufferUploader.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <pix3.h>

namespace ob::rhi::dx12
{

	//! @brief  コンストラクタ
	BufferUploader::BufferUploader(ID3D12Device& device, size_t blockSize)
		: m_device(device)
	{
		m_blockSize = align_up(blockSize, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
		m_frames.resize(4);
		for (s32 i = 0; i < m_frames.size(); ++i) {
			size_t itemNum = m_blockSize / 256;
			m_frames.at(i).items.reserve(itemNum);
		}
	}

	//! @brief  アップロード要素を追加
	void BufferUploader::add(BlobView blob, const ComPtr<ID3D12Resource>& dest, size_t offset) {

		ScopeLock lock(m_lock);

		auto& frame = m_frames.current();

		// バッファが足りない場合は拡張
		if (!frame.available(blob.size())) {
			extend();
		}

		auto& block = frame.block();

		auto& item = frame.items.emplace_back();

		item.source = block.resource;
		item.dest = dest;
		item.sourceOffset = block.blob.size();
		item.destOffset = offset;
		item.size = blob.size();

		block.blob.append(blob.data(), blob.size());

	}

	void BufferUploader::add(const Buffer::CopyFunc& func, size_t size, const ComPtr<ID3D12Resource>& dest, size_t offset) {

		ScopeLock lock(m_lock);

		auto& frame = m_frames.current();

		// バッファが足りない場合は拡張
		if (!frame.available(size)) {
			extend();
		}

		auto& block = frame.block();

		auto& item = frame.items.emplace_back();

		item.source = block.resource;
		item.dest = dest;
		item.sourceOffset = block.blob.size();
		item.destOffset = offset;
		item.size = size;

		block.blob.resize(block.blob.size() + size);

		func(block.blob.data() + item.sourceOffset);
	}

	//! @brief アップロードバッファを拡大する
	void BufferUploader::extend() {

		auto& frame = m_frames.current();

		frame.blockIndex++;

		// 確保済みバッファがある場合はインデックスだけ進める
		if (frame.blockIndex < frame.blocks.size()) {
			return;
		}

		auto& block = frame.blocks.emplace_back();

		auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(m_blockSize);

		// バッファ生成
		m_device.CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(block.resource.GetAddressOf()));


		Utility::SetName(block.resource.Get(), "BufferUploaderBlock");

		// 一次バッファ生成
		block.blob.reserve(m_blockSize);

	}

	//! @brief アップロードバッファを縮小する
	void BufferUploader::shurink() {

		auto& frame = m_frames.current();

		if (frame.blocks.empty()) return;

		frame.blocks.pop_back();

	}

	void BufferUploader::update(ID3D12GraphicsCommandList& commandList) {

		ScopeLock lock(m_lock);

		auto& frame = m_frames.current();


		for (s32 i = 0; i <= frame.blockIndex; ++i) {
			auto& block = frame.blocks.at(i);
			void* data;
			auto result = block.resource->Map(0, nullptr, &data);
			if (FAILED(result)) {
				Utility::OutputErrorLog(result, "ID3D12Resource::Map()");
				continue;
			}

			memcpy_s(data, m_blockSize, block.blob.data(), block.blob.size());
			block.resource->Unmap(0, nullptr);
		}

		::PIXBeginEvent(&commandList, PIX_COLOR_DEFAULT, L"BufferUploader");

		// blocks 事前バリア設定は暗黙的な昇格を使用 (COMMON or GENERIC_READ > COPY_SOURCE)
		// items  事前バリア設定は暗黙的な昇格を使用 (COMMON > COPY_DEST)

		// コピー
		for (auto& item : frame.items) {
			commandList.CopyBufferRegion(item.dest.Get(), item.destOffset, item.source.Get(), item.sourceOffset, item.size);
		}

		// blocks 事前バリア設定は暗黙的な降格を使用 (COPY_SOURCE > COMMON) ※ExecuteCommandLists後
		m_barriers.clear();
		for (auto& item : frame.items) {
			auto& barrier = m_barriers.emplace_back();
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = item.dest.Get();
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
		}
		if (!m_barriers.empty()) {
			commandList.ResourceBarrier(m_barriers.size(), m_barriers.data());
		}

		// バッファを縮小
		frame.clear();

		::PIXEndEvent(&commandList);

	}

}