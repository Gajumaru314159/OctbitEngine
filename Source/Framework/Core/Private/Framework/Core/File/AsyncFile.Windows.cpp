//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/AsyncFile.h>
#include <Framework/Core/File/File.h>
#include <Framework/Core/String/StringEncoder.h>
#include <Framework/Core/Log/Logger.h>

#ifdef OS_WINDOWS
#include <Framework/Core/Platform/WindowsHeaders.h>
#include <chrono>
#include <future>
#include <memory>
#include <vector>
#include <dstorage.h>

namespace ob::core {

	static ComPtr<IDStorageFactory> g_factory;

	AsyncFileSystem::AsyncFileSystem() {
		OB_ASSERT_EXPR(g_factory==nullptr);
		if (FAILED(DStorageGetFactory(IID_PPV_ARGS(g_factory.ReleaseAndGetAddressOf())))) {
			return;
		}
	}

	AsyncFileSystem::~AsyncFileSystem() {
		g_factory.Reset();
	}

	bool AsyncFileSystem::CanUseAsyncFile() {
		return g_factory;
	}



	//! @brief プラットフォームデータ
	struct AsyncFileEvent::Impl {
		HANDLE handle = nullptr;
	};

	AsyncFileEvent::AsyncFileEvent() {
		m_impl.construct();
	}
	AsyncFileEvent::~AsyncFileEvent() = default;
	bool AsyncFileEvent::poll() const {
		return WaitForSingleObject(m_impl->handle, 0) == WAIT_OBJECT_0;
	}
	void AsyncFileEvent::wait() const {
		WaitForSingleObject(m_impl->handle, INFINITE);
	}



	struct AsyncFileHandle::Impl {
		String path;
		ComPtr<IDStorageFile> file;
	};

	//! @brief コンストラクタ
	AsyncFileHandle::AsyncFileHandle(StringView path) {

		m_impl = std::make_shared<Impl>();
		m_impl->path = path;

		WString wpath;
		StringEncoder::Encode(path, wpath);

		ComPtr<IDStorageFile> file;
		auto hr = g_factory->OpenFile(wpath.c_str(), IID_PPV_ARGS(file.ReleaseAndGetAddressOf()));

		if (FAILED(hr)) {
			return;
		}

		m_impl->file = file;
	}

	//! @brief デストラクタ
	AsyncFileHandle::~AsyncFileHandle() = default;

	const String& AsyncFileHandle::path()const {
		return m_impl->path;
	}

	bool AsyncFileHandle::empty()const {
		return !m_impl->file;
	}

	size_t AsyncFileHandle::size()const {
		BY_HANDLE_FILE_INFORMATION info;
		auto hr = m_impl->file->GetFileInformation(&info);
		if (FAILED(hr))return 0;
		return static_cast<size_t>(info.nFileSizeHigh) << 32 | info.nFileSizeLow;
	}



	//! @brief プラットフォームデータ
	struct AsyncFileQueue::Impl {
		ComPtr<IDStorageQueue1> queue;
	};

	//! @brief コンストラクタ
	AsyncFileQueue::AsyncFileQueue() {

		m_impl.construct();

		DSTORAGE_QUEUE_DESC desc{};
		desc.Capacity = DSTORAGE_MAX_QUEUE_CAPACITY;
		desc.Priority = DSTORAGE_PRIORITY_NORMAL;
		desc.SourceType = DSTORAGE_REQUEST_SOURCE_FILE;

		ComPtr<IDStorageQueue1> queue;
		auto hr = g_factory->CreateQueue(&desc, IID_PPV_ARGS(queue.ReleaseAndGetAddressOf()));
		if (FAILED(hr))return;

		m_impl->queue = queue;
	}

	//! @brief デストラクタ
	AsyncFileQueue::~AsyncFileQueue() = default;

	//! @brief 非同期読み込みリクエストを追加
	void AsyncFileQueue::add(const AsyncFileRequest& desc) {

		// ファイルハンドルが無効な場合は何もしない
		if (desc.handle.m_impl == nullptr || desc.handle.m_impl->file == nullptr) return;

		bool useDecompression = 0 < desc.uncompressedSize;

		if (AsyncFileSystem::CanUseAsyncFile()) {


			DSTORAGE_REQUEST request{};
			request.Options.CompressionFormat = useDecompression ? DSTORAGE_COMPRESSION_FORMAT_GDEFLATE : DSTORAGE_COMPRESSION_FORMAT_NONE;
			request.Options.SourceType = DSTORAGE_REQUEST_SOURCE_FILE;
			request.Options.DestinationType = DSTORAGE_REQUEST_DESTINATION_MEMORY;
			request.Source.File.Source = desc.handle.m_impl->file.Get();
			request.Source.File.Offset = desc.offset;
			request.Source.File.Size = gsl::narrow<UINT32>(desc.size);
			request.Destination.Memory.Buffer = desc.dest;
			request.Destination.Memory.Size = gsl::narrow<UINT32>(useDecompression ? desc.uncompressedSize : desc.size);
			request.Name = desc.handle.path().c_str();

			m_impl->queue->EnqueueRequest(&request);

		}
		else {

			File file(desc.handle.path(), FileOpenMode::Read);
			if (useDecompression) {

				constexpr size_t STACK_ALLOCATABLE_SIZE = 1 * 1024;
				u8 stackBuffer[STACK_ALLOCATABLE_SIZE];

				bool canUseStackAlloc = desc.size < STACK_ALLOCATABLE_SIZE;

				u8* compressed = canUseStackAlloc ? stackBuffer : new u8[desc.size];

				file.read(compressed, desc.size);

				if (canUseStackAlloc) delete[] compressed;

				// TODO 解凍
				OB_ABORT("展開処理が未実装です");
				// Decompress::GDeflate(compressed, desc.size, desc.dest, desc.uncompressedSize);

			}
			else {
				file.read(desc.dest, desc.size);
			}

		}

	}

	//! @brief 非同期読み込み完了同期イベントを追加
	void AsyncFileQueue::add(AsyncFileEvent& event) {
		HANDLE handle = ::CreateEventA(nullptr, true, false, nullptr);
		m_impl->queue->EnqueueSetEvent(handle);
		event.m_impl->handle = handle;
	}

	void AsyncFileQueue::wait() {
		AsyncFileEvent event;
		add(event);
		submit();
		event.wait();
	}

	void AsyncFileQueue::submit() {
		m_impl->queue->Submit();
	}

}

#endif
