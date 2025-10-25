//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/AsyncFile.h>
#include <Framework/Core/File/File.h>

#ifdef OS_LINUX

namespace ob::core {

	AsyncFileSystem::AsyncFileSystem() {
	}

	AsyncFileSystem::~AsyncFileSystem() {
	}

	bool AsyncFileSystem::CanUseAsyncFile() {
		return false;
	}



	//! @brief プラットフォームデータ
	struct AsyncFileEvent::Impl {
		SPtr<bool> isLoaded;
	};

	AsyncFileEvent::AsyncFileEvent() {
		m_impl.construct();
		m_impl->isLoaded = std::make_shared<bool>(false);
	}
	AsyncFileEvent::~AsyncFileEvent() = default;
	bool AsyncFileEvent::poll() const {
		return true;
	}
	void AsyncFileEvent::wait() const {
	}



	struct AsyncFileHandle::Impl {
		String path;
		Blob blob;
		bool loaded = false;
	};

	//! @brief コンストラクタ
	AsyncFileHandle::AsyncFileHandle(StringView path) {
		m_impl = std::make_shared<Impl>();
		m_impl->path = path;
	}

	//! @brief デストラクタ
	AsyncFileHandle::~AsyncFileHandle() = default;

	const String& AsyncFileHandle::path()const {
		return m_impl->path;
	}

	bool AsyncFileHandle::empty()const {
		return !File::Exists(m_impl->path);
	}

	size_t AsyncFileHandle::size()const {
		return File::Size(m_impl->path);
	}



	//! @brief プラットフォームデータ
	struct AsyncFileQueue::Impl {
		Vector<AsyncFileRequest> requests;
		Vector<SPtr<bool>> events;
	};

	//! @brief コンストラクタ
	AsyncFileQueue::AsyncFileQueue() {
		m_impl.construct();
	}

	//! @brief デストラクタ
	AsyncFileQueue::~AsyncFileQueue() = default;

	//! @brief 非同期読み込みリクエストを追加
	void AsyncFileQueue::add(const AsyncFileRequest& request) {
		m_impl->requests.push_back(request);
	}

	//! @brief 非同期読み込み完了同期イベントを追加
	void AsyncFileQueue::add(AsyncFileEvent& event) {
		m_impl->events.push_back(event.m_impl->isLoaded);
	}

	//! @brief 全ての非同期読み込みの完了を待つ
	void AsyncFileQueue::wait() {
		// 何もしない
	}

	//! @brief 非同期読み込みを開始
	void AsyncFileQueue::submit() {
		for (auto& request : m_impl->requests)
		{
			if (File file = File(request.handle.path())) {
				file.seek(request.offset, SeekOrigin::Begin);
				if (!file.read(request.dest, request.size)) {
					LOG_WARNING("ファイルの非同期読み込みに失敗しました {}",request.handle.path());
				}
			}
		}
		for (auto& event : m_impl->events) {
			*event = true;
		}
		m_impl->requests.clear();
		m_impl->events.clear();
	}

}

#endif