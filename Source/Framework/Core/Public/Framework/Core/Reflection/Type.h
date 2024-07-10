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
			return FUNC_NAME;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  型テンプレートTの名前を取得
		//@―---------------------------------------------------------------------------
		template<class T>
		constexpr StringView GetTypeName() {

			constexpr StringView signature = FUNC_NAME;

			// TODO __PRETTY_FUNCTION__ 対応
			// TODO GCC Clang 対応
			constexpr size_t prefix2 = GetTypeNamePrefix().size() - std::size("Prefix(void)") + std::size("<");
			constexpr size_t suffix = StringView(">(void)").size();
			constexpr size_t prefix = prefix2 +
				(
					signature.substr(prefix2).starts_with("enum ") ? std::size("enum") :
					signature.substr(prefix2).starts_with("class ") ? std::size("class") :
					signature.substr(prefix2).starts_with("struct ") ? std::size("struct") : 0
				);

			constexpr StringView name = signature.substr(prefix, signature.size() - prefix - suffix);
			
			static_assert(!std::is_volatile_v<T>, "Type does not support volatile.");
			
			return name;
		}

	}

#define OB_RTTI()	virtual Type getType()const{return Type::Get<std::remove_cv_t<std::remove_reference_t<decltype(*this)>>>();}

	//@―---------------------------------------------------------------------------
	//! @brief  型ID
	//@―---------------------------------------------------------------------------
	class Type {
	public:
		//@―---------------------------------------------------------------------------
		//! @brief  Type取得
		//@―---------------------------------------------------------------------------
		template<class T>
		static constexpr Type Get() {
			auto name = internal::type_id::GetTypeName<std::remove_cv_t<std::remove_reference_t<T>>>();
			return name;
		}
		//@―---------------------------------------------------------------------------
		//! @brief  無効なType取得
		//@―---------------------------------------------------------------------------
		static constexpr Type Invalid() {
			return Type();
		}
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		constexpr Type() : Type( Type::Get<InvalidType>().fullName() ){}

		//@―---------------------------------------------------------------------------
		//! @brief		名前からTypeを生成
		//@―---------------------------------------------------------------------------
		constexpr Type(StringView fullName)
			: m_fullName(fullName)
			, m_hash(0)
		{
			if (auto index = fullName.rfind(':'); index != StringView::npos) {
				m_name = fullName.substr(index+1);
			} else {
				m_name = fullName;
			}

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

		//@―---------------------------------------------------------------------------
		//! @brief		空か
		//@―---------------------------------------------------------------------------
		constexpr bool empty()const { return m_name.empty(); }

		//@―---------------------------------------------------------------------------
		//! @brief		型名
		//! @details	名前空間を含みません。
		//!				未設定の場合は空文字列を返します。
		//@―---------------------------------------------------------------------------
		constexpr StringView name() const { return m_name; }

		//@―---------------------------------------------------------------------------
		//! @brief		型名
		//! @details	名前空間を含みます。
		//!				未設定の場合は空文字列を返します。
		//@―---------------------------------------------------------------------------
		constexpr StringView fullName() const { return m_fullName; }

		//@―---------------------------------------------------------------------------
		//! @brief		ハッシュ値
		//! @details	プラットフォームによらず固定です。
		//! @note		内部実装は型名のハッシュ値です。
		//@―---------------------------------------------------------------------------
		constexpr u64 hash() const { return m_hash; }

		//@―---------------------------------------------------------------------------
		//! @brief		型比較を行う
		//! @details	この比較ではダイナミックキャストを考慮しません。
		//@―---------------------------------------------------------------------------
		template<class T>
		constexpr bool is() const { return *this == Type::Get<T>(); }

		//! @cond
		constexpr operator u32()const { return m_hash; }
		constexpr bool operator==(Type rhs)const { return m_hash == rhs.m_hash; }
		constexpr bool operator!=(Type rhs)const { return m_hash != rhs.m_hash; }
		constexpr bool operator<(Type rhs)const { return m_hash < rhs.m_hash; }
		constexpr bool operator>(Type rhs)const { return m_hash > rhs.m_hash; }
		//! @endcond

	private:

		class InvalidType {

		};

	private:
		StringView	m_fullName;
		StringView	m_name;
		u64			m_hash;
	};


}


//===============================================================
// ハッシュ化
//===============================================================
//! @cond
template<>
struct std::hash<ob::core::Type> {
public:
	size_t operator()(const ob::core::Type& value)const {
		return value.hash();
	}
};
//! @endcond