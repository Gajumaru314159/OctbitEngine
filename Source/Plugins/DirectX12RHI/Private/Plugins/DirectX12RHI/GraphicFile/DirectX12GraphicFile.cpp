//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Plugins/DirectX12RHI/GraphicFile/DirectX12GraphicFile.h>
#include <Plugins/DirectX12RHI/Buffer/DirectX12Buffer.h>
#include <Plugins/DirectX12RHI/Texture/DirectX12Texture.h>
#include <Framework/Core/String/StringEncoder.h>
#include <Framework/Core/Misc/ErrorCode.h>
#include <Framework/Core/File/BinaryWriter.h>
#include <Framework/Core/File/File.h>
#include <Framework/Core/Misc/Compression.h>
#include <DirectXTex.h>

namespace ob::rhi {



	//! @brief コンストラクタ
	DirectX12GraphicFileHandle::DirectX12GraphicFileHandle(IDStorageFactory& factory,StringView path) {

		m_path = path;

		WString wpath;
		StringEncoder::Encode(path, wpath);

		ComPtr<IDStorageFile> file;
		auto hr = factory.OpenFile(wpath.c_str(), IID_PPV_ARGS(file.ReleaseAndGetAddressOf()));

		if (FAILED(hr)) {
			LOG_ERROR("DirectStorage Error : {}",ErrorCode(hr));
			return;
		}

		m_file = file;
	}

	bool DirectX12GraphicFileHandle::isValid()const {
		return m_file;
	}

	const String& DirectX12GraphicFileHandle::path()const {
		return m_path;
	}

	bool DirectX12GraphicFileHandle::empty()const {
		return !m_file;
	}

	size_t DirectX12GraphicFileHandle::size()const {
		BY_HANDLE_FILE_INFORMATION info;
		auto hr = m_file->GetFileInformation(&info);
		if (FAILED(hr))return 0;
		return ((size_t)info.nFileSizeHigh) << 32 | info.nFileSizeLow;
	}

	const ComPtr<IDStorageFile>& DirectX12GraphicFileHandle::file()const {
		return m_file;
	}


	
	DirectX12GraphicFileEvent::DirectX12GraphicFileEvent() {
	}
	
	DirectX12GraphicFileEvent::~DirectX12GraphicFileEvent() {

		if (m_handle)
		{
			CloseHandle(m_handle);
		}
	}

	bool DirectX12GraphicFileEvent::isValid()const {
		return true;
	}

	void DirectX12GraphicFileEvent::set(ComPtr<ID3D12Fence> fence,HANDLE handle) {
		m_handle = handle;
		m_fence = fence;
	}
	bool DirectX12GraphicFileEvent::poll() const {
		return WaitForSingleObject(m_handle, 0) == WAIT_OBJECT_0;
	}
	void DirectX12GraphicFileEvent::wait() const {
		WaitForSingleObject(m_handle, INFINITE);
	}




	//! @brief コンストラクタ
	DirectX12GraphicFileQueue::DirectX12GraphicFileQueue(ID3D12Device8& device,IDStorageFactory& factory, const GraphicFileQueueDesc& desc)
		: m_desc(desc)
		, m_device(device)
	{

		DSTORAGE_QUEUE_DESC dsdesc{};
		dsdesc.Capacity = desc.capacity == 0 ? DSTORAGE_MAX_QUEUE_CAPACITY : desc.capacity;
		dsdesc.Priority = DSTORAGE_PRIORITY_NORMAL;
		dsdesc.SourceType = DSTORAGE_REQUEST_SOURCE_FILE;
		dsdesc.Name = m_desc.name.c_str();
		dsdesc.Device = &m_device;

		ComPtr<IDStorageQueue1> queue;
		auto hr = factory.CreateQueue(&dsdesc, IID_PPV_ARGS(queue.ReleaseAndGetAddressOf()));

		if (FAILED(hr)) {
			LOG_ERROR("DirectStorage Error : {}", ErrorCode(hr));
			return;
		}

		m_queue = queue;
	}

	bool DirectX12GraphicFileQueue::isValid()const {
		return m_queue;
	}

	//! @brief 非同期読み込みリクエストを追加
	void DirectX12GraphicFileQueue::add(const GraphicFileRequest& desc) {

		// ファイルハンドルが無効な場合は何もしない
		auto handle = desc.handle.cast<DirectX12GraphicFileHandle>();
		if (handle == nullptr || handle->file() == nullptr) return;

		bool useDecompression = 0 < desc.uncompressedSize;

		DSTORAGE_REQUEST request{};
		request.Options.CompressionFormat = useDecompression ? DSTORAGE_COMPRESSION_FORMAT_GDEFLATE : DSTORAGE_COMPRESSION_FORMAT_NONE;
		request.Options.SourceType = DSTORAGE_REQUEST_SOURCE_FILE;
		request.Source.File.Source = handle->file().Get();
		request.Source.File.Offset = desc.offset;
		request.Source.File.Size = desc.size;
		request.Name = handle->path().c_str();

		if (std::holds_alternative<GraphicFileRequest::BufferDesc>(desc.dest)) {
			auto& bufferDesc = std::get<GraphicFileRequest::BufferDesc>(desc.dest);
			auto buffer = bufferDesc.buffer.cast<DirectX12Buffer>();
			if (buffer == nullptr) return;
			request.Options.DestinationType = DSTORAGE_REQUEST_DESTINATION_BUFFER;
			request.Destination.Buffer.Resource = buffer->getNative();
			request.Destination.Buffer.Offset = bufferDesc.offset;
			request.Destination.Buffer.Size = useDecompression ? desc.uncompressedSize : desc.size;
			request.UncompressedSize = useDecompression ? desc.uncompressedSize : 0;
		}

		if (std::holds_alternative<GraphicFileRequest::TextureDesc>(desc.dest)) {
			auto& textureDesc = std::get<GraphicFileRequest::TextureDesc>(desc.dest);
			auto texture = textureDesc.texture.cast<DirectX12Texture>();
			if (texture == nullptr) return;

			auto size = texture->size();
			bool isBC = TextureFormatUtility::IsBC(texture->format());

			request.Options.DestinationType = DSTORAGE_REQUEST_DESTINATION_TEXTURE_REGION;
			request.Destination.Texture.Resource = texture->getResource();
			request.Destination.Texture.SubresourceIndex = textureDesc.subresourceIndex;
			request.Destination.Texture.Region.left = 0;
			request.Destination.Texture.Region.top = 0;
			request.Destination.Texture.Region.front = 0;
			request.Destination.Texture.Region.right = std::max(1,size.width >> textureDesc.subresourceIndex);
			request.Destination.Texture.Region.bottom = std::max(1, size.height >> textureDesc.subresourceIndex);
			request.Destination.Texture.Region.back = std::max(1, size.depth >> textureDesc.subresourceIndex);

			if (isBC) {
				request.Destination.Texture.Region.right = align_up(request.Destination.Texture.Region.right,4);
				request.Destination.Texture.Region.bottom = align_up(request.Destination.Texture.Region.bottom, 4);
			}

			request.UncompressedSize = useDecompression ? desc.uncompressedSize : 0;
		}

		if (std::holds_alternative<GraphicFileRequest::TextureSequenceDesc>(desc.dest)) {
			auto& textureSequenceDesc = std::get<GraphicFileRequest::TextureSequenceDesc>(desc.dest);
			auto texture = textureSequenceDesc.texture.cast<DirectX12Texture>();
			if (texture == nullptr) return;
			request.Options.DestinationType = DSTORAGE_REQUEST_DESTINATION_MULTIPLE_SUBRESOURCES;
			request.Destination.MultipleSubresources.Resource = texture->getResource();
			request.Destination.MultipleSubresources.FirstSubresource = textureSequenceDesc.firstSubresourceIndex;
			request.UncompressedSize = useDecompression ? desc.uncompressedSize : 0;
		}

		m_queue->EnqueueRequest(&request);

	}

	//! @brief 非同期読み込み完了同期イベントを追加 
	void DirectX12GraphicFileQueue::add(Ref<GraphicFileEvent>& event) {
		auto impl = event.cast<DirectX12GraphicFileEvent>();
		if (impl == nullptr)return;	

		HRESULT hr;
		ComPtr<ID3D12Fence> fence;
		hr = m_device.CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf()));
		if (FAILED(hr)) {
			LOG_ERROR("DirectStorage Error : {}", ErrorCode(hr));
			return;
		}

		HANDLE handle = ::CreateEventA(nullptr, false, false, nullptr);

		constexpr uint64_t fenceValue = 1;
		hr = fence->SetEventOnCompletion(fenceValue, handle);
		if (FAILED(hr)) {
			LOG_ERROR("DirectStorage Error : {}", ErrorCode(hr));
			return;
		}

		m_queue->EnqueueSignal(fence.Get(), fenceValue);

		impl->set(fence,handle);
	}

	void DirectX12GraphicFileQueue::wait() {
		if (auto event = GraphicFileEvent::Create()) {
			add(event);
			submit();
			event->wait();
		}
	}

	void DirectX12GraphicFileQueue::submit() {
		m_queue->Submit();
	}

	void DirectX12GraphicFileQueue::validate() {
		DSTORAGE_ERROR_RECORD errorRecord{};
		m_queue->RetrieveErrorRecord(&errorRecord);

		HRESULT hr = errorRecord.FirstFailure.HResult;

#define DS_ERROR(name) case name: message = #name;break

		if (FAILED(hr)) {
			String message;
			switch (errorRecord.FirstFailure.HResult) {
				DS_ERROR(E_DSTORAGE_ALREADY_RUNNING);
				DS_ERROR(E_DSTORAGE_NOT_RUNNING);
				DS_ERROR(E_DSTORAGE_INVALID_QUEUE_CAPACITY);
				DS_ERROR(E_DSTORAGE_XVD_DEVICE_NOT_SUPPORTED);
				DS_ERROR(E_DSTORAGE_UNSUPPORTED_VOLUME);
				DS_ERROR(E_DSTORAGE_END_OF_FILE);
				DS_ERROR(E_DSTORAGE_REQUEST_TOO_LARGE);
				DS_ERROR(E_DSTORAGE_ACCESS_VIOLATION);
				DS_ERROR(E_DSTORAGE_UNSUPPORTED_FILE);
				DS_ERROR(E_DSTORAGE_FILE_NOT_OPEN);
				DS_ERROR(E_DSTORAGE_RESERVED_FIELDS);
				DS_ERROR(E_DSTORAGE_INVALID_BCPACK_MODE);
				DS_ERROR(E_DSTORAGE_INVALID_SWIZZLE_MODE);
				DS_ERROR(E_DSTORAGE_INVALID_DESTINATION_SIZE);
				DS_ERROR(E_DSTORAGE_QUEUE_CLOSED);
				DS_ERROR(E_DSTORAGE_INVALID_CLUSTER_SIZE);
				DS_ERROR(E_DSTORAGE_TOO_MANY_QUEUES);
				DS_ERROR(E_DSTORAGE_INVALID_QUEUE_PRIORITY);
				DS_ERROR(E_DSTORAGE_TOO_MANY_FILES);
				DS_ERROR(E_DSTORAGE_INDEX_BOUND);
				DS_ERROR(E_DSTORAGE_IO_TIMEOUT);
				DS_ERROR(E_DSTORAGE_INVALID_FILE_HANDLE);
				DS_ERROR(E_DSTORAGE_DEPRECATED_PREVIEW_GDK);
				DS_ERROR(E_DSTORAGE_XVD_NOT_REGISTERED);
				DS_ERROR(E_DSTORAGE_INVALID_FILE_OFFSET);
				DS_ERROR(E_DSTORAGE_INVALID_SOURCE_TYPE);
				DS_ERROR(E_DSTORAGE_INVALID_INTERMEDIATE_SIZE);
				DS_ERROR(E_DSTORAGE_SYSTEM_NOT_SUPPORTED);
				DS_ERROR(E_DSTORAGE_STAGING_BUFFER_LOCKED);
				DS_ERROR(E_DSTORAGE_INVALID_STAGING_BUFFER_SIZE);
				DS_ERROR(E_DSTORAGE_STAGING_BUFFER_TOO_SMALL);
				DS_ERROR(E_DSTORAGE_INVALID_FENCE);
				DS_ERROR(E_DSTORAGE_INVALID_STATUS_ARRAY);
				DS_ERROR(E_DSTORAGE_INVALID_MEMORY_QUEUE_PRIORITY);
				DS_ERROR(E_DSTORAGE_DECOMPRESSION_ERROR);
				DS_ERROR(E_DSTORAGE_ZLIB_BAD_HEADER);
				DS_ERROR(E_DSTORAGE_ZLIB_BAD_DATA);
				DS_ERROR(E_DSTORAGE_ZLIB_PARITY_FAIL);
				DS_ERROR(E_DSTORAGE_BCPACK_BAD_HEADER);
				DS_ERROR(E_DSTORAGE_BCPACK_BAD_DATA);
				DS_ERROR(E_DSTORAGE_DECRYPTION_ERROR);
				DS_ERROR(E_DSTORAGE_PASSTHROUGH_ERROR);
				DS_ERROR(E_DSTORAGE_FILE_TOO_FRAGMENTED);
				DS_ERROR(E_DSTORAGE_COMPRESSED_DATA_TOO_LARGE);
				DS_ERROR(E_DSTORAGE_INVALID_DESTINATION_TYPE);
				DS_ERROR(E_DSTORAGE_FILEBUFFERING_REQUIRES_DISABLED_BYPASSIO);
			case E_INVALIDARG: message = "E_INVALIDARG Textureリクエストでこのエラーが発生する場合はID3D12Device::GetCopyableFootprints()に準拠したメモリレイアウト、サイズになっているか確認してください。"; break;
			default: message = ErrorCode(errorRecord.FirstFailure.HResult).message();
			}
#undef DS_ERROR
			switch (errorRecord.FirstFailure.CommandType) {
			case DSTORAGE_COMMAND_TYPE_REQUEST: message += Format(" ({})", errorRecord.FirstFailure.Request.RequestName); break;
			}
			LOG_ERROR("DirectStorage Error : {}", message);
		}
	}




	bool DirectX12GraphicFile::Generate(ID3D12Device8& device, StringView input, StringView output, s32 compressionLevel) {

		if (compressionLevel < 0) {
			LOG_WARNING("圧縮レベルが0未満です。");
			return false;
		}
		if (10 < compressionLevel) {
			LOG_WARNING("圧縮レベルが10より大きいです。");
			return false;
		}

		u32 compressionLevelMap[11] = {
			0,1,2,3,4,6,7,9,10,11,12
		};
		compressionLevel = compressionLevelMap[compressionLevel];
		bool useCompression = 0 < compressionLevel;

		if (auto optblob = File::ReadAllByte(input)) {

			auto& inputBlob = *optblob;

			// 拡張子に合わせて読み込み
			HRESULT result = 0;
			DirectX::TexMetadata metadata = {};
			DirectX::ScratchImage scratchImg = {};

			result = DirectX::LoadFromDDSMemory(inputBlob.data(), inputBlob.size(), DirectX::DDS_FLAGS_NONE, &metadata, scratchImg);
			if (FAILED(result)) result = DirectX::LoadFromWICMemory(inputBlob.data(), inputBlob.size(), DirectX::WIC_FLAGS_NONE, &metadata, scratchImg);
			if (FAILED(result)) result = DirectX::LoadFromTGAMemory(inputBlob.data(), inputBlob.size(), &metadata, scratchImg);
			if (FAILED(result)) result = DirectX::LoadFromHDRMemory(inputBlob.data(), inputBlob.size(), &metadata, scratchImg);
			if (FAILED(result)) false;

			// TODO Tex3D対応
			D3D12_RESOURCE_DESC desc{};

			if (metadata.dimension == DirectX::TEX_DIMENSION_TEXTURE1D) {
				desc = CD3DX12_RESOURCE_DESC::Tex1D(
					metadata.format,
					(UINT16)metadata.width,
					(UINT16)metadata.arraySize,
					(UINT16)metadata.mipLevels);
			} else if (metadata.dimension == DirectX::TEX_DIMENSION_TEXTURE2D) {
				desc = CD3DX12_RESOURCE_DESC::Tex2D(
					metadata.format,
					(UINT16)metadata.width,
					(UINT)metadata.height,
					(UINT16)metadata.arraySize,
					(UINT16)metadata.mipLevels);
			} else if(metadata.dimension == DirectX::TEX_DIMENSION_TEXTURE3D) {
				desc = CD3DX12_RESOURCE_DESC::Tex3D(
					metadata.format,
					(UINT16)metadata.width,
					(UINT)metadata.height,
					(UINT)metadata.depth,
					(UINT16)metadata.mipLevels);
				LOG_ERROR("GraphicFile::Generate() Texture3Dは非対応です");
				return false;
			} else {
				LOG_ERROR("不正なディメンション");
				return false;
			}

			Blob outputBlob;
			BinaryStream stream(outputBlob);
			BinaryWriter writer(stream);

			// ヘッダ書き込み
			writer.writeUInt32(metadata.format);
			writer.writeUInt32(metadata.width);
			writer.writeUInt32(metadata.height);
			writer.writeUInt32(metadata.arraySize);
			writer.writeUInt32(metadata.mipLevels);


			// リソースレイアウト取得
			std::vector<D3D12_SUBRESOURCE_DATA> subresources;
			result = DirectX::PrepareUpload(&device, scratchImg.GetImages(), scratchImg.GetImageCount(), metadata, subresources);
			if (FAILED(result)) false;

			Vector<D3D12_PLACED_SUBRESOURCE_FOOTPRINT> layouts(subresources.size());
			Vector<UINT> numRows(subresources.size());
			Vector<UINT64> rowSizes(subresources.size());
			UINT64 totalBytes = 0;
			UINT firstSubresource = 0;

			device.GetCopyableFootprints(
				&desc,
				firstSubresource,
				subresources.size(),
				0,
				layouts.data(),
				numRows.data(),
				rowSizes.data(),
				&totalBytes);

			// レイアウト書き込み用の番兵を追加
			auto& lastLayout = layouts.emplace_back();
			lastLayout.Offset = totalBytes;

			// 後で書き込む用のダミーデータ書き込み
			auto offsetInfoPos = stream.position();
			for (s32 i = 0; i < subresources.size(); ++i) {
				writer.writeUInt32(0);
				writer.writeUInt32(0);
				writer.writeUInt32(0);
				writer.writeUInt32(0);
				writer.writeUInt32(0);
			}

			// GPU都合に合わせてデータを並べ替える
			Vector<u32> offsets;
			for (s32 i = 0; i < subresources.size(); ++i) {

				// 16バイトアラインメントにそろえる(Xbox用)
				auto padding = align_up(stream.position(), 16) - stream.position();
				for (s32 i = 0; i < padding; ++i)writer.writeUInt8(0);

				// 再配置
				size_t size = layouts[i+1].Offset - layouts[i].Offset;
				Blob blob(size);
				Blob compressed(std::max<size_t>(size*2,1024));

				auto const& layout = layouts[i];
				auto const& subresource = subresources[i];

				D3D12_MEMCPY_DEST memcpyDest{};
				memcpyDest.pData = blob.data();
				memcpyDest.RowPitch = layout.Footprint.RowPitch;
				memcpyDest.SlicePitch = layout.Footprint.RowPitch * numRows[i];

				MemcpySubresource(
					&memcpyDest,
					&subresource,
					static_cast<SIZE_T>(rowSizes[i]),
					numRows[i],
					layout.Footprint.Depth);

				offsets.push_back(stream.position());
				if (useCompression) {
					size_t compressedSize = compressed.size();
					GDeflate::Compress(compressed.data(), &compressedSize,blob.data(),blob.size(), compressionLevel,0);
					compressed.resize(compressedSize);
					writer.write(compressed.data(), compressed.size());
				} else {
					writer.write(blob.data(), blob.size());
				}

			}
			offsets.push_back(stream.position());

			// ミップ情報書き込み
			writer.seek(offsetInfoPos);
			for (s32 i = 0; i < subresources.size(); ++i) {
				// offset
				writer.writeUInt32(offsets[i]);
				if (useCompression) {
					// size
					writer.writeUInt32(offsets[i + 1] - offsets[i]);
					// uncompressed
					writer.writeUInt32(layouts[i + 1].Offset - layouts[i].Offset);
				} else {
					// size
					writer.writeUInt32(offsets[i + 1] - offsets[i]);
					// uncompressed
					writer.writeUInt32(0);
				}
				writer.writeUInt32(layouts[i].Footprint.Width);
				writer.writeUInt32(layouts[i].Footprint.Height);
			}

			// ファイル出力
			File file(output, FileOpenMode::Write);
			if (!file)return false;

			file.write(outputBlob.data(), outputBlob.size());
			
			return true;

		}

		return false;

	}

	Vector<GraphicFileMipInfo> DirectX12GraphicFile::Prepare(StringView path) {

		Vector<GraphicFileMipInfo> result;

		File file(path);
		BinaryReader reader(file);

		u32 format = reader.readU32();
		u32 width = reader.readU32();
		u32 height = reader.readU32();
		u32 arraySize = reader.readU32();
		u32 mipLevels = reader.readU32();

		for (s32 item = 0; item < arraySize; ++item) {
			for (s32 mip = 0; mip < mipLevels; ++mip) {
				auto& info = result.emplace_back();
				info.offset = reader.readU32();
				info.size = reader.readU32();
				info.uncompressedSize = reader.readU32();
				info.width = reader.readU32();
				info.height = reader.readU32();
			}
		}

		return result;
	}
}