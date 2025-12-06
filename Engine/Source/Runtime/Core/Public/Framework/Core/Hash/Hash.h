//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/String/StringView.h>

namespace ob::core {

	//! @brief  ハッシュ
	class Hash {
	public:

		template <class T>
		static constexpr void Combine(size_t& seed, const T& v)
		{
			std::hash<T> hasher;
			constexpr std::size_t kMul = 0x9ddfea08eb382d69ULL;
			size_t a = (hasher(v) ^ seed) * kMul;
			a ^= (a >> 47);
			size_t b = (seed ^ a) * kMul;
			b ^= (b >> 47);
			seed = b * kMul;
		}

		template <class T, class... Args>
		static constexpr void Combine(size_t& seed, const T& v, const Args&... args)
		{
			Combine(seed, v);
			if constexpr (sizeof...(args) > 0) {
				Combine(seed, args...);
			}
		}


		//! @brief  FNV-1aによる文字列のハッシュ化
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

		//! @brief  FNV-1aによる文字列のハッシュ化
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

		//! @brief  FNV-1aによるTChar[]のハッシュ化
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

		//! @brief  FNV-1aによるTChar[]のハッシュ化
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



//! @brief  FNV-1aによる文字列の32bitハッシュ化
#define OB_FNV32(str) (ConstValue<u32,ob::core::Hash::FNV32(str)>::value)
//! @brief  FNV-1aによる文字列の64bitハッシュ化
#define OB_FNV64(str) (ConstValue<u64,ob::core::Hash::FNV64(str)>::value)


//! @brief  文字列の32bitハッシュ化
#define OB_HASH32(str) OB_FNV32(str)
//! @brief  文字列の64bitハッシュ化
#define OB_HASH64(str) OB_FNV64(str)


}