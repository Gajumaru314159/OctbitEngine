//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Core.h>
#include <Framework/Core/Reflection/TypeInfo.h>
#include <Framework/Core/Reflection/TypeInfoManager.h>

namespace type_info_builder {

	struct ReflectionFunction {
		using func_type = void(*)();
		ReflectionFunction(func_type func) :func(func) {}
		func_type func;
		ReflectionFunction* next = nullptr;
	};
	ReflectionFunction* GetReflectionFunction();
	void PushReflectionFunction(ReflectionFunction*);

}


//@―---------------------------------------------------------------------------
//! @brief		Enum型情報の定義
//! @details	
//! @note		
//@―---------------------------------------------------------------------------
#define OB_DEFINE_ENUM_INFO(type)\
namespace type_info_builder::type {\
	void Register() {\
		::ob::core::internal::EnumBuilderTemplate<::type> builder{};\
	}\
}\
template<> void ::ob::core::internal::EnumBuilderTemplate<::type>::Register()


//@―---------------------------------------------------------------------------
//! @brief		Class型情報の定義
//! @details	
//! @note		
//@―---------------------------------------------------------------------------
#define OB_DEFINE_CLASS_INFO(type)\
namespace type_info_builder::type {\
	void Register() {\
		::ob::core::internal::ClassBuilderTemplate<::type> builder{};\
	}\
}\
template<> void ::ob::core::internal::ClassBuilderTemplate<::type>::Register()


namespace ob::core::internal {

	//@―---------------------------------------------------------------------------
	//! @brief		タグ情報ビルダー
	//@―---------------------------------------------------------------------------
	class TagBuilder {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		TagBuilder(TagInfo&);

		//@―---------------------------------------------------------------------------
		//! @brief		タグ追加
		//@―---------------------------------------------------------------------------
		void tag(StringView key, StringView value = "");

	private:
		TagInfo* m_info;
	};


	//@―---------------------------------------------------------------------------
	//! @brief		Enum型情報ビルダー
	//@―---------------------------------------------------------------------------
	class EnumBuilder : public TagBuilder {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		EnumBuilder(TypeInfo&);

		//@―---------------------------------------------------------------------------
		//! @brief		要素追加
		//! @details	追加した順番にインデックスが割り振られます。インデックスは0ベースです。
		//@―---------------------------------------------------------------------------
		template<typename E>
		TagBuilder element(StringView name, E value) {
			return elementImpl(name, enum_cast(value));
		}

	private:
		TagBuilder elementImpl(StringView name, s64 value);
	private:
		TypeInfo& m_info;
	};


	//@―---------------------------------------------------------------------------
	//! @brief		クラス情報ビルダー
	//@―---------------------------------------------------------------------------
	class ClassBuilder : public TagBuilder {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief			コンストラクタ
		//@―---------------------------------------------------------------------------
		ClassBuilder(TypeInfo&);


	protected:

		void baseImpl(Type);
		void constructorImpl();
		TagBuilder addPropertyImpl() {}
		TagBuilder methodImpl(StringView name);

	protected:
		TypeInfo& m_info;
	};


	//@―---------------------------------------------------------------------------
	//! @brief		Enum型情報ビルダー
	//@―---------------------------------------------------------------------------
	template<class _T>
	class EnumBuilderTemplate :public EnumBuilder {
	public:
		using T = _T;
	public:
		EnumBuilderTemplate() : EnumBuilder(TypeInfoManager::Instance().registerInfo(Type::Get<T>())) {
			Register();
		}
		void Register();
	};

	//@―---------------------------------------------------------------------------
	//! @brief		Class型情報ビルダー
	//@―---------------------------------------------------------------------------
	template<class _T>
	class ClassBuilderTemplate : public ClassBuilder {
	public:
		using T = _T;
	public:
		ClassBuilderTemplate() : ClassBuilder(TypeInfoManager::Instance().registerInfo(Type::Get<T>())) {
			Register();
		}
		void Register();

		//@―---------------------------------------------------------------------------
		//! @brief			基底クラスを追加
		//@―---------------------------------------------------------------------------
		template<class TBase, class = std::enable_if_t<std::is_base_of<TBase, T>::value>>
		void base() { baseImpl(::ob::Type::Get<TBase>()); }

		//@―---------------------------------------------------------------------------
		//! @brief			コンストラクタ追加
		//! @tparam Args	引数型リスト
		//@―---------------------------------------------------------------------------
		//! @{
		TagBuilder constructor() {
			auto& info = m_info.constructors.emplace_back();
			info.invoker = &CreateWithoutArg;
			return info;
		}

#define RETURN_TYPE(...) std::enable_if_t<std::is_constructible<T, __VA_ARGS__>::value,TagBuilder>

		template<class Arg>
		auto constructor(StringView name = "arg") -> RETURN_TYPE(Arg)
		{
			auto& info = m_info.constructors.emplace_back();
			info.arguments = {
				ArgumentInfo{Type::Get<Arg>(),name}
			};
			info.invoker = &Create<Arg>;
			return info;
		}
		template<class Arg0, class Arg1>
		auto constructor(StringView name0 = "arg0", StringView name1 = "arg1") -> RETURN_TYPE(Arg0,Arg1) {
			auto& info = m_info.constructors.emplace_back();
			info.arguments = {
				ArgumentInfo{Type::Get<Arg0>(),name0},
				ArgumentInfo{Type::Get<Arg1>(),name1},
			};
			info.invoker = &Create<Arg0, Arg1>;
			return info;
		}
		template<class Arg0, class Arg1, class Arg2>
		auto constructor(StringView name0 = "arg0", StringView name1 = "arg1", StringView name2 = "arg2") -> RETURN_TYPE(Arg0,Arg1,Arg2) {
			auto& info = m_info.constructors.emplace_back();
			info.arguments = {
				ArgumentInfo{Type::Get<Arg0>(),name0},
				ArgumentInfo{Type::Get<Arg1>(),name1},
				ArgumentInfo{Type::Get<Arg2>(),name2},
			};
			info.invoker = &Create<Arg0, Arg1, Arg2>;
			return info;
		}
		template<class Arg0, class Arg1, class Arg2, class Arg3>
		auto constructor(StringView name0 = "arg0", StringView name1 = "arg1", StringView name2 = "arg2", StringView name3 = "arg3") -> RETURN_TYPE(Arg0,Arg1,Arg2,Arg3) {
			auto& info = m_info.constructors.emplace_back();
			info.arguments = {
				ArgumentInfo{Type::Get<Arg0>(),name0},
				ArgumentInfo{Type::Get<Arg1>(),name1},
				ArgumentInfo{Type::Get<Arg2>(),name2},
				ArgumentInfo{Type::Get<Arg3>(),name3},
			};
			info.invoker = &Create<Arg0, Arg1, Arg2, Arg3>;
			return info;
		}
		//! @}

		//@―---------------------------------------------------------------------------
		//! @brief			コンストラクタ追加
		//! @tparam Args	引数型リスト
		//@―---------------------------------------------------------------------------
		template<class... Args>
		auto constructor(Array<StringView, sizeof...(Args)> names) -> RETURN_TYPE(Args...) {

			auto& info = m_info.constructors.emplace_back();

			Array<Type, sizeof...(Args)> types = { Type::Get<Args>() ... };
			for (s32 i = 0; i < types.size(); ++i) {
				auto& arg = info.arguments.emplace_back();
				arg.type = types[i];
				arg.name = names[i];
			}

			if constexpr (sizeof...(Args) == 0)
				info.invoker = &CreateWithoutArg;
			else
				info.invoker = &Create<Args...>;

			return info;
		}

#undef RETURN_TYPE



		//@―---------------------------------------------------------------------------
		//! @brief			関数追加
		//@―---------------------------------------------------------------------------
		template<class TFunc, class... TArgDescs>
		TagBuilder method(StringView name, TFunc function, TArgDescs&& ...desc) {
			auto& info = m_info.methods[name];
			info.name = name;
			return info;
		}

		//@―---------------------------------------------------------------------------
		//! @brief			プロパティ追加(メンバ変数)
		//@―---------------------------------------------------------------------------
		template<class TField>
		TagBuilder property(StringView name, TField T::* address) {
			auto& info = m_info.properties[name];
			info.name = name;
			info.type = Type::Get<TField>();
			info.getter = &Getter<TField,address>;
			return info;
		}

		//@―---------------------------------------------------------------------------
		//! @brief			プロパティ追加(Getter)
		//@―---------------------------------------------------------------------------
		template<class F>
		TagBuilder property(StringView name, F getter) {
			auto& info = m_info.properties[name];
			info.name = name;
			info.getter = [=](const AnyReference& owner) {
				return Any((owner.get<T>().*(getter))());
			};
			return info;
		}

		//@―---------------------------------------------------------------------------
		//! @brief			プロパティ追加(Getter)
		//@―---------------------------------------------------------------------------
		template<class F1, class F2>
		TagBuilder property(StringView name, F1 getter, F2 setter) {
			auto& info = m_info.properties[name];
			info.name = name;
			info.getter = [=](const AnyReference& owner) {
				return Any((owner.get<T>().*(getter))());
			};
			info.setter = [=](AnyReference& owner,const AnyReference& value) {
				using return_type = decltype((owner.get<T>().*(getter))());
				(
					owner.get<T>().*
					(setter)
				)
				(
					value.get<return_type>()
				);
			};
			return info;
		}

	private:

		static void* CreateWithoutArg([[meybe_unused]] Span<AnyReference>) {
			return new T();
		}

		template<class T,class... Args,size_t ...I>
		static void* CreateImpl(Span<AnyReference> args, std::index_sequence<I...>) {
			return new T(args[I].get<Args>()...);
		}

		template<class... Args>
		static void* Create(Span<AnyReference> args) {

			Type types[] = {Type::Get<Args>()...};
			if (!std::equal(args.begin(), args.end(), std::begin(types), std::end(types), [](const Any& a, const Type& b) {return a.type() == b; })) {
				return {};
			}

			return CreateImpl<T,Args...>(args,std::make_index_sequence<sizeof...(Args)>());
		}

		template<class TField,TField T::* address>
		static Any Getter(AnyReference owner) {
			return owner.get<T>().*address;
		}

	};

}