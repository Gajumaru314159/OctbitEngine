//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/File/ByteOrder.h>

namespace ob::core {

	class Stream;

	//! @brief  説明
	class BinaryWriter{
	public:
		explicit BinaryWriter(Stream& stream, ByteOrder byteOrder = ByteOrder::Auto);
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
		void writeString(StringView value);

		void write(const void* buffer, size_t count);
		void seek(offset_t offset);
		void flush();

		template<class T>
		void write(const T& value) {
			using pure_type = remove_cvr_t<T>;
			if constexpr (std::is_same<pure_type, s8>::value)
				writeInt8(value);
			else if constexpr (std::is_same<pure_type, s16>::value)
				writeInt16(value);
			else if constexpr (std::is_same<pure_type, s32>::value)
				writeInt32(value);
			else if constexpr (std::is_same<pure_type, s64>::value)
				writeInt64(value);
			else if constexpr (std::is_same<pure_type, u8>::value)
				writeUInt8(value);
			else if constexpr (std::is_same<pure_type, u16>::value)
				writeUInt16(value);
			else if constexpr (std::is_same<pure_type, u32>::value)
				writeUInt32(value);
			else if constexpr (std::is_same<pure_type, u64>::value)
				writeUInt64(value);
			else if constexpr (std::is_same<pure_type, f32>::value)
				writeFloat(value);
			else if constexpr (std::is_same<pure_type, f64>::value)
				writeDouble(value);
			else
				static_assert(false,"not implemented");
		}

	private:
		Stream& m_stream;
		ByteOrder m_byteOrder;
	};

}