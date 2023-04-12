﻿//***********************************************************
//! @file
//! @brief		バイナリ・リーダー
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/File/ByteOrder.h>
#include <Framework/Core/File/FileTypes.h>

namespace ob::core {

	class Stream;

	//@―---------------------------------------------------------------------------
	//! @brief  バイナリ・リーダー
	//@―---------------------------------------------------------------------------
	class BinaryReader {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//! 
		//! @details ストリームからバイナリデータを読み込む。コンストラクタに渡す Stream はBinaryReaderより後に解放される必要があります。
		//! @param stream	 入力ストリーム
		//! @param byteOrder 入力ストリームのバイトオーダー
		//@―---------------------------------------------------------------------------
		BinaryReader(Stream& stream, ByteOrder byteOrder = ByteOrder::Auto);
		~BinaryReader();	//!< デストラクタ
		s8  readS8();		//!< s8 を読み込む
		s16 readS16();		//!< s16 を読み込む
		s32 readS32();		//!< s32 を読み込む
		s64 readS64();		//!< s64 を読み込む
		u8  readU8();		//!< u8 を読み込む
		u16 readU16();		//!< u16 を読み込む
		u32 readU32();		//!< u32 を読み込む
		u64 readU64();		//!< u64 を読み込む
		f32 readF32();		//!< f32 を読み込む
		f64 readF64();		//!< f64 を読み込む


		//@―---------------------------------------------------------------------------
		//! @brief  バイト列読み込み
		//! 
		//! @param buffer 　 読み込みデータ格納先のポインタ
		//! @param byteCount 読み込むバイト数
		//! @return 読み込んだデータ個数※エラー時やストリームの終わりに達したときは byteCount 以下の値が返される
		//@―---------------------------------------------------------------------------
		size_t read(void* buffer, size_t count);


		//@―---------------------------------------------------------------------------
		//! @brief  バイト列読み込み
		//! 
		//! @param dest 　 読み込み先
		//! @return 読み込んだデータ個数※エラー時やストリームの終わりに達したときは byteCount 以下の値が返される
		//@―---------------------------------------------------------------------------
		template<class T>
		size_t read(T& dest) {
			return read(&dest, sizeof(T));
		}


		//@―---------------------------------------------------------------------------
		//! @brief  シーク
		//! 
		//! @param offset 　 基準からの移動量
		//! @param origin	 基準位置
		//! @return 0 のとき成功
		//@―---------------------------------------------------------------------------
		void seek(offset_t offset, SeekOrigin origin);

		//@―---------------------------------------------------------------------------
		//! @brief  ストリームの終わりか
		//@―---------------------------------------------------------------------------
		bool isEOF() const;

		//@―---------------------------------------------------------------------------
		//! @brief  バイトオーダーを設定
		//@―---------------------------------------------------------------------------
		void setByteOrder(ByteOrder byteOrder);

		//@―---------------------------------------------------------------------------
		//! @brief  バイトオーダーを逆にする
		//@―---------------------------------------------------------------------------
		void swapByteOrder();

	private:

		bool readImpl(void* dest, size_t size);

	private:
		Stream& m_stream;
		ByteOrder m_byteOrder;
	};






	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond

	//@―---------------------------------------------------------------------------
	//! @brief  バイトオーダーを設定
	//@―---------------------------------------------------------------------------
	inline void BinaryReader::setByteOrder(ByteOrder byteOrder) {
		m_byteOrder = byteOrder;
		if (byteOrder == ByteOrder::Auto) {
			m_byteOrder = Endian::Get();
		}
	}


	//@―---------------------------------------------------------------------------
	//! @brief  バイトオーダーを逆にする
	//@―---------------------------------------------------------------------------
	inline void BinaryReader::swapByteOrder() {
		m_byteOrder = m_byteOrder == ByteOrder::BigEndian ? ByteOrder::LittleEndian : ByteOrder::BigEndian;
	}

	//! @endcond
}