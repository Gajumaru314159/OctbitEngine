//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Plugins/DirectX12RHI/GraphicFile/GraphicFileImpl.h>
#include <Plugins/DirectX12RHI/Buffer/BufferImpl.h>
#include <Plugins/DirectX12RHI/Texture/TextureImpl.h>
#include <Framework/Core/String/StringEncoder.h>
#include <Framework/Core/Misc/ErrorCode.h>

namespace ob::rhi::dx12 {



	//! @brief コンストラクタ
	GraphicFileHandleImpl::GraphicFileHandleImpl(IDStorageFactory& factory,StringView path) {

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

	bool GraphicFileHandleImpl::isValid()const {
		return m_file;
	}

	const String& GraphicFileHandleImpl::path()const {
		return m_path;
	}

	bool GraphicFileHandleImpl::empty()const {
		return !m_file;
	}

	size_t GraphicFileHandleImpl::size()const {
		BY_HANDLE_FILE_INFORMATION info;
		auto hr = m_file->GetFileInformation(&info);
		if (FAILED(hr))return 0;
		return ((size_t)info.nFileSizeHigh) << 32 | info.nFileSizeLow;
	}

	const ComPtr<IDStorageFile>& GraphicFileHandleImpl::file()const {
		return m_file;
	}


	
	GraphicFileEventImpl::GraphicFileEventImpl() {
	}
	
	GraphicFileEventImpl::~GraphicFileEventImpl() {

		if (m_handle)
		{
			CloseHandle(m_handle);
		}
	}

	bool GraphicFileEventImpl::isValid()const {
		return true;
	}

	void GraphicFileEventImpl::set(ComPtr<ID3D12Fence> fence,HANDLE handle) {
		m_handle = handle;
		m_fence = fence;
	}
	bool GraphicFileEventImpl::poll() const {
		return WaitForSingleObject(m_handle, 0) == WAIT_OBJECT_0;
	}
	void GraphicFileEventImpl::wait() const {
		WaitForSingleObject(m_handle, INFINITE);
	}




	//! @brief コンストラクタ
	GraphicFileQueueImpl::GraphicFileQueueImpl(ID3D12Device8& device,IDStorageFactory& factory, const GraphicFileQueueDesc& desc)
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

	bool GraphicFileQueueImpl::isValid()const {
		return m_queue;
	}

	//! @brief 非同期読み込みリクエストを追加
	void GraphicFileQueueImpl::add(const GraphicFileRequest& desc) {

		// ファイルハンドルが無効な場合は何もしない
		auto handle = desc.handle.cast<GraphicFileHandleImpl>();
		if (handle == nullptr || handle->file() == nullptr) return;

		bool useDecompression = 0 < desc.uncompressedSize;

		DSTORAGE_REQUEST request{};
		request.Options.CompressionFormat = useDecompression ? DSTORAGE_COMPRESSION_FORMAT_GDEFLATE : DSTORAGE_COMPRESSION_FORMAT_NONE;
		request.Options.SourceType = DSTORAGE_REQUEST_SOURCE_FILE;
		request.Source.File.Source = handle->file().Get();
		request.Source.File.Offset = desc.offset;
		request.Source.File.Size = desc.size;
		request.Name = handle->path().c_str();

		if (std::holds_alternative<GraphicFileRequest::BufferRequest>(desc.dest)) {
			auto& bufferDesc = std::get<GraphicFileRequest::BufferRequest>(desc.dest);
			auto buffer = bufferDesc.buffer.cast<BufferImpl>();
			if (buffer == nullptr) return;
			request.Options.DestinationType = DSTORAGE_REQUEST_DESTINATION_BUFFER;
			request.Destination.Buffer.Resource = buffer->getNative();
			request.Destination.Buffer.Offset = bufferDesc.offset;
			request.Destination.Buffer.Size = useDecompression ? desc.uncompressedSize : desc.size;
			request.UncompressedSize = useDecompression ? desc.uncompressedSize : 0;
		}

		if (std::holds_alternative<GraphicFileRequest::TextureRequest>(desc.dest)) {
			auto& textureDesc = std::get<GraphicFileRequest::TextureRequest>(desc.dest);
			auto texture = textureDesc.texture.cast<TextureImpl>();
			if (texture == nullptr) return;
			request.Options.DestinationType = DSTORAGE_REQUEST_DESTINATION_TEXTURE_REGION;
			request.Destination.Texture.Resource = texture->getResource();
			request.Destination.Texture.SubresourceIndex = textureDesc.subresourceIndex;
			request.Destination.Texture.Region.left = textureDesc.left;
			request.Destination.Texture.Region.top = textureDesc.top;
			request.Destination.Texture.Region.right = textureDesc.right;
			request.Destination.Texture.Region.bottom = textureDesc.bottom;
			request.Destination.Texture.Region.front = textureDesc.front;
			request.Destination.Texture.Region.back = textureDesc.back;
			request.UncompressedSize = useDecompression ? desc.uncompressedSize : 0;
		}

		m_queue->EnqueueRequest(&request);

	}

	//! @brief 非同期読み込み完了同期イベントを追加 
	void GraphicFileQueueImpl::add(Ref<GraphicFileEvent>& event) {
		auto impl = event.cast<GraphicFileEventImpl>();
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

	void GraphicFileQueueImpl::wait() {
		if (auto event = GraphicFileEvent::Create()) {
			add(event);
			submit();
			event->wait();
		}
	}

	void GraphicFileQueueImpl::submit() {
		m_queue->Submit();
	}

	void GraphicFileQueueImpl::validate() {
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
			default: message = ErrorCode(errorRecord.FirstFailure.HResult).message();
			}
#undef DS_ERROR
			switch (errorRecord.FirstFailure.CommandType) {
			case DSTORAGE_COMMAND_TYPE_REQUEST: message += Format(" ({})", errorRecord.FirstFailure.Request.RequestName); break;
			}
			LOG_ERROR("DirectStorage Error : {}", message);
		}
	}

}