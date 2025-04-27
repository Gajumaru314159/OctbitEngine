//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Plugins/DirectX12RHI/Texture/TextureUploader.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <pix3.h>

namespace ob::rhi
{

	//! @brief  コンストラクタ
	TextureUploader::TextureUploader(ID3D12Device& device)
		: m_device(device)
	{
		m_frames.resize(4);
	}

	//! @brief  アップロード要素を追加
	void TextureUploader::add(const ComPtr<ID3D12Resource>& dest, Span<Subresource> subresources) {

		if (dest == nullptr) {
			LOG_ERROR("[TextureUploader] destがnullです。");
			return;
		}

		auto desc = dest->GetDesc();
		desc.Alignment = 0;
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.Width = desc.Width * desc.Height * desc.DepthOrArraySize * desc.MipLevels * 8;
		desc.Height = 1;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		auto sourceResource = createUploadResource(desc);
		if (sourceResource == nullptr) {
			LOG_ERROR("[TextureUploader] コピーソースリソースの生成に失敗しました");
			return;
		}

		// 各Mipmapは別のメモリ領域にあることが多い
		
		// DDSレイアウトの場合
		// DirectUploadの場合
		// ミップスライス
		// 配列スライス
		// ボリュームテクスチャ

		
		// コピーソースにテクスチャデータを書き込み
		// TODO UMAではない場合WriteToSubresourceはコストがかかるのでGetCopyableFootprintsの結果をもとに自前でコピーする
		// https://learn.microsoft.com/ja-jp/windows/win32/api/d3d12/nf-d3d12-id3d12resource-writetosubresource

		if (m_isUMA) {

			for (auto [index, subresource] : Indexed(subresources)) {

				HRESULT result = sourceResource->WriteToSubresource(index, nullptr, subresource.data.data(), subresource.rowPitch, subresource.slicePitch);
				if (FAILED(result)) {
					Utility::OutputErrorLog(result, "ID3D12Resource::WriteToSubresource()");
					continue;
				}

			}

		} else {

			constexpr u32 maxSubresoureNum = BitOp::GetMSB((u32)D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION);

			D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint[maxSubresoureNum];
			UINT pNumRows[maxSubresoureNum];
			UINT64 pRowSizeInBytes[maxSubresoureNum];
			UINT64 pTotalBytes[maxSubresoureNum];
			m_device.GetCopyableFootprints(&desc, 0, desc.MipLevels, 0, footprint, pNumRows, pRowSizeInBytes, pTotalBytes);


#if 0
			// Map一回

#else
			// 個別Map
			s32 subresourceIndex = 0;
			for (s32 array = 0; array < desc.DepthOrArraySize; ++array) {

				for (s32 mipLevel = 0; mipLevel < desc.MipLevels; ++mipLevel) {

					void* data;
					HRESULT result = sourceResource->Map(0, nullptr, &data);
					if (SUCCEEDED(result)) {

						auto& subresource = subresources[subresourceIndex];
						UINT64 rowSizeInBytes = pRowSizeInBytes[mipLevel];
						// UINT64 rowSizeInBytes =  footprint[mipLevel].Footprint.RowPitch;

						for (s32 row = 0; row < pNumRows[subresourceIndex]; ++row) {

							void* dest = GetOffsetPtr(data, rowSizeInBytes * row);
							void* src = GetOffsetPtr(data, subresource.rowPitch * row);
							size_t size = subresource.rowPitch;

							memcpy(dest,src,size);

						}

						sourceResource->Unmap(0, nullptr);
					} else {
						Utility::OutputErrorLog(result, "ID3D12Resource::Map()");
					}
					
					subresourceIndex++;
				}

			}

#endif

		}

		ScopeLock lock(m_lock);

		auto& frame = m_frames.current();
		auto& request = frame.requests.emplace_back();
				
		request.source = sourceResource;
		request.dest = dest;

	}

    //! @brief アップロード用の一時リソースを作成 
    ComPtr<ID3D12Resource> TextureUploader::createUploadResource(const D3D12_RESOURCE_DESC& desc) {

		D3D12_HEAP_PROPERTIES  heap;
		memset(&heap, 0, sizeof(heap));
		heap.Type = D3D12_HEAP_TYPE_UPLOAD;

		ComPtr<ID3D12Resource> resource;
		auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto result = m_device.CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,IID_PPV_ARGS(resource.GetAddressOf()));

		if (FAILED(result)) {
			Utility::OutputErrorLog(result, "CreateCommittedResource()");
			return nullptr;
		}

		Utility::SetName(resource.Get(), "TextureUploaderBlock");

		return resource;
	}


	void TextureUploader::update(ID3D12GraphicsCommandList& commandList) {

		ScopeLock lock(m_lock);

		auto& frame = m_frames.current();

		::PIXBeginEvent(&commandList, PIX_COLOR_DEFAULT, L"TextureUploader");

		// blocks 事前バリア設定は暗黙的な昇格を使用 (COMMON or GENERIC_READ > COPY_SOURCE)
		// requests  事前バリア設定は暗黙的な昇格を使用 (COMMON > COPY_DEST)

		// コピー
		for (auto& request : frame.requests) {
			// リソース全体コピー
			commandList.CopyResource(request.dest.Get(),request.source.Get());

			// NOTE MipmapStreamingのように一部のサブリソースを既存の別リソースからコピーしてくる場合は
			// CopyTextureRegionでサブリソースごとにコピーする。

			//commandList.CopyTextureRegion(&request.destLocation, request.destOffset.x, request.destOffset.y, request.destOffset.z, &request.sourceLocation,&request.sourceBox);
		}

		// blocks 事前バリア設定は暗黙的な降格を使用 (COPY_SOURCE > COMMON) ※ExecuteCommandLists後
		// TODO 同じリソースが複数回使用される場合は、バリアをまとめて実行する
		m_barriers.clear();
		for (auto& request : frame.requests) {
			auto& barrier = m_barriers.emplace_back();
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = request.dest.Get();
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