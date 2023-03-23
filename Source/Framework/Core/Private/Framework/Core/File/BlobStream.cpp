//***********************************************************
//! @file
//! @brief		Blobストリーム
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/BlobStream.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	BlobStream::BlobStream(Blob& blob)
		: m_blob(blob)
		, m_position(0)
	{
	}


	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	BlobStream::~BlobStream() = default;


	//@―---------------------------------------------------------------------------
	//! @brief  読み込み可能か
	//@―---------------------------------------------------------------------------
	bool BlobStream::canRead()const {
		return true;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  書き込み可能か
	//@―---------------------------------------------------------------------------
	bool BlobStream::canWrite()const {
		return true;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  サイズ取得
	//@―---------------------------------------------------------------------------
	size_t BlobStream::size()const {
		return m_blob.size();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  読み込み位置取得
	//@―---------------------------------------------------------------------------
	size_t BlobStream::position()const {
		return m_position;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  読み込み
	//@―---------------------------------------------------------------------------
	bool BlobStream::read(void* buffer, size_t byteCount) {
		if (size() < m_position + byteCount)return false;
		std::memcpy(buffer, m_blob.data() + m_position, byteCount);
		return byteCount;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  書き込み
	//@―---------------------------------------------------------------------------
	bool BlobStream::write(const void* buffer, size_t byteCount) {
		auto needSize = m_position + byteCount;
		m_blob.resize(needSize);
		std::memcpy(m_blob.data() + m_position, buffer, byteCount);
		return true;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  シーク
	//@―---------------------------------------------------------------------------
	bool BlobStream::seek(offset_t offset, SeekOrigin origin) {
		offset_t newPos = offset;
		if (origin == SeekOrigin::Begin) {
		}
		if (origin == SeekOrigin::Current) {
			newPos += m_position;
		}
		if (origin == SeekOrigin::End) {
			newPos += m_blob.size();
		}
		if (is_in_range<offset_t>(newPos, 0, m_blob.size())) {
			m_position = newPos;
			return true;
		}
		return false;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  フラッシュ
	//@―---------------------------------------------------------------------------
	void BlobStream::flush() { 
	}

}// namespace ob