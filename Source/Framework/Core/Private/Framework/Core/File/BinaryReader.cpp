//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/BinaryWriter.h>
#include <Framework/Core/File/Stream.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//! 
	//! @details ストリームからバイナリデータを読み込む。コンストラクタに渡す Stream はBinaryWriterより後に解放される必要があります。
	//! @param stream	 入力ストリーム
	//! @param byteOrder 入力ストリームのバイトオーダー
	//@―---------------------------------------------------------------------------
	BinaryWriter::BinaryWriter(Stream& stream,ByteOrder byteOrder)
		: m_stream(stream)
		, m_byteOrder(byteOrder)
	{
		OB_ASSERT(stream.canWrite(),"書き込み可能なストリームを設定してください。");
	}

	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	BinaryWriter::~BinaryWriter()
	{
		m_stream.flush();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  値の書き込み
	//@―---------------------------------------------------------------------------
	void BinaryWriter::writeInt8(s8 value) {
		write(&value, sizeof(value));
	}
	void BinaryWriter::writeInt16(s16 value) {
		write(&value, sizeof(value));
	}
	void BinaryWriter::writeInt32(s32 value) {
		write(&value, sizeof(value));
	}
	void BinaryWriter::writeInt64(s64 value) {
		write(&value, sizeof(value));
	}
	void BinaryWriter::writeUInt8(u8 value) {
		write(&value, sizeof(value));
	}
	void BinaryWriter::writeUInt16(u16 value) {
		write(&value, sizeof(value));
	}
	void BinaryWriter::writeUInt32(u32 value) {
		write(&value, sizeof(value));
	}
	void BinaryWriter::writeUInt64(u64 value) {
		write(&value, sizeof(value));
	}
	void BinaryWriter::writeFloat(f32 value) {
		write(&value, sizeof(value));
	}
	void BinaryWriter::writeDouble(f64 value) {
		write(&value, sizeof(value));
	}
	void BinaryWriter::write(const void* buffer, size_t count) {
		m_stream.write(buffer, count);
	}
	void BinaryWriter::seek(offset_t offset) {
		m_stream.seek(offset, SeekOrigin::Current);
	}
	void BinaryWriter::flush() {
		m_stream.flush();
	}

}