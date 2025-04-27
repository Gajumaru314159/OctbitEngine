//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Plugins/DirectX12RHI/Buffer/BufferUploader.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <pix3.h>

namespace ob::rhi
{

	//! @brief  コンストラクタ
	BufferUploader::BufferUploader(ID3D12Device& device, size_t blockSize)
		: m_device(device)
	{
		m_blockSize = align_up(blockSize, D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
		m_frames.resize(4);
		for (s32 i = 0; i < m_frames.size(); ++i) {
			size_t requestNum = m_blockSize / 256;
			m_frames.at(i).requests.reserve(requestNum);
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

		auto& request = frame.requests.emplace_back();

		request.source = block.resource;
		request.dest = dest;
		request.sourceOffset = block.blob.size();
		request.destOffset = offset;
		request.size = blob.size();

		block.blob.append(blob.data(), blob.size());

		m_entriedResources.emplace(dest.Get());

	}

	void BufferUploader::add(const Buffer::CopyFunc& func, size_t size, const ComPtr<ID3D12Resource>& dest, size_t offset) {

		ScopeLock lock(m_lock);

		auto& frame = m_frames.current();

		// バッファが足りない場合は拡張
		if (!frame.available(size)) {
			extend();
		}

		auto& block = frame.block();

		auto& request = frame.requests.emplace_back();

		request.source = block.resource;
		request.dest = dest;
		request.sourceOffset = block.blob.size();
		request.destOffset = offset;
		request.size = size;

		block.blob.resize(block.blob.size() + size);

		func(block.blob.data() + request.sourceOffset);

		m_entriedResources.emplace(dest.Get());
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

	//! @brief 更新
	//! 
	//! この関数は1フレームに1回だけ呼び出す必要があります。
	void BufferUploader::update(ID3D12GraphicsCommandList& commandList) {

		ScopeLock lock(m_lock);

		auto& frame = m_frames.current();

		// システムメモリからアップロードバッファにコピー
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
		// requests  事前バリア設定は暗黙的な昇格を使用 (COMMON > COPY_DEST)

		// コピー
		for (auto& request : frame.requests) {
			commandList.CopyBufferRegion(request.dest.Get(), request.destOffset, request.source.Get(), request.sourceOffset, request.size);
		}

		// blocks 事前バリア設定は暗黙的な降格を使用 (COPY_SOURCE > COMMON) ※ExecuteCommandLists後
		// TODO 同じリソースが複数回使用される場合は、バリアをまとめて実行する
		m_barriers.clear();
		for (auto& resource : m_entriedResources) {
			auto& barrier = m_barriers.emplace_back();
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = resource;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
		}
		if (!m_barriers.empty()) {
			commandList.ResourceBarrier(m_barriers.size(), m_barriers.data());
		}

		// バッファを縮小
		frame.clear();
		m_entriedResources.clear();

		::PIXEndEvent(&commandList);

	}

}