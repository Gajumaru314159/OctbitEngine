//***********************************************************
//! @file
//! @brief		ハッシュ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Utility/ConstValue.h>
#include <Framework/Core/String/StringView.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  ハッシュ
	//@―---------------------------------------------------------------------------
	class Hash {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  FNV-1aによる文字列のハッシュ化
		//@―---------------------------------------------------------------------------
		template<class TChar>
		static constexpr u32 FNV32(StringViewBase<TChar> text) {

			constexpr u32 offset_basis = 2166136261u;
			constexpr u32 fnv_prime = 16777619u;
			u32 result = offset_basis;

			for (size_t i = 0; i < text.size(); ++i) {
				result ^= static_cast<u32>(text[i]);
				result *= fnv_prime;
			}

			return result;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  FNV-1aによる文字列のハッシュ化
		//@―---------------------------------------------------------------------------
		template<class TChar>
		static constexpr u64 FNV64(StringViewBase<TChar> text) {

			constexpr u64 offset_basis = 14695981039346656037u;
			constexpr u64 fnv_prime = 1099511628211u;
			u64 result = offset_basis;

			for (size_t i = 0; i < text.size(); ++i) {
				result ^= static_cast<u64>(text[i]);
				result *= fnv_prime;
			}

			return result;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  FNV-1aによるTChar[]のハッシュ化
		//@―---------------------------------------------------------------------------
		template<class TChar,size_t N>
		static constexpr u32 FNV32(const TChar(&input)[N]) {

			constexpr u32 offset_basis = 2166136261u;
			constexpr u32 fnv_prime = 16777619u;
			u32 result = offset_basis;

			for (size_t i = 0; i < N-1; ++i) {
				result ^= static_cast<u32>(input[i]);
				result *= fnv_prime;
			}

			return result;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  FNV-1aによるTChar[]のハッシュ化
		//@―---------------------------------------------------------------------------
		template<class TChar,size_t N>
		static constexpr u64 FNV64(const TChar(&input)[N]) {

			constexpr u64 offset_basis = 14695981039346656037u;
			constexpr u64 fnv_prime = 1099511628211u;
			u64 result = offset_basis;

			for (size_t i = 0; i < N-1; ++i) {
				result ^= static_cast<u64>(input[i]);
				result *= fnv_prime;
			}

			return result;
		}

	};



//@―---------------------------------------------------------------------------
//! @brief  FNV-1aによる文字列の32bitハッシュ化
//@―---------------------------------------------------------------------------
#define OB_FNV32(str) (ConstValue<u32,Hash::FNV32(TC(str))>::value)
//@―---------------------------------------------------------------------------
//! @brief  FNV-1aによる文字列の64bitハッシュ化
//@―---------------------------------------------------------------------------
#define OB_FNV64(str) (ConstValue<u64,Hash::FNV64(TC(str))>::value)


//@―---------------------------------------------------------------------------
//! @brief  文字列の32bitハッシュ化
//@―---------------------------------------------------------------------------
#define OB_HASH32(str) OB_FNV32(str)
//@―---------------------------------------------------------------------------
//! @brief  文字列の64bitハッシュ化
//@―---------------------------------------------------------------------------
#define OB_HASH64(str) OB_FNV64(str)


}