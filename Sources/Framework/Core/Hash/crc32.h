//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class Hash {
    public:

		// sha3-512


		//@―---------------------------------------------------------------------------
		//! @brief  FNV-1aによるCharのハッシュ化
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
		//! @brief  FNV-1aによるCharのハッシュ化
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






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

	template <typename T, T N> struct ConstValue { static const T value = N; };

#define OB_FNV32(str) (ConstValue<u32,Hash::FNV32(TC(str))>::value)
#define OB_FNV64(str) (ConstValue<u32,Hash::FNV64(TC(str))>::value)

    //! @endcond
}// namespcae ob