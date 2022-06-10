//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/File/Stream.h>
#include <Framework/Core/File/Path.h>

namespace ob::core {

	enum class FileAttribut:u32
	{
		None		= get_bit(0),	//!< なし
		Normal		= get_bit(1),	//!< 通常ファイル
		Directory	= get_bit(2),	//!< ディレクトリ
		ReadOnly	= get_bit(3),	//!< 読み取り専用
		Hidden		= get_bit(4),	//!< 隠しファイル
	};

	enum class FileOpenMode:u32
	{
		None		= get_bit(0),	//!< なし
		Read		= get_bit(1),	//!< 読み取り
		Write		= get_bit(2),	//!< 書き込み
		ReadWrite	= Read | Write,	//!< 読み書き
		Append		= get_bit(3),	//!< 追記
		Truncate	= get_bit(4),	//!< ファイルサイズを0にする
	};
	class FileStreamImpl;

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class FileStream :public Stream {
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  説明
		//@―---------------------------------------------------------------------------
		FileStream(const Path& path, FileOpenMode mode = FileOpenMode::Read);
		~FileStream();



		bool canRead()const override;
		bool canWrite()const override;
		size_t size()const override;
		size_t position()const override;
		size_t read(void* buffer, size_t byteCount) override;
		size_t write(void* buffer, size_t byteCount) override;
		size_t seek(offset_t offset, SeekOrigin origin) override;
		void flush() override;


	private:

		std::unique_ptr<FileStreamImpl> m_impl;

	};






	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond



	//! @endcond
}// namespcae ob