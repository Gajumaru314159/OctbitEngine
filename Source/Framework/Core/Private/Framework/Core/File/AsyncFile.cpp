//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/AsyncFile.h>
#include <Framework/Core/File/File.h>
#include <Framework/Core/String/StringEncoder.h>
#include <Framework/Core/Log/Logger.h>

#include <chrono>
#include <future>
#include <memory>
#include <vector>

#if defined(OS_WINDOWS)
#include <Framework/Core/Platform/WindowsHeaders.h>
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

	bool AsyncFileSystem::CanUseDirectStorage() {
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

		if (AsyncFileSystem::CanUseDirectStorage()) {


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





	struct AsyncFile::Impl {
		Blob blob;
		AsyncFileEvent event;
		AsyncFileHandle handle;
	};


	//! @brief AsyncFileDesc から AsyncFile を生成
	AsyncFile::AsyncFile(AsyncFileQueue& queue, StringView path) {

		m_impl.construct();

		AsyncFileHandle handle(path);
		auto size = handle.size();

		m_impl->handle = handle;
		m_impl->blob.resize(size);

		AsyncFileRequest request;
		request.handle = handle;
		request.size = size;
		request.dest = m_impl->blob.data();

		queue.add(request);

		queue.add(m_impl->event);

	}

	//! @brief デストラクタ
	AsyncFile::~AsyncFile() = default;

	//! @brief 非同期読み込みが完了しているか
	bool AsyncFile::poll()const {
		return m_impl->event.poll();
	}

	//! @brief 非同期読み込みの完了を待つ
	void AsyncFile::wait()const {
		return m_impl->event.wait();
	}

	//! @brief 読み込み済みデータにアクセスする
	//! @details 読み込みが完了していない場合はnullptrを返します。
	const void* AsyncFile::data()const {
		return m_impl->blob.data();
	}

	//! @brief 読み込み済みデータのサイズを取得する
	//! @details 読み込みが完了していない場合は0を返します。
	size_t AsyncFile::size()const {
		return m_impl->blob.size();
	}

}

#elif defined(OS_LINUX)

namespace ob::core {

	namespace {
		bool ProcessRequest(const AsyncFileRequest& request) {
			if (request.handle.empty()) {
				return false;
			}
			if (request.uncompressedSize != 0) {
				LOG_WARNING("AsyncFile", "圧縮データの展開はこのプラットフォームではサポートされていません。");
				return false;
			}
			if (request.dest == nullptr) {
				return false;
			}
			File file(request.handle.path(), FileOpenMode::Read);
			if (!file.canRead()) {
				return false;
			}
			if (!file.seek(static_cast<offset_t>(request.offset), SeekOrigin::Begin)) {
				return false;
			}
			return file.read(request.dest, request.size);
		}
	}

	AsyncFileSystem::AsyncFileSystem() = default;
	AsyncFileSystem::~AsyncFileSystem() = default;
	bool AsyncFileSystem::CanUseDirectStorage() { return false; }

	struct AsyncFileEvent::Impl {
		std::shared_ptr<std::promise<void>> promise;
		std::shared_future<void> future;
	};

	AsyncFileEvent::AsyncFileEvent() {
		m_impl.construct();
	}

	AsyncFileEvent::~AsyncFileEvent() = default;

	bool AsyncFileEvent::poll() const {
		if (!m_impl->future.valid()) {
			return false;
		}
		return m_impl->future.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready;
	}

	void AsyncFileEvent::wait() const {
		if (m_impl->future.valid()) {
			m_impl->future.wait();
		}
	}

	struct AsyncFileHandle::Impl {
		String path;
		size_t size = 0;
		bool valid = false;
	};

	AsyncFileHandle::AsyncFileHandle(StringView path) {
		m_impl = std::make_shared<Impl>();
		m_impl->path = path;
		if (File::Exists(path)) {
			m_impl->valid = true;
			m_impl->size = File::Size(path);
		}
	}

	AsyncFileHandle::~AsyncFileHandle() = default;

	const String& AsyncFileHandle::path()const {
		static String empty;
		return m_impl ? m_impl->path : empty;
	}

	bool AsyncFileHandle::empty()const {
		return !m_impl || !m_impl->valid;
	}

	size_t AsyncFileHandle::size()const {
		return (m_impl && m_impl->valid) ? m_impl->size : 0;
	}

	struct AsyncFileQueue::Impl {
		std::vector<AsyncFileRequest> requests;
		std::vector<std::shared_ptr<std::promise<void>>> events;
	};

	AsyncFileQueue::AsyncFileQueue() {
		m_impl.construct();
	}

	AsyncFileQueue::~AsyncFileQueue() = default;

	void AsyncFileQueue::add(const AsyncFileRequest& desc) {
		m_impl->requests.push_back(desc);
	}

	void AsyncFileQueue::add(AsyncFileEvent& event) {
		auto promise = std::make_shared<std::promise<void>>();
		event.m_impl->promise = promise;
		event.m_impl->future = promise->get_future().share();
		m_impl->events.push_back(std::move(promise));
	}

	void AsyncFileQueue::wait() {
		AsyncFileEvent event;
		add(event);
		submit();
		event.wait();
	}

	void AsyncFileQueue::submit() {
		for (auto& request : m_impl->requests) {
			if (!ProcessRequest(request)) {
				LOG_WARNING("AsyncFile", "非同期読み込みに失敗しました。[{}]", request.handle.path());
			}
		}
		for (auto& promise : m_impl->events) {
			if (promise) {
				promise->set_value();
			}
		}
		m_impl->requests.clear();
		m_impl->events.clear();
	}

	struct AsyncFile::Impl {
		Blob blob;
		AsyncFileEvent event;
		AsyncFileHandle handle;
	};

	AsyncFile::AsyncFile(AsyncFileQueue& queue, StringView path) {

		m_impl.construct();

		AsyncFileHandle handle(path);
		auto size = handle.size();

		m_impl->handle = handle;
		m_impl->blob.resize(size);

		AsyncFileRequest request;
		request.handle = handle;
		request.size = size;
		request.dest = m_impl->blob.data();

		queue.add(request);

		queue.add(m_impl->event);
	}

	AsyncFile::~AsyncFile() = default;

	bool AsyncFile::poll()const {
		return m_impl->event.poll();
	}

	void AsyncFile::wait()const {
		m_impl->event.wait();
	}

	const void* AsyncFile::data()const {
		return m_impl->blob.data();
	}

	size_t AsyncFile::size()const {
		return m_impl->blob.size();
	}

}

#endif
