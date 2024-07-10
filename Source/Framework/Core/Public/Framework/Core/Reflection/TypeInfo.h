//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/Type.h>
#include <Framework/Core/Reflection/Any.h>
#include <Framework/Core/Template/Container/Vector.h>
#include <Framework/Core/Template/Container/Map.h>
#include <Framework/Core/Template/Utility/Function.h>

namespace ob::core {

	struct PropertyConverter {
		s32 fromVersion;
		s32 toVersion;
		StringView from;
		StringView to;
	};


	struct TypedValue {
		TypedValue() = default;
		template<class T>
		TypedValue(T* value) : type(Type::Get<T>()), pointer(value) {}
		Type		type;
		const void*	pointer = nullptr;
	};


	using ConstructorInvoker = Func<TypedValue(Span<TypedValue> args)>;
	using MethodInvoker = Func<TypedValue(TypedValue owner, Span<TypedValue> args)>;
	using PropertySetter = Func<void(TypedValue owner, TypedValue value)>;
	using PropertyGetter = Func<TypedValue(TypedValue owner)>;


	//@―---------------------------------------------------------------------------
	//! @brief  タグ情報
	//@―---------------------------------------------------------------------------
	struct TagInfo {
		HashMap<StringView, String> tags;

		//@―---------------------------------------------------------------------------
		//! @brief		タグを持っているか
		//@―---------------------------------------------------------------------------
		bool hasTag(StringView name)const { return tags.count(name); }

		//@―---------------------------------------------------------------------------
		//! @brief		タグを取得
		//@―---------------------------------------------------------------------------
		Optional<StringView> findTag(StringView name)const { auto found = tags.find(name); return (found == tags.end()) ? std::nullopt : Optional<StringView>{ found->second }; }
	};


	//@―---------------------------------------------------------------------------
	//! @brief  Enum要素情報
	//@―---------------------------------------------------------------------------
	struct EnumElementInfo : TagInfo {
		StringView				name;
		s32						index;
		s64						value;
	};

	//@―---------------------------------------------------------------------------
	//! @brief  引数情報
	//@―---------------------------------------------------------------------------
	struct ArgumentInfo {
		Type					type;
		StringView				name;
	};

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ情報
	//@―---------------------------------------------------------------------------
	struct ConstructorInfo : TagInfo {
		Vector<ArgumentInfo>	arguments;
		ConstructorInvoker		invoker;

		template<class T>
		T* invoke(Span<TypedValue> args) const {
			return reinterpret_cast<T*>(const_cast<void*>(invoker(args).pointer));
		}

		template<class... Args>
		bool match()const {
			Type types[] = { Type::Get<Args>()... };
			return std::equal(arguments.begin(), arguments.end(), std::begin(types), std::end(types), [](const ArgumentInfo& a, const Type& b) {return a.type == b; });
		}
	};

	//@―---------------------------------------------------------------------------
	//! @brief  プロパティ情報
	//@―---------------------------------------------------------------------------
	struct PropertyInfo : TagInfo {
		Type					type;
		StringView				name;
		PropertySetter			setter;
		PropertyGetter			getter;

		template<class T>
		T get(TypedValue owner) const {
			return *reinterpret_cast<const T*>(getter(owner).pointer);
		}

		template<class T,class TOwner>
		void set(TOwner&& owner, T&& value) const {
			setter(TypedValue(&owner), TypedValue(&value));
		}

		bool					canRead() const { return !!getter; }
		bool					canWrite() const { return !!setter; }
	};

	//@―---------------------------------------------------------------------------
	//! @brief  メソッド情報
	//@―---------------------------------------------------------------------------
	struct MethodInfo : TagInfo {
		StringView				name;
		bool					isStatic;
		Type					returnType;
		Vector<ArgumentInfo>	arguments;
		MethodInvoker			invoke;
	};

	//@―---------------------------------------------------------------------------
	//! @brief  プロパティタイプマップ
	//@―---------------------------------------------------------------------------
	using PropertyInfoMap = HashMap<StringView, PropertyInfo>;

	//@―---------------------------------------------------------------------------
	//! @brief  メソッドタイプマップ
	//@―---------------------------------------------------------------------------
	using MethodInfoMap = HashMap<StringView, MethodInfo>;

	//@―---------------------------------------------------------------------------
	//! @brief  タイプ情報
	//@―---------------------------------------------------------------------------
	struct TypeInfo : TagInfo {
		Type					type;
		HashSet<Type>			bases;

		Vector<ConstructorInfo>	constructors;

		PropertyInfoMap			properties;
		MethodInfoMap			methods;

		bool					isEnum;
		Vector<EnumElementInfo>	enumElements;

		bool					isList;
		Optional<Type>			elementType;
	};

}
