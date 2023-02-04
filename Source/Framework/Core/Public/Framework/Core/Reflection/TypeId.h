//***********************************************************
//! @file
//! @brief		型ID
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Hash/Hash.h>

namespace ob::core {

	namespace internal::type_id {

		//@―---------------------------------------------------------------------------
		//! @brief  文字列がある文字を含むか
		//@―---------------------------------------------------------------------------
		constexpr bool HasChar(StringView s, Char c) {
			return s.find(c) != s.npos;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  関数名のPrefixを取得
		//@―---------------------------------------------------------------------------
		constexpr StringView GetTypeNamePrefix(void) {
			return TC(FUNC_NAME);
		}

		//@―---------------------------------------------------------------------------
		//! @brief  型テンプレートTの名前を取得
		//@―---------------------------------------------------------------------------
		template<class T>
		constexpr StringView GetTypeName() {

			constexpr StringView signature = TC(FUNC_NAME);

			// TODO __PRETTY_FUNCTION__ 対応
			// TODO GCC Clang 対応
			constexpr size_t prefix = GetTypeNamePrefix().size() - std::size("Prefix(void)") + std::size("<class ");
			constexpr size_t suffix = StringView(TC(">(void)")).size();

			constexpr StringView name = signature.substr(prefix, signature.size() - prefix - suffix);
			
			static_assert(!std::is_volatile_v<T>, "TypeId does not support volatile.");
			static_assert(!HasChar(name, '*'), "TypeId does not support pointer.");
			static_assert(!HasChar(name, '&'), "TypeId does not support reference.");
			static_assert(!HasChar(name, '<'), "TypeId does not support template.");
			
			return name;
		}

	}


	//@―---------------------------------------------------------------------------
	//! @brief  型ID
	//@―---------------------------------------------------------------------------
	class TypeId {
	public:
		//@―---------------------------------------------------------------------------
		//! @brief  TypeId取得
		//@―---------------------------------------------------------------------------
		template<class T>
		static constexpr TypeId Get() {
			auto name = internal::type_id::GetTypeName<T>();
			return name;
		}
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		constexpr TypeId() :m_hash(OB_FNV64("[Empty]")) {}

		//@―---------------------------------------------------------------------------
		//! @brief		空か
		//@―---------------------------------------------------------------------------
		constexpr bool empty()const { return m_name.empty(); }

		//@―---------------------------------------------------------------------------
		//! @brief		型名
		//! @details	名前空間を含みます。
		//!				未設定の場合は空文字列を返します。
		//@―---------------------------------------------------------------------------
		constexpr StringView name() const { return m_name; }

		//@―---------------------------------------------------------------------------
		//! @brief		ハッシュ値
		//! @details	プラットフォームによらず固定です。
		//! @note		内部実装は型名のハッシュ値です。
		//@―---------------------------------------------------------------------------
		constexpr u64 hash() const { return m_hash; }

		//! @cond
		constexpr operator bool()const { return !empty(); }
		constexpr bool operator==(TypeId rhs)const { return m_hash == rhs.m_hash; }
		constexpr bool operator!=(TypeId rhs)const { return m_hash != rhs.m_hash; }
		constexpr bool operator<(TypeId rhs)const { return m_hash < rhs.m_hash; }
		constexpr bool operator>(TypeId rhs)const { return m_hash > rhs.m_hash; }
		//! @endcond

	private:

		constexpr TypeId(StringView name) 
			: m_name(name)
			, m_hash(0)
		{
			// FNV64
			constexpr u64 offset_basis = 14695981039346656037u;
			constexpr u64 fnv_prime = 1099511628211u;
			u64 result = offset_basis;

			for (size_t i = 0; i < name.size(); ++i) {
				result ^= static_cast<u64>(name[i]);
				result *= fnv_prime;
			}
			m_hash = result;
		}

	private:
		StringView	m_name;
		u64			m_hash;
	};


}// namespcae ob
