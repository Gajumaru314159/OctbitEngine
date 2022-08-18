//***********************************************************
//! @file
//! @brief		ハッシュ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Utility/ConstValue.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  ハッシュ
    //@―---------------------------------------------------------------------------
    class Hash {
    public:

		//@―---------------------------------------------------------------------------
		//! @brief  FNV-1aによるChar[]のハッシュ化
		//@―---------------------------------------------------------------------------
		template<size_t N>
		static constexpr u32 FNV32(const Char(& input)[N]) {

			constexpr u32 offset_basis = 2166136261u;
			constexpr u32 fnv_prime = 16777619u;
			u32 result = offset_basis;

			for(size_t i=0;i<N;++i) {
				result ^= static_cast<u32>(input[i]);
				result *= fnv_prime;
			}

			return result;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  FNV-1aによるChar[]のハッシュ化
		//@―---------------------------------------------------------------------------
		template<size_t N>
		static constexpr u64 FNV64(const Char(&input)[N]) {

			constexpr u64 offset_basis = 14695981039346656037u;
			constexpr u64 fnv_prime = 1099511628211u;
			u64 result = offset_basis;

			for (size_t i = 0; i < N; ++i) {
				result ^= static_cast<u64>(input[i]);
				result *= fnv_prime;
			}

			return result;
		}

    };



//@―---------------------------------------------------------------------------
//! @brief  FNV-1aによるChar[]の32bitハッシュ化
//@―---------------------------------------------------------------------------
#define OB_FNV32(str) (ConstValue<u32,Hash::FNV32(TC(str))>::value)
//@―---------------------------------------------------------------------------
//! @brief  FNV-1aによるChar[]の64bitハッシュ化
//@―---------------------------------------------------------------------------
#define OB_FNV64(str) (ConstValue<u32,Hash::FNV64(TC(str))>::value)


//@―---------------------------------------------------------------------------
//! @brief  Char[]の32bitハッシュ化
//@―---------------------------------------------------------------------------
#define OB_HASH32(str) OB_FNV32(str)
//@―---------------------------------------------------------------------------
//! @brief  Char[]の64bitハッシュ化
//@―---------------------------------------------------------------------------
#define OB_HASH64(str) OB_FNV64(str)


}// namespcae ob