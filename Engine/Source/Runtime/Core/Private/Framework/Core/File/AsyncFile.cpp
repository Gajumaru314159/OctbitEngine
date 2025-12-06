//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/AsyncFile.h>

namespace ob::core {

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