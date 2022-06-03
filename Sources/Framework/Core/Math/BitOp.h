//***********************************************************
//! @file
//! @brief		ビット操作ユーティリティ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CoreTypes.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief      ビット操作ユーティリティ
	//@―---------------------------------------------------------------------------
	struct BitOp {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief 有効ビット数を計算
		//@―---------------------------------------------------------------------------
		static inline s32 getBitCount(u8  value);
		//! @copydoc getBitCount(u8)
		static inline s32 getBitCount(u16 value);
		//! @copydoc getBitCount(u8)
		static inline s32 getBitCount(u32 value);
		//! @copydoc getBitCount(u8)
		static inline s32 getBitCount(u64 value);

		//@―---------------------------------------------------------------------------
		//! @brief 最上位ビットのインデックスを取得(Most Significant Bit)
		//@―---------------------------------------------------------------------------
		static inline s32 getMSB(u8  value);
		//! @copydoc getMSB(u8)
		static inline s32 getMSB(u16 value);
		//! @copydoc getMSB(u8)
		static inline s32 getMSB(u32 value);
		//! @copydoc getMSB(u8)
		static inline s32 getMSB(u64 value);

		//@―---------------------------------------------------------------------------
		//! @brief 最下位ビットのインデックスを取得(Least Significant Bit)
		//@―---------------------------------------------------------------------------
		static inline s32 getLSB(u8  value);
		//! @copydoc getLSB(u8)
		static inline s32 getLSB(u16 value);
		//! @copydoc getLSB(u8)
		static inline s32 getLSB(u32 value);
		//! @copydoc getLSB(u8)
		static inline s32 getLSB(u64 value);

	};

}// namespcae ob





//! @cond
namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief 有効ビット数を計算
	//@―---------------------------------------------------------------------------
	inline s32 BitOp::getBitCount(u8  value) {
		value = (value & 0x55) + ((value >> 1) & 0x55);
		value = (value & 0x33) + ((value >> 2) & 0x33);
		value = (value & 0x0F) + ((value >> 4) & 0x0F);
		return (s32)value;
	}
	inline s32 BitOp::getBitCount(u16 value) {
		value = (value & 0x5555) + ((value >> 1) & 0x5555);
		value = (value & 0x3333) + ((value >> 2) & 0x3333);
		value = (value & 0x0F0F) + ((value >> 4) & 0x0F0F);
		value = (value & 0x00FF) + ((value >> 8) & 0x00FF);
		return (s32)value;
	}
	inline s32 BitOp::getBitCount(u32 value) {
		value = (value & 0x55555555) + ((value >> 1) & 0x55555555);
		value = (value & 0x33333333) + ((value >> 2) & 0x33333333);
		value = (value & 0x0F0F0F0F) + ((value >> 4) & 0x0F0F0F0F);
		value = (value & 0x00FF00FF) + ((value >> 8) & 0x00FF00FF);
		value = (value & 0x0000FFFF) + ((value >> 16) & 0x0000FFFF);
		return (s32)value;
	}
	inline s32 BitOp::getBitCount(u64 value) {
		value = (value & 0x5555555555555555) + ((value >> 1) & 0x5555555555555555);
		value = (value & 0x3333333333333333) + ((value >> 2) & 0x3333333333333333);
		value = (value & 0x0F0F0F0F0F0F0F0F) + ((value >> 4) & 0x0F0F0F0F0F0F0F0F);
		value = (value & 0x00FF00FF00FF00FF) + ((value >> 8) & 0x00FF00FF00FF00FF);
		value = (value & 0x0000FFFF0000FFFF) + ((value >> 16) & 0x0000FFFF0000FFFF);
		value = (value & 0x0000000FFFF0FFFF) + ((value >> 32) & 0x0000000FFFF0FFFF);
		return (s32)value;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  最上位ビットのインデックスを取得(Most Significant Bit)
	//@―---------------------------------------------------------------------------
	inline s32 BitOp::getMSB(u8  value) {
		u8 temp;
		if (value == 0)return -1;
		temp = value;
		temp |= temp >> 1;
		temp |= temp >> 2;
		temp |= temp >> 4;
		return getBitCount(temp) - 1;
	}
	inline s32 BitOp::getMSB(u16 value) {
		u16 temp;
		if (value == 0)return -1;
		temp = value;
		temp |= temp >> 1;
		temp |= temp >> 2;
		temp |= temp >> 4;
		temp |= temp >> 8;
		return getBitCount(temp) - 1;
	}
	inline s32 BitOp::getMSB(u32 value) {
		u32 temp;
		if (value == 0)return -1;
		temp = value;
		temp |= temp >> 1;
		temp |= temp >> 2;
		temp |= temp >> 4;
		temp |= temp >> 8;
		temp |= temp >> 16;
		return getBitCount(temp) - 1;
	}
	inline s32 BitOp::getMSB(u64 value) {
		u64 temp;
		if (value == 0)return -1;
		temp = value;
		temp |= temp >> 1;
		temp |= temp >> 2;
		temp |= temp >> 4;
		temp |= temp >> 8;
		temp |= temp >> 16;
		temp |= temp >> 32;
		return getBitCount(temp)-1;
	}

	//@―---------------------------------------------------------------------------
	//! @brief 最下位ビットのインデックスを取得(Least Significant Bit)
	//@―---------------------------------------------------------------------------
	inline s32 BitOp::getLSB(u8  value) {
		u8 temp;
		if (value == 0)return -1;
		temp = value;
		temp |= temp << 1;
		temp |= temp << 2;
		temp |= temp << 4;
		return 8 - getBitCount(temp);
	}
	inline s32 BitOp::getLSB(u16 value) {
		u16 temp;
		if (value == 0)return -1;
		temp = value;
		temp |= temp << 1;
		temp |= temp << 2;
		temp |= temp << 4;
		temp |= temp << 8;
		return 16 - getBitCount(temp);
	}
	inline s32 BitOp::getLSB(u32 value) {
		u32 temp;
		if (value == 0)return -1;
		temp = value;
		temp |= temp << 1;
		temp |= temp << 2;
		temp |= temp << 4;
		temp |= temp << 8;
		temp |= temp << 16;
		return 32 - getBitCount(temp);
	}
	inline s32 BitOp::getLSB(u64 value) {
		u64 temp;
		if (value == 0)return -1;
		temp = value;
		temp |= temp << 1;
		temp |= temp << 2;
		temp |= temp << 4;
		temp |= temp << 8;
		temp |= temp << 16;
		temp |= temp << 32;
		return 64 - getBitCount(temp);
	}


}// namespcae ob
//! @endcond