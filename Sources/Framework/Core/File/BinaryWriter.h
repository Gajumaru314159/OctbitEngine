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
	class BinaryWriter{
	public:
		BinaryWriter(Stream* stream, ByteOrder byteOrder = ByteOrder::Auto);
		~BinaryWriter();

		void writeInt8(s8 value);
		void writeInt16(s16 value);
		void writeInt32(s32 value);
		void writeInt64(s64 value);
		void writeUInt8(u8 value);
		void writeUInt16(u16 value);
		void writeUInt32(u32 value);
		void writeUInt64(u64 value);
		void writeFloat(f32 value);
		void writeDouble(f64 value);

		void write(const void* buffer, size_t count);
		void seek(offset_t offset);

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