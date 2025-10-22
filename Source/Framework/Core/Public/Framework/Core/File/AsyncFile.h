//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/String/StringView.h>
#include <Framework/Core/Misc/BlobView.h>
#include <Framework/Core/Utility/MemoryStorage.h>
#include <Framework/Core/Utility/Singleton.h>

namespace ob::core {

	class AsyncFileSystem : public Singleton<AsyncFileSystem> {
	public:
		AsyncFileSystem();
		~AsyncFileSystem() override;
	public:
		static bool CanUseAsyncFile();
	};

	class AsyncFileHandle {
	public:
		AsyncFileHandle() = default;
		explicit AsyncFileHandle(StringView path);
		~AsyncFileHandle();
		const String& path()const;
		bool empty()const;
		size_t size()const;
	private:
		friend class AsyncFileQueue;
		struct Impl;
		SPtr<Impl> m_impl;
	};

	//! @brief		非同期ファイル定義
	//! @details	* uncompressedSizeが0以外の場合はプラットフォームに応じた解凍処理が行われます。
	//!				* destが指定されている場合は指定先にファイルの内容が展開されます。
	//!				* destが指定されていない場合は内部的にバッファを確保し、そのバッファに展開されます。
	struct AsyncFileRequest {
		AsyncFileHandle handle;					//! ファイルハンドル
		size_t			offset = 0;				//! 読み込み開始位置	
		size_t			size = 0;				//! 読み込みサイズ	
		size_t			uncompressedSize = 0;	//! 解凍後のサイズ
		void*			dest = nullptr;			//! 読み込み先
	};

	//! @brief 非同期ファイル読み込みイベント
	class AsyncFileEvent {
	public:
		AsyncFileEvent();
		~AsyncFileEvent();
		bool poll() const;
		void wait() const;
	private:
		friend class AsyncFileQueue;
		struct Impl;
		MemoryStorageBase<Impl, 32, 8> m_impl;
	};

	//! @brief 非同期ファイル読み込みキュー
	//! @detail このクラスは非同期ファイル読み込みを管理するキューです。
	class AsyncFileQueue {
	public:
		AsyncFileQueue();
		~AsyncFileQueue();
		void add(const AsyncFileRequest& desc);
		void add(AsyncFileEvent&);
		void wait();
		void submit();
	private:
		struct Impl;
		MemoryStorageBase<Impl, 128, 8> m_impl;
	};


	//! @brief  非同期ファイル
	//! @details このクラスはAsyncFileQueue、AsyncFileHandle、AsyncFileRequestのラッパークラスです。
	class AsyncFile {
	public:
		//! @brief コンストラクタ
		AsyncFile(AsyncFileQueue& queue,StringView path);

		//! @brief デストラクタ
		~AsyncFile();

		//! @brief 非同期読み込みが完了しているか
		bool poll()const;

		//! @brief 非同期読み込みの完了を待つ
		void wait()const;

		//! @brief 読み込み済みデータにアクセスする
		//! @details 読み込みが完了していない場合はnullptrを返します。
		const void* data()const;

		//! @brief 読み込み済みデータのサイズを取得する
		//! @details 読み込みが完了していない場合は0を返します。
		size_t size()const;
	private:
		struct Impl;
		MemoryStorageBase<Impl, 128,8> m_impl;
	};

}