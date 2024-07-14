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

		//@―---------------------------------------------------------------------------
		//! @brief		説明をDescriptionタグとして追加
		//@―---------------------------------------------------------------------------
		void desc(StringView value);

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
			auto& element = m_info.enumElements.emplace_back();
			element.name = name;
			element.index = m_info.enumElements.size() - 1;
			element.value = enum_cast(value);
			return element;
		}

	protected:
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

		StringView getArgumentName(size_t index);

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

		//@―---------------------------------------------------------------------------
		//! @brief			コンストラクタ
		//@―---------------------------------------------------------------------------
		EnumBuilderTemplate() 
			: EnumBuilder(TypeInfoManager::Instance().registerInfo(Type::Get<T>()))
		{
			// 基底型登録
			m_info.bases.emplace(Type::Get<std::underlying_type_t<T>>());
			m_info.isEnum = true;

			// コンストラクタ登録
			auto& ctor = m_info.constructors.emplace_back();
			ctor.arguments = { {Type::Get<T>(),"value"}};
			ctor.invoker = [](Span<ConstAnyReference> args) { return Any(std::make_unique<T>(args[0].get<T>())); };

			// 値取得
			m_info.enumValueGetter = [](const ConstAnyReference& instance) {
				return enum_cast(instance.get<T>());
			};

			// タイプ登録
			Register();
		}

		//@―---------------------------------------------------------------------------
		//! @brief			タイプ登録
		//@―---------------------------------------------------------------------------
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

		//@―---------------------------------------------------------------------------
		//! @brief			コンストラクタ
		//@―---------------------------------------------------------------------------
		ClassBuilderTemplate() : ClassBuilder(TypeInfoManager::Instance().registerInfo(Type::Get<T>())) {
			// タイプ登録
			Register();
		}

		//@―---------------------------------------------------------------------------
		//! @brief			タイプ登録
		//@―---------------------------------------------------------------------------
		void Register();

		//@―---------------------------------------------------------------------------
		//! @brief			基底クラスを追加
		//@―---------------------------------------------------------------------------
		template<class TBase, class = std::enable_if_t<std::is_base_of<TBase, T>::value>>
		void base() {
			m_info.bases.emplace(::ob::Type::Get<TBase>());
		}

		//@―---------------------------------------------------------------------------
		//! @brief			コンストラクタを追加
		//@―---------------------------------------------------------------------------
		//! @{
		TagBuilder constructor() {
			static_assert(std::is_constructible<T>::value,"0引数のコンストラクタがありません");
			auto& info = m_info.constructors.emplace_back();
			info.invoker = &CreateWithoutArgs;
			return info;
		}
		template<class... Args,class... Names>
		auto constructor(Names&&... argNames)
			-> std::enable_if_t<std::is_constructible<T, Args...>::value && (sizeof...(Args)==sizeof...(Names) || sizeof...(Names) == 0), TagBuilder>
		{
			auto& info = m_info.constructors.emplace_back();

			// 0引数に対応するために最後尾に空要素を追加している
			StringView names[] = { StringView(argNames)... ,"" };
			Type types[] = { Type::Get<Args>() ...,Type() };

			// 型と名前を登録
			for (s32 i = 0; i < std::size(types) - 1; ++i) {
				auto& arg = info.arguments.emplace_back();
				arg.type = types[i];
				if constexpr (sizeof...(Names) == 0)
					arg.name = getArgumentName(i);
				else
					arg.name = names[i];
			}

			// invokerを登録
			info.invoker = &Create<Args...>;

			return info;
		}
		//! @}

		//! @cond
		//! テンプレートメタプログラミングで関数ポインタの引数型を取得
		template<typename U>
		struct MethodTraits;

		//! 関数ポインタの特殊化
		template<typename OwnerType, typename ReturnType, typename... Args>
		struct MethodTraits<ReturnType(OwnerType::*)(Args...)> {
			static constexpr Array<Type,sizeof...(Args)+1> Types() {
				return { Type::Get<Args>()... ,Type()};
			}
			static constexpr size_t Count() { 
				return sizeof...(Args); 
			}
		};

		template<typename OwnerType, typename ReturnType, typename... Args>
		struct MethodTraits<ReturnType(OwnerType::*)(Args...)const> {
			static constexpr Array<Type, sizeof...(Args) + 1> Types() {
				return { Type::Get<Args>()... ,Type() };
			}
			static constexpr size_t Count() {
				return sizeof...(Args);
			}
		};
		//! @endcond

		//@―---------------------------------------------------------------------------
		//! @brief			メソッド追加
		//@―---------------------------------------------------------------------------
		template<class TMethod,class... Names>
		auto method(StringView name, TMethod function,Names&&... argNames) 
			-> std::enable_if_t<MethodTraits<TMethod>::Count() == sizeof...(Names) || 0 == sizeof...(Names) , TagBuilder>
		{
			using return_type = typename member_function_traits<TMethod>::return_type;

			auto& info = m_info.methods[name];
			info.name = name;
			info.returnType = Type::Get<return_type>();

			StringView names[] = { StringView(argNames)... ,""};
			auto types = MethodTraits<TMethod>::Types();

			for (s32 i = 0; i < std::size(types) - 1; ++i) {
				auto& arg = info.arguments.emplace_back();
				arg.type = types[i];
				if constexpr (std::size(names)-1 == 0)
					arg.name = getArgumentName(i);
				else
					arg.name = names[i];
			}

			return info;
		}

		//@―---------------------------------------------------------------------------
		//! @brief			プロパティ追加(メンバ変数)
		//@―---------------------------------------------------------------------------
		template<class TField>
		TagBuilder field(StringView name, TField T::* address) {
			auto& info = m_info.properties[name];
			info.name = name;
			info.type = Type::Get<TField>();
			info.getter = [=](const ConstAnyReference& owner) {
				return Any(owner.get<T>().*address);
			};
			if constexpr (!std::is_const<std::remove_reference_t<TField>>::value) {
				info.setter = [=](AnyReference& owner, const ConstAnyReference& value) {
					(owner.get<T>().*(address)) = value.get<TField>();
				};
			}
			return info;
		}

		//@―---------------------------------------------------------------------------
		//! @brief			プロパティ追加(Getter)
		//@―---------------------------------------------------------------------------
		template<class F>
		TagBuilder property(StringView name, F getter) {
			using return_type = typename member_function_traits<F>::return_type;
			auto& info = m_info.properties[name];
			info.type = Type::Get<return_type>();
			info.name = name;
			info.getter = [=](const ConstAnyReference& owner) {
				return Any((owner.get<T>().*(getter))());
			};
			return info;
		}

		//@―---------------------------------------------------------------------------
		//! @brief			プロパティ追加(Getter/Setter)
		//@―---------------------------------------------------------------------------
		template<class F1, class F2>
		TagBuilder property(StringView name, F1 getter, F2 setter) {
			using return_type = typename member_function_traits<F1>::return_type;
			auto& info = m_info.properties[name];
			info.type = Type::Get<return_type>();
			info.name = name;
			info.getter = [=](const ConstAnyReference& owner) {
				return Any((owner.get<T>().*(getter))());
			};
			info.setter = [=](AnyReference& owner,const ConstAnyReference& value) {
				(owner.get<T>().*(setter))(value.get<remove_cvr_t<return_type>>());
			};
			return info;
		}

	private:

		//@―---------------------------------------------------------------------------
		//! @brief			引数なしのコンストラクタ
		//@―---------------------------------------------------------------------------
		static Any CreateWithoutArgs([[meybe_unused]] Span<ConstAnyReference>) {
			return Any(std::make_unique<T>());
		}

		//@―---------------------------------------------------------------------------
		//! @brief			引数ありのコンストラクタ
		//@―---------------------------------------------------------------------------
		template<class T,class... Args,size_t ...I>
		static Any CreateImpl(Span<ConstAnyReference> args, std::index_sequence<I...>) {
			return Any(std::make_unique<T>(args[I].get<Args>()...));
		}

		//@―---------------------------------------------------------------------------
		//! @brief			引数ありのコンストラクタ
		//@―---------------------------------------------------------------------------
		template<class... Args>
		static Any Create(Span<ConstAnyReference> args) {

			Type types[] = {Type::Get<Args>()...};
			if (!std::equal(args.begin(), args.end(), std::begin(types), std::end(types), [](const ConstAnyReference& a, const Type& b) {return a.type() == b; })) {
				return {};
			}

			return std::move(CreateImpl<T,Args...>(args,std::make_index_sequence<sizeof...(Args)>()));
		}

	};

}