//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/File/ByteOrder.h>

namespace ob::core {

	class Stream;

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------
	class BinaryReader {
	public:
		BinaryReader(Stream* stream, ByteOrder byteOrder = ByteOrder::Auto);
		~BinaryReader();

		s8  readInt8();
		s16 readInt16();
		s32 readInt32();
		s64 readInt64();
		u8  readUInt8();
		u16 readUInt16();
		u32 readUInt32();
		u64 readUInt64();
		f32 readFloat();
		f64 readDouble();

		size_t read(void* buffer, size_t count);
		void seek(offset_t offset);
		bool isEOF() const;

	private:
		class Stream* m_stream;
		ByteOrder m_byteOrder;
		//std::vector<byte_t> m_stringCache;
	};






	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond



	//! @endcond
}// namespcae ob