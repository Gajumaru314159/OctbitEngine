//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/BinaryReader.h>
#include <Framework/Core/File/Stream.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//! 
	//! @details ストリームからバイナリデータを読み込む。コンストラクタに渡す Stream はBinaryReaderより後に解放される必要があります。
	//! @param stream	 入力ストリーム
	//! @param byteOrder 入力ストリームのバイトオーダー
	//@―---------------------------------------------------------------------------
	BinaryReader::BinaryReader(Stream& stream,ByteOrder byteOrder)
		: m_stream(stream)
	{
		setByteOrder(byteOrder);
		OB_ASSERT(stream.canRead(),"読み取り可能なストリームを設定してください。");
	}

	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	BinaryReader::~BinaryReader()
	{
	}

	//@―---------------------------------------------------------------------------
	//! @brief   s8 を読み込む
	//@―---------------------------------------------------------------------------
	s8 BinaryReader::readS8()
	{
		s8 buffer;
		if (!readImpl(&buffer, sizeof(buffer)))return 0;
		return buffer;
	}

	//@―---------------------------------------------------------------------------
	//! @brief   s16 を読み込む
	//@―---------------------------------------------------------------------------
	s16 BinaryReader::readS16()
	{
		s16 buffer;
		if (!readImpl(&buffer, sizeof(buffer)))return 0;
		return buffer;
	}

	//@―---------------------------------------------------------------------------
	//! @brief   s32 を読み込む
	//@―---------------------------------------------------------------------------
	s32 BinaryReader::readS32()
	{
		s32 buffer;
		if (!readImpl(&buffer, sizeof(buffer)))return 0;
		return buffer;
	}

	//@―---------------------------------------------------------------------------
	//! @brief   s64 を読み込む
	//@―---------------------------------------------------------------------------
	s64 BinaryReader::readS64()
	{
		s64 buffer;
		if (!readImpl(&buffer, sizeof(buffer)))return 0;
		return buffer;

	}

	//@―---------------------------------------------------------------------------
	//! @brief   u8 を読み込む
	//@―---------------------------------------------------------------------------
	u8 BinaryReader::readU8()
	{
		u8 buffer;
		if (!readImpl(&buffer, sizeof(buffer)))return 0;
		return buffer;
	}

	//@―---------------------------------------------------------------------------
	//! @brief   u16 を読み込む
	//@―---------------------------------------------------------------------------
	u16 BinaryReader::readU16()
	{
		u16 buffer;
		if (!readImpl(&buffer, sizeof(buffer)))return 0;
		return buffer;
	}

	//@―---------------------------------------------------------------------------
	//! @brief   u32 を読み込む
	//@―---------------------------------------------------------------------------
	u32 BinaryReader::readU32()
	{
		u32 buffer;
		if (!readImpl(&buffer, sizeof(buffer)))return 0;
		return buffer;
	}

	//@―---------------------------------------------------------------------------
	//! @brief   u64 を読み込む
	//@―---------------------------------------------------------------------------
	u64 BinaryReader::readU64()
	{
		u64 buffer;
		if (!readImpl(&buffer, sizeof(buffer)))return 0;
		return buffer;
	}

	//@―---------------------------------------------------------------------------
	//! @brief   f32 を読み込む
	//@―---------------------------------------------------------------------------
	f32 BinaryReader::readF32()
	{
		f32 buffer;
		if (!readImpl(&buffer, sizeof(buffer)))return 0;
		return buffer;
	}

	//@―---------------------------------------------------------------------------
	//! @brief   f64 を読み込む
	//@―---------------------------------------------------------------------------
	f64 BinaryReader::readF64()
	{
		f64 buffer;
		if (!readImpl(&buffer, sizeof(buffer)))return 0;
		return buffer;
	}

	//@―---------------------------------------------------------------------------
	//! @brief   読み込み
	//@―---------------------------------------------------------------------------
	bool BinaryReader::readImpl(void* dest, size_t size) {
		size_t count = m_stream.read(dest, size);
		if (count != size) return false;
		if (m_byteOrder != Endian::Get()) {
			auto bytes = reinterpret_cast<byte*>(dest);
			std::reverse(bytes, bytes +size);
		}
		return true;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  バイト列読み込み
	//! 
	//! @param buffer 　 読み込みデータ格納先のポインタ
	//! @param byteCount 読み込むバイト数
	//! @return 読み込んだデータ個数※エラー時やストリームの終わりに達したときは byteCount 以下の値が返される
	//@―---------------------------------------------------------------------------
	size_t BinaryReader::read(void* buffer, size_t count)
	{
		return m_stream.read(buffer, count);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  シーク
	//! 
	//! @param offset 　 基準からの移動量
	//! @param origin	 基準位置
	//! @return 0 のとき成功
	//@―---------------------------------------------------------------------------
	void BinaryReader::seek(offset_t offset,SeekOrigin origin)
	{
		m_stream.seek(offset, origin);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ストリームの終わりか
	//@―---------------------------------------------------------------------------
	bool BinaryReader::isEOF() const
	{
		return (m_stream.position() >= m_stream.size());
	}

}// namespace ob