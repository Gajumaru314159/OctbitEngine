//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/BinaryReader.h>
#include <Framework/Core/File/Stream.h>

namespace ob::core {

	BinaryReader::BinaryReader(Stream* stream,ByteOrder byteOrder)
		: m_stream(stream)
		, m_byteOrder(byteOrder)
	{
		OB_CHECK_ASSERT_EXPR(stream);
		OB_CHECK_ASSERT_EXPR(stream->canRead());
	}

	BinaryReader::~BinaryReader()
	{
	}

	s8 BinaryReader::readInt8()
	{
		s8 buffer;
		m_stream->read(&buffer, 1);
		return buffer;
	}

	s16 BinaryReader::readInt16()
	{
		byte buffer[2];
		size_t count = m_stream->read(&buffer, std::size(buffer));
		if (count == std::size(buffer)) return 0;

		if (m_byteOrder == ByteOrder::LittleEndian) {
			std::swap(buffer[0], buffer[1]);
		}
		return *((s16*)buffer);
	}

	s32 BinaryReader::readInt32()
	{
		byte buffer[4];
		size_t count = m_stream->read(&buffer, std::size(buffer));
		if (count == std::size(buffer)) return 0;

		if (m_byteOrder == ByteOrder::LittleEndian) {
			std::reverse(std::begin(buffer), std::end(buffer));
		}
		return *((s32*)buffer);
	}

	s64 BinaryReader::readInt64()
	{
		byte buffer[8];
		size_t count = m_stream->read(&buffer, std::size(buffer));
		if (count == std::size(buffer)) return 0;

		if (m_byteOrder == ByteOrder::LittleEndian) {
			std::reverse(std::begin(buffer), std::end(buffer));
		}
		return *((s64*)buffer);

	}

	u8 BinaryReader::readUInt8()
	{
		u8 buffer;
		m_stream->read(&buffer, 1);
		return buffer;
	}

	u16 BinaryReader::readUInt16()
	{
		byte buffer[2];
		size_t count = m_stream->read(&buffer, std::size(buffer));
		if (count == std::size(buffer)) return 0;

		if (m_byteOrder == ByteOrder::LittleEndian) {
			std::reverse(std::begin(buffer), std::end(buffer));
		}
		return *((u16*)buffer);
	}

	u32 BinaryReader::readUInt32()
	{
		byte buffer[4];
		size_t count = m_stream->read(&buffer, std::size(buffer));
		if (count == std::size(buffer)) return 0;

		if (m_byteOrder == ByteOrder::LittleEndian) {
			std::reverse(std::begin(buffer), std::end(buffer));
		}
		return *((u32*)buffer);
	}

	u64 BinaryReader::readUInt64()
	{
		byte buffer[8];
		size_t count = m_stream->read(&buffer, std::size(buffer));
		if (count == std::size(buffer)) return 0;

		if (m_byteOrder == ByteOrder::LittleEndian) {
			std::reverse(std::begin(buffer), std::end(buffer));
		}
		return *((u64*)buffer);
	}

	f32 BinaryReader::readFloat()
	{
		f32 buffer;
		m_stream->read(&buffer, 4);
		return buffer;
	}

	f64 BinaryReader::readDouble()
	{
		f64 buffer;
		m_stream->read(&buffer, 8);
		return buffer;
	}

	size_t BinaryReader::read(void* buffer, size_t count)
	{
		if (m_stream==nullptr) return 0;
		return m_stream->read(buffer, count);
	}

	void BinaryReader::seek(offset_t offset)
	{
		if (m_stream == nullptr) return;
		m_stream->seek(offset, SeekOrigin::Current);
	}

	bool BinaryReader::isEOF() const
	{
		if (m_stream == nullptr) return true;
		return (m_stream->position() >= m_stream->size());
	}

}// namespace ob