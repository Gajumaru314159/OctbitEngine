//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Hash/Hash.h>

namespace ob::core {

	namespace internal::type_id {

		//! @brief  関数名のPrefixを取得
		constexpr StringView GetTypeName(void) {
			return FUNC_NAME;
		}

		//! @brief  型テンプレートTの名前を取得
		template<class T>
		constexpr StringView GetTypeName(void) {

			static_assert(!std::is_volatile_v<T>, "Type does not support volatile.");

			using namespace std::string_view_literals;

			constexpr StringView signature = FUNC_NAME;

			// TODO __PRETTY_FUNCTION__ 対応
			// TODO GCC Clang 対応
			
#if defined(__clang__)
			constexpr size_t prefix = GetTypeName().size() + " [T = "sv.size();
			constexpr size_t suffix = "]"sv.size();
#elif defined(_MSC_VER)
			constexpr size_t prefix2 = GetTypeName().size() - "(void)"sv.size() + "<"sv.size();
			constexpr size_t suffix = ">(void)"sv.size();
			constexpr size_t prefix = prefix2 +
				(
					signature.substr(prefix2).starts_with("enum ") ? "enum "sv.size() :
					signature.substr(prefix2).starts_with("class ") ? "class "sv.size() :
					signature.substr(prefix2).starts_with("struct ") ? "struct "sv.size() :
					signature.substr(prefix2).starts_with("union ") ? "union "sv.size() : 0
					);
#endif


			constexpr StringView name = signature.substr(prefix, signature.size() - prefix - suffix);
			

			return name;
		}

	}

#define OB_RTTI()	virtual Type getType()const{return Type::Get<std::remove_cv_t<std::remove_reference_t<decltype(*this)>>>();}

	//! @brief  型ID
	class Type {
	public:
		using hash_type = u64;
	public:
		//! @brief  Type取得
		template<class T>
		static constexpr Type Get() {
			constexpr auto name = internal::type_id::GetTypeName<std::remove_cv_t<std::remove_reference_t<T>>>();
			return name;
		}
		//! @brief  Type取得
		template<class T>
		static constexpr Type Get(T&&) {
			auto name = internal::type_id::GetTypeName<std::remove_cv_t<std::remove_reference_t<T>>>();
			return name;
		}
		//! @brief  無効なType取得
		static constexpr Type Invalid() {
			return Type();
		}
	public:

		//! @brief		コンストラクタ
		constexpr Type() : Type( Type::Get<InvalidType>().name() ){}

		//! @brief		名前からTypeを生成
		constexpr Type(StringView fullName)
			: m_name(fullName)
			, m_hash(0)
		{
			// FNV64
			constexpr u64 offset_basis = 14695981039346656037u;
			constexpr u64 fnv_prime = 1099511628211u;
			u64 result = offset_basis;

			for (size_t i = 0; i < fullName.size(); ++i) {
				result ^= static_cast<u64>(fullName[i]);
				result *= fnv_prime;
			}
			m_hash = result;
		}

		//! @brief		空か
		constexpr bool empty()const { return m_name.empty(); }

		//! @brief		型名
		//! @details	名前空間を含みます。
		//!				未設定の場合は空文字列を返します。
		constexpr StringView name() const { return m_name; }

		//! @brief		型名
		//! @details	名前空間を含みません。
		//!				未設定の場合は空文字列を返します。
		constexpr StringView shortName() const { 
			auto limit = m_name.find('<');
			if (limit == m_name.npos) limit = m_name.size();

			if (auto index = m_name.rfind(':',limit); index != StringView::npos) {
				return m_name.substr(index + 1);
			}
			else {
				return m_name;
			}		
		}

		//! @brief		型名
		//! @details	名前空間を含みません。
		//!				未設定の場合は空文字列を返します。
		constexpr StringView nameSpace() const {
			auto limit = m_name.find('<');
			if (limit == m_name.npos) limit = m_name.size();

			if (auto index = m_name.rfind(':',limit); index != StringView::npos) {
				return m_name.substr(0,index - 1);
			}
			else {
				return "";
			}
		}

		//! @brief		ハッシュ値
		//! @details	プラットフォームによらず固定です。
		//! @note		内部実装は型名のハッシュ値です。
		constexpr hash_type hash() const { return m_hash; }

		//! @brief		型比較を行う
		//! @details	この比較ではダイナミックキャストを考慮しません。
		template<class T>
		constexpr bool is() const { return *this == Type::Get<T>(); }

		//! @cond
		constexpr operator hash_type()const { return m_hash; }
		constexpr bool operator==(Type rhs)const { return m_hash == rhs.m_hash; }
		constexpr bool operator!=(Type rhs)const { return m_hash != rhs.m_hash; }
		constexpr bool operator<(Type rhs)const { return m_hash < rhs.m_hash; }
		constexpr bool operator>(Type rhs)const { return m_hash > rhs.m_hash; }
		//! @endcond

	private:

		friend class TypeInfoManager;
		Type(Type::hash_type hash) {
			m_hash = hash;
		}

		class InvalidType {

		};

	private:
		StringView	m_name;
		hash_type	m_hash;
	};


}


//===============================================================
// ハッシュ化
//===============================================================
//! @cond
template<>
struct std::hash<ob::core::Type> {
public:
	size_t operator()(const ob::core::Type& value)const noexcept {
		return value.hash();
	}
};
//! @endcond