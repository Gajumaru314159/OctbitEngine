//***********************************************************
//! @file
//! @brief		Bynaryストリーム
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/BinaryStream.h>

namespace ob::core {

	//! @brief  コンストラクタ
	BinaryStream::BinaryStream(Blob& blob)
		: m_blob(blob)
		, m_position(0)
	{
	}


	//! @brief  デストラクタ
	BinaryStream::~BinaryStream() = default;


	//! @brief  読み込み可能か
	bool BinaryStream::canRead()const {
		return true;
	}


	//! @brief  書き込み可能か
	bool BinaryStream::canWrite()const {
		return true;
	}


	//! @brief  サイズ取得
	size_t BinaryStream::size()const {
		return m_blob.size();
	}


	//! @brief  読み込み位置取得
	size_t BinaryStream::position()const {
		return m_position;
	}


	//! @brief  読み込み
	bool BinaryStream::read(void* buffer, size_t byteCount) {
		if (size() < m_position + byteCount)return false;
		std::memcpy(buffer, m_blob.data() + m_position, byteCount);
		m_position += byteCount;
		return byteCount;
	}


	//! @brief  書き込み
	bool BinaryStream::write(const void* buffer, size_t byteCount) {
		auto needSize = m_position + byteCount;
		if(m_blob.size()<needSize)m_blob.resize(needSize);
		std::memcpy(m_blob.data() + m_position, buffer, byteCount);
		m_position += byteCount;
		return true;
	}


	//! @brief  シーク
	bool BinaryStream::seek(offset_t offset, SeekOrigin origin) {
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


	//! @brief  フラッシュ
	void BinaryStream::flush() { 
	}

}