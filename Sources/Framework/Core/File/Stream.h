//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>

namespace ob::core {

	enum class SeekOrigin {
		Begin,
		Current,
		End,
	};

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class Stream {
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  説明
		//@―---------------------------------------------------------------------------

		virtual bool canRead()const = 0;
		virtual bool canWrite()const = 0;
		virtual size_t size()const = 0;
		virtual size_t position()const = 0;
		virtual size_t read(void* buffer, size_t byteCount) = 0;
		virtual size_t write(void* buffer, size_t byteCount) = 0;
		virtual size_t seek(offset_t offset, SeekOrigin origin) = 0;
		virtual void flush() = 0;

	protected:
		virtual ~Stream()=default;

	};






	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond



	//! @endcond
}// namespcae ob