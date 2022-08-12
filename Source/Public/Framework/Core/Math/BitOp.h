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
		static constexpr s32 GetBitCount(u8  value);
		//! @copydoc GetBitCount(u8)
		static constexpr s32 GetBitCount(u16 value);
		//! @copydoc GetBitCount(u8)
		static constexpr s32 GetBitCount(u32 value);
		//! @copydoc GetBitCount(u8)
		static constexpr s32 GetBitCount(u64 value);

		//@―---------------------------------------------------------------------------
		//! @brief 最上位ビットのインデックスを取得(Most Significant Bit)
		//@―---------------------------------------------------------------------------
		static constexpr s32 GetMSB(u8  value);
		//! @copydoc GetMSB(u8)
		static constexpr s32 GetMSB(u16 value);
		//! @copydoc GetMSB(u8)
		static constexpr s32 GetMSB(u32 value);
		//! @copydoc GetMSB(u8)
		static constexpr s32 GetMSB(u64 value);

		//@―---------------------------------------------------------------------------
		//! @brief 最下位ビットのインデックスを取得(Least Significant Bit)
		//@―---------------------------------------------------------------------------
		static constexpr s32 GetLSB(u8  value);
		//! @copydoc GetLSB(u8)
		static constexpr s32 GetLSB(u16 value);
		//! @copydoc GetLSB(u8)
		static constexpr s32 GetLSB(u32 value);
		//! @copydoc GetLSB(u8)
		static constexpr s32 GetLSB(u64 value);

	};

}// namespcae ob





//! @cond
namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief 有効ビット数を計算
	//@―---------------------------------------------------------------------------
	constexpr s32 BitOp::GetBitCount(u8  value) {
		value = (value & 0x55) + ((value >> 1) & 0x55);
		value = (value & 0x33) + ((value >> 2) & 0x33);
		value = (value & 0x0F) + ((value >> 4) & 0x0F);
		return (s32)value;
	}
	constexpr s32 BitOp::GetBitCount(u16 value) {
		value = (value & 0x5555) + ((value >> 1) & 0x5555);
		value = (value & 0x3333) + ((value >> 2) & 0x3333);
		value = (value & 0x0F0F) + ((value >> 4) & 0x0F0F);
		value = (value & 0x00FF) + ((value >> 8) & 0x00FF);
		return (s32)value;
	}
	constexpr s32 BitOp::GetBitCount(u32 value) {
		value = (value & 0x55555555) + ((value >> 1) & 0x55555555);
		value = (value & 0x33333333) + ((value >> 2) & 0x33333333);
		value = (value & 0x0F0F0F0F) + ((value >> 4) & 0x0F0F0F0F);
		value = (value & 0x00FF00FF) + ((value >> 8) & 0x00FF00FF);
		value = (value & 0x0000FFFF) + ((value >> 16) & 0x0000FFFF);
		return (s32)value;
	}
	constexpr s32 BitOp::GetBitCount(u64 value) {
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
	constexpr s32 BitOp::GetMSB(u8  value) {
		if (value == 0)return -1;
		u8 temp = value;
		temp |= temp >> 1;
		temp |= temp >> 2;
		temp |= temp >> 4;
		return GetBitCount(temp) - 1;
	}
	constexpr s32 BitOp::GetMSB(u16 value) {
		if (value == 0)return -1;
		u16 temp = value;
		temp |= temp >> 1;
		temp |= temp >> 2;
		temp |= temp >> 4;
		temp |= temp >> 8;
		return GetBitCount(temp) - 1;
	}
	constexpr s32 BitOp::GetMSB(u32 value) {
		if (value == 0)return -1;
		u32 temp = value;
		temp |= temp >> 1;
		temp |= temp >> 2;
		temp |= temp >> 4;
		temp |= temp >> 8;
		temp |= temp >> 16;
		return GetBitCount(temp) - 1;
	}
	constexpr s32 BitOp::GetMSB(u64 value) {
		if (value == 0)return -1;
		u64 temp = value;
		temp |= temp >> 1;
		temp |= temp >> 2;
		temp |= temp >> 4;
		temp |= temp >> 8;
		temp |= temp >> 16;
		temp |= temp >> 32;
		return GetBitCount(temp)-1;
	}

	//@―---------------------------------------------------------------------------
	//! @brief 最下位ビットのインデックスを取得(Least Significant Bit)
	//@―---------------------------------------------------------------------------
	constexpr s32 BitOp::GetLSB(u8  value) {
		if (value == 0)return -1;
		u8 temp = value;
		temp |= temp << 1;
		temp |= temp << 2;
		temp |= temp << 4;
		return 8 - GetBitCount(temp);
	}
	constexpr s32 BitOp::GetLSB(u16 value) {
		if (value == 0)return -1;
		u16 temp = value;
		temp |= temp << 1;
		temp |= temp << 2;
		temp |= temp << 4;
		temp |= temp << 8;
		return 16 - GetBitCount(temp);
	}
	constexpr s32 BitOp::GetLSB(u32 value) {
		if (value == 0)return -1;
		u32 temp = value;
		temp |= temp << 1;
		temp |= temp << 2;
		temp |= temp << 4;
		temp |= temp << 8;
		temp |= temp << 16;
		return 32 - GetBitCount(temp);
	}
	constexpr s32 BitOp::GetLSB(u64 value) {
		if (value == 0)return -1;
		u64 temp = value;
		temp |= temp << 1;
		temp |= temp << 2;
		temp |= temp << 4;
		temp |= temp << 8;
		temp |= temp << 16;
		temp |= temp << 32;
		return 64 - GetBitCount(temp);
	}


}// namespcae ob
//! @endcond