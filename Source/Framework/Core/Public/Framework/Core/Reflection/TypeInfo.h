//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/Type.h>
#include <Framework/Core/Reflection/Any.h>
#include <Framework/Core/Reflection/AnyReference.h>
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


	using ConstructorInvoker = Func<Any(Span<AnyReference> args)>;
	using MethodInvoker = Func<Any(AnyReference& owner, Span<AnyReference> args)>;
	using PropertySetter = Func<void(const AnyReference& owner, const AnyReference& value)>;
	using PropertyGetter = Func<Any(const AnyReference& owner)>;
	using EnumValueGetter = Func<s32(const AnyReference& owner)>;


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
		Any						sample;
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
		UPtr<T> invoke(Span<AnyReference> args) const {
			return invoker(args).release<T>();
		}
		template<class T,class... Args>
		UPtr<T> invoke(Args&&... args) const {
			// 0引数に対応するために最後尾に空要素を追加している
			AnyReference rargs[] = {args...,AnyReference()};
			return invoker(Span<AnyReference>(rargs,sizeof...(Args))).release<T>();
		}

		template<class... Args>
		bool match()const {
			// 0引数に対応するために最後尾に空要素を追加している
			Type types[] = { Type::Get<Args>()... ,Type()};
			return std::equal(arguments.begin(), arguments.end(), std::begin(types), std::end(types)-1, [](const ArgumentInfo& a, const Type& b) {return a.type == b; });
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

		template<class T,class TOwner>
		T get(TOwner&& owner) const {
			return getter(owner).get<T>();
		}

		template<class T,class TOwner, class = std::enable_if_t<!std::is_const<std::remove_reference_t<TOwner>>::value>>
		void set(TOwner&& owner, T&& value) const {
			if(setter) setter(owner, value);
		}

		bool					canRead() const { return !!getter; }
		bool					canWrite() const { return !!setter; }
		bool					canReadWrite() const { return canRead() && canWrite(); }
	};

	//@―---------------------------------------------------------------------------
	//! @brief  メソッド情報
	//@―---------------------------------------------------------------------------
	struct MethodInfo : TagInfo {
		StringView				name;
		bool					isConst;
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

		Vector<StringView>		propertyOrder;
		Vector<StringView>		methodOrder;

		bool					isEnum;
		EnumValueGetter			enumValueGetter;
		Vector<EnumElementInfo>	enumElements;

		bool isSuperClassOf(const Type& type)const;

		template<class T>
		bool isSuperClassOf()const {
			return isSuperClassOf(Type::Get<T>());
		}

		template<class... Args>
		const ConstructorInfo* findConstructor()const {
			for (auto& constructor : constructors) {
				if (constructor.match<Args...>()) {
					return &constructor;
				}
			}
			return nullptr;
		}

		template<class T = void>
		const PropertyInfo* findProperty(StringView name)const {
			auto itr = properties.find(name);
			if (itr == properties.end()) return nullptr;

			if constexpr (std::is_same<T, void>::value) {
				return &itr->second;
			} else {
				if (itr->second.type.is<T>()) {
					return &itr->second;
				}
			}
			return nullptr;
		}

		const MethodInfo* findMethod(StringView name)const {
			auto itr = methods.find(name);
			if (itr == methods.end()) return nullptr;
			return &itr->second;
		}

		const EnumElementInfo* findEnumElement(StringView name)const {
			for (auto& element : enumElements) {
				if (element.name != name)continue;
				return &element;
			}
			return nullptr;
		}
		const EnumElementInfo* findEnumElement(s32 value)const {
			for (auto& element : enumElements) {
				if (element.value != value)continue;
				return &element;
			}
			return nullptr;
		}

	};

}
