//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/TypeId.h>

namespace ob::core {



	class PropertyInfo {
	public:
		//PropertyInfo(TypeInfo* owner, Accessor accessor);

		const String& name() const { return m_name; }


	private:
		String m_name;
		//Accessor m_accessor;
	};


	class FunctionInfo {
	public:
		//PropertyInfo(TypeInfo* owner, Accessor accessor);

		const String& name() const { return m_name; }


	private:
		String m_name;
		//Accessor m_accessor;
	};


	enum class TypeHash:u64{};



	class Test {
		char const* typename_;
	public:
		Test()
			:typename_{ __func__ }
		{}

		char const* GetTypename() { return typename_; }
	};



	//@―---------------------------------------------------------------------------
	//! @brief  シーン
	//@―---------------------------------------------------------------------------
	class TypeInfo {
	public :
		static TypeInfo* addType(TypeId typeId, TypeInfo* baseClass);

	public:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		TypeInfo(StringView name, TypeInfo* baseClass);

		//@―---------------------------------------------------------------------------
		//! @brief		基底クラスを取得
		//! @detaisl	プリミティブ型はnullptrを返す。
		//@―---------------------------------------------------------------------------
		TypeInfo* base()const;

		//@―---------------------------------------------------------------------------
		//! @brief		継承しているか
		//@―---------------------------------------------------------------------------
		bool inherit(TypeInfo*)const;

		//@―---------------------------------------------------------------------------
		//! @brief		型のハッシュ値を返す
		//! @details	プラットフォームによらず固定です。
		//! @note		内部実装は型名のハッシュ値です。
		//@―---------------------------------------------------------------------------
		TypeId hash()const { return m_typeId; }

		//@―---------------------------------------------------------------------------
		//! @brief		タグを持っているか
		//@―---------------------------------------------------------------------------
		bool hasTag(StringView name)const { return m_tags.count(name); }

		//@―---------------------------------------------------------------------------
		//! @brief		タグを取得
		//@―---------------------------------------------------------------------------
		Optional<StringView> findTag(StringView name)const { auto found = m_tags.find(name); return (found == m_tags.end()) ? std::nullopt : Optional<StringView>{ found->second }; }


		// コンストラクタ

	private:
		TypeId m_typeId;
		String m_name;
		Map<String, String, std::less<>> m_tags;
	};


	//@―---------------------------------------------------------------------------
	//! @brief  Class型情報
	//@―---------------------------------------------------------------------------
	class ClassTypeInfo :public TypeInfo {
	public:
		struct PropertyInfoPred {
			constexpr bool operator()(const PropertyInfo& left, const PropertyInfo& right) const noexcept {
				return left.name() < right.name();
			}
			constexpr bool operator()(const PropertyInfo& left, StringView right) const noexcept {
				return left.name() < right;
			}
		};
		struct FunctionInfoPred {
			constexpr bool operator()(const FunctionInfo& left, const FunctionInfo& right) const noexcept {
				return left.name() < right.name();
			}
			constexpr bool operator()(const FunctionInfo& left, StringView right) const noexcept {
				return left.name() < right;
			}
		};
		using PropertyInfoSet = Set<PropertyInfo, PropertyInfoPred>;
		using FunctionInfoSet = MultiSet<FunctionInfo, PropertyInfoPred>;
	public:
		const PropertyInfoSet& properties()const { return m_properties; }

	private:
		PropertyInfoSet m_properties;
		FunctionInfoSet m_functions;
	};


	//@―---------------------------------------------------------------------------
	//! @brief  Class型情報
	//@―---------------------------------------------------------------------------
	class ListTypeInfo :public TypeInfo {
	public:
		TypeInfo* elementTypeInfo()const;
		size_t size()const;
	};


	//@―---------------------------------------------------------------------------
	//! @brief  Enum型情報
	//@―---------------------------------------------------------------------------
	class EnumTypeInfo : public TypeInfo {
	public:
		const Array<String>& items()const { return m_items; }
	private:
		Array<String> m_items;
	};

}// namespcae ob
