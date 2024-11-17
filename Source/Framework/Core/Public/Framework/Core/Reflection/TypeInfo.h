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

	using ConstructorInvoker = Func<Any(Span<Any> args)>;
	using PlacedConstructorInvoker = Func<void(void*, Span<Any> args)>;
	using PlacedDestructorInvoker = Func<void(void*)>;
	using MethodInvoker = Func<Any(Any& owner, Span<Any> args)>;
	using PropertySetter = Func<void(Any& owner, const Any& value)>;
	using PropertyGetter = Func<Any(const Any& owner)>;
	using EnumValueGetter = Func<s32(const Any& owner)>;


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
		//Any						sample;
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
		Vector<ArgumentInfo>		arguments;
		ConstructorInvoker			invoker;
		PlacedConstructorInvoker	placedInvoker;

		template<class T>
		UPtr<T> invoke(Span<Any> args) const {
			return invoker(args).release<T>();
		}
		template<class T,class... Args>
		UPtr<T> invoke(Args&&... args) const {
			// 0引数に対応するために最後尾に空要素を追加している
			Any rargs[] = {args...,Any()};
			return invoker(Span<Any>(rargs,sizeof...(Args))).release<T>();
		}

		template<class T>
		void invoke_placed(void* p,Span<Any> args) const {
			placedInvoker(p,args);
		}
		template<class T, class... Args>
		void invoke_placed(Args&&... args) const {
			// 0引数に対応するために最後尾に空要素を追加している
			Any rargs[] = { args...,Any() };
			placedInvoker(p, Span<Any>(rargs, sizeof...(Args)));
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
		bool					isReference;

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
		PlacedDestructorInvoker destructor;
		PlacedDestructorInvoker placedDestructor;

		PropertyInfoMap			properties;
		MethodInfoMap			methods;

		Vector<StringView>		propertyOrder;
		Vector<StringView>		methodOrder;

		bool					isEnum;
		EnumValueGetter			enumValueGetter;
		Vector<EnumElementInfo>	enumElements;

		void* copy(const void* other)const { return nullptr; }
		void destroy(void* pointer)const { OB_ASSERT_EXPR(destructor); destructor(pointer); }
		void destroyPlaced(void* pointer)const { OB_ASSERT_EXPR(placedDestructor);  placedDestructor(pointer); }
		void assign(void* to, const void* from)const {  }


		bool isBaseOf(const Type& super)const;

		template<class T>
		bool isBaseOf()const {
			return isBaseOf(Type::Get<T>());
		}

		bool isSuperClassOf(const Type& base)const;

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

	public:

		template<class T>
		static const TypeInfo* Find() { return Find(Type::Get<T>()); }
		static const TypeInfo* Find(const Type& type);
		static const TypeInfo* Find(StringView type);
		static const TypeInfo* Find(Type::hash_type hash);
		static void Visit(const std::function<void(const TypeInfo&)> func);

	};

}
