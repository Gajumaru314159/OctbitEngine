//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/File/Stream.h>
#include <Framework/Core/Misc/Blob.h>

namespace ob::core {

	//! @brief  ファイル
	class File : public Stream {
	public:

		//! @brief  ファイルが存在するか
		static bool Exists(StringView path);

		//! @brief  ファイルサイズを取得
		static size_t Size(StringView path);
		
		//! @brief  ファイルをコピーする
		//! @param src		コピー元
		//! @param dst		コピー先
		//! @param options	オプション
		//! @return 成功したか
		static bool Copy(StringView src, StringView dst, FileCopyOptions options = FileCopyOption::None);
		
		//! @brief	ファイルを削除する
		static bool Delete(StringView path);
		
		//! @brief  ファイルを移動する
		static bool Move(StringView from, StringView to);
		
		//! @brief  ファイル名を変更する
		static bool Rename(StringView from,StringView to);

		//! @brief  ファイルを文字列として全て読み込む
		static Optional<String> ReadAllText(StringView path);

		//! @brief  ファイルをバイナリデータとして全て読み込む
		static Optional<Blob> ReadAllByte(StringView path);

	public:

		//! @brief  デフォルトコンストラクタ
		File();

		//! @brief  ファイルを開く
		//! 
		//! @param path ファイルパス
		//! @param mode モード
		File(StringView path, FileOpenMode mode = FileOpenMode::Read);

		//! @brief  デストラクタ
		~File() override;

		//! @brief  有効な状態か
		operator bool()const;

		//! @brief  読み込み可能か
		bool canRead()const override;

		//! @brief  書き込み可能か
		bool canWrite()const override;

		//! @brief  サイズ取得
		size_t size()const override;

		//! @brief  シーク位置取得
		size_t position()const override;

		//! @brief  読み込み
		//! 
		//! @param buffer 　 読み込みデータ格納先のポインタ
		//! @param byteCount 読み込むバイト数
		//! @return 成功したか
		bool read(void* buffer, size_t byteCount) override;

		//! @brief  書き込み
		//! 
		//! @param buffer 　 書き込むデータ格納先のポインタ
		//! @param byteCount 書き込むバイト数
		//! @return 成功したか
		bool write(const void* buffer, size_t byteCount) override;

		//! @brief  シーク
		//! 
		//! @param offset 　 基準からの移動量
		//! @param origin	 基準位置
		//! @return 成功したか
		bool seek(offset_t offset, SeekOrigin origin) override;

		//! @brief  バッファのフラッシュ
		//! 
		//! @details 書き込み中のデータを即時反映させる。
		void flush() override;

		//! @brief  ファイルをクローズして無効なハンドルにする
		void close();

	private:
		UPtr<class FileImpl> m_impl;
	};

}