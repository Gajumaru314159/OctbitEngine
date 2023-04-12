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
			constexpr size_t prefix2 = GetTypeNamePrefix().size() - std::size("Prefix(void)") + std::size("<");
			constexpr size_t suffix = StringView(TC(">(void)")).size();
			constexpr size_t prefix = prefix2 +
				(
					signature.substr(prefix2).starts_with(TC("enum ")) ? std::size("enum") :
					signature.substr(prefix2).starts_with(TC("class ")) ? std::size("class") :
					signature.substr(prefix2).starts_with(TC("struct ")) ? std::size("struct") : 0
				);

			constexpr StringView name = signature.substr(prefix, signature.size() - prefix - suffix);
			
			static_assert(!std::is_volatile_v<T>, "TypeId does not support volatile.");
			
			return name;
		}

	}

#define OB_RTTI()	virtual TypeId getTypeId()const{return TypeId::Get<std::remove_cv_t<std::remove_reference_t<decltype(*this)>>>();}

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
			auto name = internal::type_id::GetTypeName<std::remove_cv_t<std::remove_reference_t<T>>>();
			return name;
		}
		//@―---------------------------------------------------------------------------
		//! @brief  無効なTypeId取得
		//@―---------------------------------------------------------------------------
		static constexpr TypeId Invalid() {
			return TypeId();
		}
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		constexpr TypeId() : TypeId( TypeId::Get<InvalidType>().name() ){}

		//@―---------------------------------------------------------------------------
		//! @brief		名前からTypeIdを生成
		//@―---------------------------------------------------------------------------
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

		class InvalidType {

		};

	private:
		StringView	m_name;
		u64			m_hash;
	};


}


//===============================================================
// ハッシュ化
//===============================================================
//! @cond
template<>
struct std::hash<ob::core::TypeId> {
public:
	size_t operator()(const ob::core::TypeId& value)const {
		return value.hash();
	}
};
//! @endcond