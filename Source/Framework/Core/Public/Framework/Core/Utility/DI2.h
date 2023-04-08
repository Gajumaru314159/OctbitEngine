//***********************************************************
//! @file
//! @brief		Dependency Injection
//! @author		Gajumaru
//***********************************************************

// DIシステムはCInjectをもとに実装されています。
//
// CInject
// https://github.com/mjirous/cinject
//
// MIT License
// 
// Copyright(c) 2017 Michal Jirouš
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Exception/Exception.h>
#include <Framework/Core/Template/include.h>
#include <Framework/Core/Reflection/TypeId.h>

//@―---------------------------------------------------------------------------
//! @brief		DependencyInjectionに使用するコンストラクタをマークするためのマクロ
//! @details	DependencyInjectionに使用するコンストラクタをSERVICE_INJECTマクロで囲んでください。
//!				```
//!				SERVICE_INJECT(Sample(SPtr<Clazz> clazz)):m_clazz(clazz){}
//!				```
//@―---------------------------------------------------------------------------
#define SERVICE_INJECT(constructorFunction) \
	using ConstructorTypedef = ::ob::core::di::ConstructorType<constructorFunction>; \
	constructorFunction

namespace ob::core::di {

	// 実装に使用するのは以下の2つのクラスです。
	template<typename ... TService>
	class ServiceBuilder;
	class ServiceContainer;

	// 最大依存数はConstructorFactory::createInstanceで設定します。


	//============================================
	// テンプレートヘルパー
	//============================================

	// false (static_assert型名出力用)
	template<typename T> struct always_false { enum { value = false }; };

	// SPtrか
	template<typename T> struct is_shared_ptr : public std::false_type {};
	template<typename T> struct is_shared_ptr<SPtr<T>> : public std::true_type {};

	// SPtrの要素取得
	template<typename T> struct trim_shared_ptr;
	template<typename T> struct trim_shared_ptr<SPtr<T>> { typedef T type; };
	template<typename T> using  trim_shared_ptr_t = typename trim_shared_ptr<T>::type;

	// Arrayか
	template<typename T> struct is_vector : public std::false_type {};
	template<typename T> struct is_vector<Array<T>> : public std::true_type {};

	// Arrayの要素
	template<typename T> struct trim_vector;
	template<typename T> struct trim_vector<Array<T>> { typedef T type; };
	template<typename T> using  trim_vector_t = typename trim_vector<T>::type;

	// ConstructorTypedef定義用マーカー
	template<typename T> struct ConstructorType { typedef T Type; };

	// ConstructorTypedefをメンバに持つか
	template<typename T> struct has_constructor_injection {
		typedef char true_type[1];
		typedef char false_type[2];

		// メンバにConstructorTypedefを持つかだけチェックできればよいので実装はいらない(呼び出されないため)
		template <typename C> static true_type& check(typename C::ConstructorTypedef*);
		template <typename  > static false_type& check(...);

		static const bool value = sizeof(check<T>(0)) == sizeof(true_type);
	};


	//============================================
	// 例外
	//============================================

	//! @brief 循環依存例外
	class CircularDependencyFoundException : public Exception {
	public:
		explicit CircularDependencyFoundException(const TypeId& type) 
			: Exception(Format(TC("Found circular dependency on object '{}'"), type.name()))
		{}
	};

	//! @brief コンポーネント未登録例外
	class ServiceNotFoundException : public Exception {
	public:
		explicit ServiceNotFoundException(const TypeId& type)
			: Exception(Format(TC("Component for interface '{}' not found"), type.name()))
		{}
	};

	//! @brief 無効な操作
	class InvalidOperationException : public Exception {
	public:
		explicit InvalidOperationException(StringView message)
			: Exception(message)
		{}
	};


	//============================================
	// InjectionContext
	//============================================

	class ServiceContainer;

	//@―---------------------------------------------------------------------------
	//! @brief	依存追加済みの型情報を持つクラス
	//@―---------------------------------------------------------------------------
	class InjectionContext {
	public:
		InjectionContext(ServiceContainer& container, TypeId requesterComponent) :
			m_container(container)
		{
			pushType(requesterComponent);
		}

		~InjectionContext() {
			popType();
		}

		ServiceContainer& getContainer() { 
			return m_container; 
		}

		void pushType(const TypeId& type) {
			m_componentStack.emplace_back(type);
		}

		void popType() {
			m_componentStack.pop_back();
		}

		const Array<TypeId>& getComponentStack() {
			return m_componentStack;
		}

	public:
		InjectionContext(const InjectionContext&) = delete;
		InjectionContext(const InjectionContext&&) = delete;
		void operator=(const InjectionContext&) = delete;
		void operator=(const InjectionContext&&) = delete;
	private:
		ServiceContainer& m_container;
		Array<TypeId> m_componentStack;
	};


	//============================================
	// ContextGuard
	//============================================

	//@―---------------------------------------------------------------------------
	//! @brief	依存の循環をチェックするためのクラス
	//@―---------------------------------------------------------------------------
	class ContextGuard {
	public:
		ContextGuard(InjectionContext* context, const TypeId type) 
			: m_context(context)
			, m_type(type)
		{
			m_context->pushType(type);
		}

		~ContextGuard() {
			m_context->popType();
		}

		void ensureNoCycle() {
			// スタック内に重複あり
			const Array<TypeId>& stack = m_context->getComponentStack();
			for (size_t i = 0; i < stack.size() - 1; ++i) {
				if (stack[i] == stack.back()) {
					throw CircularDependencyFoundException(stack.back());
				}
			}
		}

	private:
		InjectionContext* m_context;
		TypeId m_type;
	};


	//============================================
	// InstanceRetriever
	//============================================

	//@―---------------------------------------------------------------------------
	//! @brief	InstanceRetrieverインターフェイス
	//@―---------------------------------------------------------------------------
	struct IInstanceRetriever {
		IInstanceRetriever(TypeId interfaceType,HashSet<TypeId> castableTypes) : interfaceType(interfaceType), castableTypes(std::move(castableTypes)) {}
		virtual ~IInstanceRetriever() = default;
		TypeId interfaceType;
		HashSet<TypeId> castableTypes;
	};

	//@―---------------------------------------------------------------------------
	//! @brief	SPtr<TInterface>を転送できるIInstanceRetriever
	//@―---------------------------------------------------------------------------
	template<typename TInterface>
	struct InstanceRetriever : IInstanceRetriever {
		InstanceRetriever(HashSet<TypeId> castableTypes):IInstanceRetriever(TypeId::Get<TInterface>(),std::move(castableTypes)) {}
		virtual SPtr<TInterface> forwardInstance(InjectionContext* context) = 0;
	};

	//@―---------------------------------------------------------------------------
	//! @brief	InstanceRetrieverの実装
	//@―---------------------------------------------------------------------------
	template<typename TImpl, typename TInterface, typename TStorage>
	struct CastInstanceRetriever : InstanceRetriever<TInterface> {
		explicit CastInstanceRetriever(SPtr<TStorage> storage, HashSet<TypeId> castableTypes)
			: InstanceRetriever<TInterface>(std::move(castableTypes))
			, m_storage(storage) 
		{}
		//! @brief インスタンスを転送
		SPtr<TInterface> forwardInstance(InjectionContext* context) override {
			return m_storage->getOrCreateInstance(context);
		}
		SPtr<TStorage> m_storage;
	};


	//============================================
	// コンテナ
	//============================================

	template<typename... TArgs>
	class ServiceBuilder;

	//@―---------------------------------------------------------------------------
	//! @brief      コンテナ
	//! @details    コンテナは、インターフェースと実装の間のバインディングを設定するために使用されます。
	//!             ```
	//!             class IFoo{};
	//!             class Foo : public IFoo {};
	//!             
	//!             void smaple(){
	//!                 ServiceContainer container;
	//!                 container.bind<IFoo>().to<Foo>();
	//!                 
	//!                 SPtr<IFoo> foo = container.get<IFoo>();
	//!             }
	//!             ```
	//! @note		宣言順的にファイル後方で実装しています。
	//@―---------------------------------------------------------------------------
	class ServiceContainer {
	public:

		//! @brief デフォルトコンストラクタ
		ServiceContainer() = default;

		//@―---------------------------------------------------------------------------
		//! @brief			親コンテナを指定して構築
		//!	@details		```
		//!					ServiceContainer c;
		//!					c.bind<City>().toSelf().inSingletonScope();
		//!					ServiceContainer child(&c);
		//!					child.bind<Building>().toSelf().inSingletonScope();
		//!					
		//!					auto building = child.get<Building>();
		//!					auto city     = child.get<City>();
		//!					```
		//! @param parent
		//@―---------------------------------------------------------------------------
		explicit ServiceContainer(const ServiceContainer* parent) : m_parent(parent) {}

		//@―---------------------------------------------------------------------------
		//! @brief				バインディング開始
		//! @tparam TInterface	登録するインターフェイス群
		//! @return				ビルダー
		//@―---------------------------------------------------------------------------
		template<typename... TInterface>
		ServiceBuilder<TInterface...> bind();

		//@―---------------------------------------------------------------------------
		//! @brief				依存を解決してインスタンスを取得 (Array<SPtr<T>>版)
		//@―---------------------------------------------------------------------------
		template<typename T>
		std::enable_if_t<
			is_vector<T>::value&&
			is_shared_ptr<typename trim_vector<T>::type>::value &&
			!std::is_reference<T>::value,
			// 戻り値 Array<SPtr<T>>
			Array<typename trim_vector<T>::type>
		>
			get(InjectionContext* context = nullptr);

		//@―---------------------------------------------------------------------------
		//! @brief				依存を解決してインスタンスを取得 (SPtr<T>版)
		//@―---------------------------------------------------------------------------
		template<typename T>
		std::enable_if_t<
			!is_vector<T>::value&&
			is_shared_ptr<T>::value &&
			!std::is_reference<T>::value,
			// 戻り値　SPtr<T>
			T
		>
			get(InjectionContext* context = nullptr);

		//@―---------------------------------------------------------------------------
		//! @brief				依存を解決してインスタンスを取得 (T版)
		//@―---------------------------------------------------------------------------
		template<typename TInterface>
		std::enable_if_t<
			!is_vector<TInterface>::value &&
			!is_shared_ptr<TInterface>::value &&
			!std::is_reference<TInterface>::value,
			// 戻り値 SPtr<T>
			SPtr<TInterface>
		>
			get(InjectionContext* context = nullptr);

	private:
		
		void findInstanceRetrievers(Array<SPtr<IInstanceRetriever>>& instanceRetrievers, const TypeId& type) const;

	private:

		template<typename... TService> friend class ServiceBuilderBase;

		using RetrieverMap = HashMap<TypeId, Array<SPtr<IInstanceRetriever>>>;

		const ServiceContainer* m_parent = nullptr;
		RetrieverMap m_retrievers;
	};


	//============================================
	// コンストラクタファクトリ
	//============================================
	
	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ解決
	//! @details	型Tへの変換関数でコンテナから対象の型を取得する。
	//@―---------------------------------------------------------------------------
	struct CtorArgResolver {
		explicit CtorArgResolver(InjectionContext* context) : m_context(context) {}

		// ポインタ以外をキャスト
		template<typename TCtorArgument, std::enable_if_t<!std::is_pointer<TCtorArgument>::value, int> = 0>
		operator TCtorArgument() {
			return m_context->getContainer().get<TCtorArgument>(m_context);
		}

		InjectionContext* m_context;
	};

	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ解決
	//! @details	T,T&,T*ではないもののみ解決
	//@―---------------------------------------------------------------------------
	template<typename TInstance>
	struct CtorArgResolver1st {
		explicit CtorArgResolver1st(InjectionContext* context) : m_context(context) {}

		//! @brief キャスト演算子
		template<typename TCtorArgument, typename std::enable_if_t<!std::is_same<TCtorArgument, TInstance>::value && !std::is_same<TCtorArgument, TInstance&>::value && !std::is_pointer<TCtorArgument>::value, int> = 0>
		operator TCtorArgument() {
			return m_context->getContainer().get<TCtorArgument>(m_context);
		}

		InjectionContext* m_context;
	};

	//@―---------------------------------------------------------------------------
	//! @brief		ファクトリ基底
	//! @details	使用可能なファクトリがない場合は推測エラーとしてアサートを発生させる。
	//@―---------------------------------------------------------------------------
	template<typename T, class TEnable = void>
	class ConstructorFactory {
		static_assert(always_false<T>::value, "Could not deduce any ConstructorFactory");
	};

	//@―---------------------------------------------------------------------------
	//! @brief		引数なしコンストラクタのためのファクトリ
	//! @details	ConstructorTypedefが未定義かつ生成可能な型用。
	//@―---------------------------------------------------------------------------
	template<typename TInstance>
	class ConstructorFactory<TInstance, std::enable_if_t<!has_constructor_injection<TInstance>::value && std::is_constructible<TInstance>::value>> {
	public:
		SPtr<TInstance> createInstance(InjectionContext* context) {
			return std::make_shared<TInstance>();
		}
	};

	//@―---------------------------------------------------------------------------
	//! @brief		10引数のコンストラクタまで対応したファクトリ
	//! @details	ConstructorTypedefが未定義かつコピーコンストラクタ以外。
	//@―---------------------------------------------------------------------------
	template<typename TInstance>
	class ConstructorFactory<TInstance, std::enable_if_t<!has_constructor_injection<TInstance>::value && !std::is_constructible<TInstance>::value>> {
	public:
		SPtr<TInstance> createInstance(InjectionContext* context) {
			// Resolverの数 = バインド可能な最大の数です。
			return try_instantiate(
				CtorArgResolver(context),
				CtorArgResolver(context),
				CtorArgResolver(context),
				CtorArgResolver(context),
				CtorArgResolver(context),
				CtorArgResolver(context),
				CtorArgResolver(context),
				CtorArgResolver(context),
				CtorArgResolver(context),
				// コピー/ムーブを発生させないResolver
				CtorArgResolver1st<TInstance>(context));
		}

	private:

		//! @brief 与えられた引数で構築
		template<typename TArg, typename TNextArg, typename ... TRestArgs>
		std::enable_if_t<std::is_constructible<TInstance, TArg, TNextArg, TRestArgs ...>::value, SPtr<TInstance>>
			try_instantiate(TArg a1, TNextArg a2, TRestArgs ... args)
		{
			return std::make_shared<TInstance>(a1, a2, args...);
		}

		//! @brief 最後の引数を減らして再トライ
		template<typename TArg, typename TNextArg, typename ... TRestArgs>
		std::enable_if_t<!std::is_constructible<TInstance, TArg, TNextArg, TRestArgs ...>::value, SPtr<TInstance>>
			try_instantiate(TArg a1, TNextArg a2, TRestArgs ... args)
		{
			return try_instantiate(a2, args...);
		}

		//! @brief 1引数で構築可能
		template<typename TArg>
		std::enable_if_t<std::is_constructible<TInstance, TArg>::value, SPtr<TInstance>>
			try_instantiate(TArg arg)
		{
			return std::make_shared<TInstance>(arg);
		}

		//! @brief 1引数でも構築不可
		template<typename TArg>
		std::enable_if_t<!std::is_constructible<TInstance, TArg>::value, SPtr<TInstance>>
			try_instantiate(TArg arg)
		{
			// コンストラクタが複数存在するか、インジェクションに適したコンストラクタが見つかりませんでした。
			// SERVICE_INJECTマクロを使用してコンストラクタを明示的にマークしてください。
			static_assert(always_false<TInstance>::value, "Could not find any suitable constructor for injection. Try explicitly mark the constructor using CINJECT macro");
		}
	};

	template<typename TInstance>
	struct ConstructorInvoker;

	// InjectionContextからコンストラクタ呼び出し
	template<typename TInstance, typename ... TConstructorArgs>
	struct ConstructorInvoker<TInstance(TConstructorArgs...)> {
		static SPtr<TInstance> invoke(InjectionContext* context) {
			ServiceContainer& container = context->getContainer();
			return std::make_shared<TInstance>(container.get<TConstructorArgs>(context)...);
		}
	};

	//@―---------------------------------------------------------------------------
	//! @brief		ConstructorTypedefが定義されているクラスのファクトリ
	//@―---------------------------------------------------------------------------
	// InjectionContextからコンストラクタ呼び出し (CINJECT用)
	template<typename TInstance>
	class ConstructorFactory<TInstance, std::enable_if_t<has_constructor_injection<TInstance>::value>> {
	public:
		SPtr<TInstance> createInstance(InjectionContext* context) {
			return ConstructorInvoker<typename TInstance::ConstructorTypedef::Type>::invoke(context);
		}
	};



	//============================================
	// 関数ファクトリ
	//============================================

	//@―---------------------------------------------------------------------------
	//! @brief		生成関数を指定したファクトリ
	//@―---------------------------------------------------------------------------
	template<typename TInstance>
	struct FunctionFactory {

		using FactoryMethodType = Func<SPtr<TInstance>(InjectionContext*)>;

		FunctionFactory(FactoryMethodType factoryMethod)
			: m_factoryMethod(factoryMethod)
		{}

		SPtr<TInstance> createInstance(InjectionContext* context) {
			return m_factoryMethod(context);
		}

		FactoryMethodType m_factoryMethod;
	};


	//============================================
	// 参照ファクトリ
	//============================================

	//@―---------------------------------------------------------------------------
	//! @brief		参照ファクトリ
	//@―---------------------------------------------------------------------------
	template<typename TInstance>
	struct ReferenceFactory {
		ReferenceFactory(SPtr<TInstance> instance) : m_instance(instance) {}

		SPtr<TInstance> createInstance(InjectionContext* context) {
			return m_instance;
		}

		SPtr<TInstance> m_instance;
	};



	//============================================
	// インスタンス・ストレージ
	//============================================

	//@―---------------------------------------------------------------------------
	//! @brief		TFactoryによるTImplのストレージ
	//@―---------------------------------------------------------------------------
	template<typename TImpl, typename TFactory>
	class InstanceStorage {
	public:
		explicit InstanceStorage(TFactory factory,HashSet<TypeId> castableTypes)
			: m_factory(factory)
			, m_castableTypes(std::move(castableTypes))
		{}

		void setSingleton(bool value) { m_isSingleton = value; }

		virtual SPtr<TImpl> getOrCreateInstance(InjectionContext* context) {
			if (m_isSingleton == false)
				return createInstance(context);
			
			if (m_instance == nullptr)
				m_instance = createInstance(context);

			return m_instance;
		}

		template<class T>
		bool isCastableTo()const {
			return m_castableTypes.count(TypeId::Get<T>());
		}

	private:

		SPtr<TImpl> createInstance(InjectionContext* context) {

			// 型と名前を指定してContextGuardを生成
			ContextGuard guard(
				context, 
				TypeId::Get<TImpl>()
			);

			// 循環チェック
			guard.ensureNoCycle();

			return m_factory.createInstance(context);
		}

	private:
		SPtr<TImpl> m_instance;	// Injectorが参照するインスタンス
		TFactory	m_factory;
		bool		m_isSingleton = false;
		String		m_name;
		HashSet<TypeId> m_castableTypes;
	};


	//============================================
	// ストレージ設定
	//============================================

	//@―---------------------------------------------------------------------------
	//! @brief		InstanceStorageのコンフィグ
	//! @details	インスタンスには、transientとsingletonがあります。
	//!				シングルトンであれば、要求されたときにいつでも同じインスタンスが提供されます。
	//!				そうでない場合は は、常に新しいインスタンスが作成されます。
	//@―---------------------------------------------------------------------------
	template<typename TStorage>
	class StorageConfig {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		explicit StorageConfig(SPtr<TStorage> storage) : m_storage(storage) {}

		//@―---------------------------------------------------------------------------
		//! @brief		シングルトンとして扱うインスタンスを設定
		//@―---------------------------------------------------------------------------
		StorageConfig& inSingletonScope() {
			m_storage->setSingleton(true);
			return *this;
		}

	private:
		SPtr<TStorage> m_storage;
	};

	//@―---------------------------------------------------------------------------
	//! @brief		参照ファクトリー用に特殊化した
	//@―---------------------------------------------------------------------------
	template<typename TInstance>
	class StorageConfig<InstanceStorage<TInstance, ReferenceFactory<TInstance>>> {
	public:
		explicit StorageConfig(SPtr<InstanceStorage<TInstance, ReferenceFactory<TInstance>>> storage) : m_storage(storage) {}
	private:
		SPtr<InstanceStorage<TInstance, ReferenceFactory<TInstance>>> m_storage;
	};



	//============================================
	// ビルダー
	//============================================

	//@―---------------------------------------------------------------------------
	//! @brief		インターフェースと実装の間のバインディングを構築する。
	//!	@details	```
	//!				ServiceContainer c;
	//!				c.bind<IFirst>().to<Implementation>(
	//!				c.bind<IFirst>().toFunction<Cheetah>
	//!				c.bind<IFirst>().toContainer(cheetah
	//!				c.bind<Cheetah>().toSelf();
	//!				```
	//@―---------------------------------------------------------------------------
	template<typename ... TService>
	class ServiceBuilderBase {
	public:

		//! @brief  @cond
		explicit ServiceBuilderBase(ServiceContainer* container) : m_container(container) {}
		//! @brief  @endcond

		//@―---------------------------------------------------------------------------
		//! @brief		TServiceを実装する型TImplにバインド
		//@―---------------------------------------------------------------------------
		template<typename TImpl>
		std::enable_if_t<
			(std::is_base_of_v<TService,TImpl> && ...),
			StorageConfig<InstanceStorage<TImpl, ConstructorFactory<TImpl>>>
		>
			to()
		{
			using InstanceStorageType = InstanceStorage<TImpl, ConstructorFactory<TImpl>>;

			// インスタンスホルダを生成
			auto instanceStorage = std::make_shared<InstanceStorageType>(ConstructorFactory<TImpl>(), createCastableTypes());

			// 登録
			registerType<TImpl, InstanceStorageType, TService...>(instanceStorage);

			return StorageConfig<InstanceStorageType>(instanceStorage);
		}

		//@―---------------------------------------------------------------------------
		//! @brief		TServiceを実装する型TImplを返す関数にバインド
		//@―---------------------------------------------------------------------------
		template<typename TImpl>
		std::enable_if_t<
			(std::is_base_of_v<TService, TImpl> && ...),
			StorageConfig<InstanceStorage<TImpl, FunctionFactory<TImpl>>>
		>		
			toFunction(typename FunctionFactory<TImpl>::FactoryMethodType factoryMethod)
		{
			using InstanceStorageType = InstanceStorage<TImpl, FunctionFactory<TImpl>>;

			// インスタンスホルダを生成
			auto instanceStorage = std::make_shared<InstanceStorageType>(factoryMethod, createCastableTypes());

			// 登録
			registerType<TImpl, InstanceStorageType, TService...>(instanceStorage);

			return StorageConfig<InstanceStorageType>(instanceStorage);
		}

		//@―---------------------------------------------------------------------------
		//! @brief		TServiceを実装する型TImplの参照にバインド
		//@―---------------------------------------------------------------------------
		template<typename TImpl>
		std::enable_if_t<
			(std::is_base_of_v<TService, TImpl> && ...),
			StorageConfig<InstanceStorage<TImpl, ReferenceFactory<TImpl>>>
		>
			toConstant(SPtr<TImpl> instance)
		{
			using InstanceStorageType = InstanceStorage<TImpl, ReferenceFactory<TImpl>>;

			// インスタンスホルダを生成
			auto instanceStorage = std::make_shared<InstanceStorageType>(instance, createCastableTypes());

			// 登録
			registerType<TImpl, InstanceStorageType, TService...>(instanceStorage);

			return StorageConfig<InstanceStorageType>(instanceStorage);
		}

	private:

		//@―---------------------------------------------------------------------------
		//! @brief		変換可能な型のセットを生成
		//@―---------------------------------------------------------------------------
		HashSet<TypeId> createCastableTypes()const {
			Array<TypeId> types{ TypeId::Get<TService>()...};
			HashSet<TypeId> castableTypes;
			for (auto& typeId : types) {
				castableTypes.emplace(typeId);
			}
			return std::move(castableTypes);
		}

		//@―---------------------------------------------------------------------------
		//! @brief		型登録ヘルパ
		//@―---------------------------------------------------------------------------
		template<typename TImpl, typename TStorage, typename TService1, typename TServiceOther, typename ... TRest>
		void registerType(SPtr<TStorage> instanceStorage) {
			addRegistration<TImpl, TStorage, TService1>(instanceStorage);
			registerType<TImpl, TStorage, TServiceOther, TRest...>(instanceStorage);
		}

		//@―---------------------------------------------------------------------------
		//! @brief		型登録ヘルパ(ラスト)
		//@―---------------------------------------------------------------------------
		template<typename TImpl, typename TStorage, typename TService>
		void registerType(SPtr<TStorage> instanceStorage) {
			addRegistration<TImpl, TStorage, TService>(instanceStorage);
		}

		//@―---------------------------------------------------------------------------
		//! @brief		型登録
		//@―---------------------------------------------------------------------------
		template<typename TImpl, typename TStorage, typename TService>
		void addRegistration(SPtr<TStorage> instanceStorage) {

			// TImpl は を実装していない
			static_assert(std::is_convertible<TImpl*, TService*>::value, "No conversion exists from TImpl* to TService*");

			m_container->m_retrievers[TypeId::Get<TService>()]
				.emplace_back(std::make_shared<CastInstanceRetriever<TImpl, TService, TStorage>>(instanceStorage,createCastableTypes()));
		}

	private:
		ServiceContainer* m_container;
	};

	//@―---------------------------------------------------------------------------
	//! @brief		2つ以上のインターフェイスに使用される基本ビルダー
	//! @see		ServiceBuilderBase
	//@―---------------------------------------------------------------------------
	template<typename ... TService>
	class ServiceBuilder : public ServiceBuilderBase<TService...> {
	public:
		//! @cond
		explicit ServiceBuilder(ServiceContainer* container) : ServiceBuilderBase<TService...>(container) {}
		//! @endcond
	};

	//@―---------------------------------------------------------------------------
	//! @brief		toSelf() を可能にする単一コンポーネント登録のための特殊化
	//! @details	実装を直接バインドするために使用します。
	//@―---------------------------------------------------------------------------
	template<typename TService>
	class ServiceBuilder<TService> : public ServiceBuilderBase<TService> {
	public:
		//! @cond
		explicit ServiceBuilder(ServiceContainer* container) : ServiceBuilderBase<TService>(container) {}
		//! @endcond

		//@―---------------------------------------------------------------------------
		//! @brief		実装として登録
		//@―---------------------------------------------------------------------------
		StorageConfig<InstanceStorage<TService, ConstructorFactory<TService>>> toSelf() {
			// 文法はtemplate-dependentを調べてください
			// container.bind<T>().to<T>();と同義です。
			return ServiceBuilderBase<TService>::template to<TService>();
		}
	};



	//============================================
	// コンテナ実装部
	//============================================

	//! @cond

	//@―---------------------------------------------------------------------------
	//! @brief	バインディング開始
	//@―---------------------------------------------------------------------------
	template<typename... TInterface>
	ServiceBuilder<TInterface...> ServiceContainer::bind() {
		return ServiceBuilder<TInterface...>(this);
	}

	//@―---------------------------------------------------------------------------
	//! @brief	Array<SPtr<T>>用
	//@―---------------------------------------------------------------------------
	template<typename T>
	std::enable_if_t<
		is_vector<T>::value&&
		is_shared_ptr<trim_vector_t<T>>::value &&
		!std::is_reference<T>::value,
		Array<trim_vector_t<T>>
	>
		ServiceContainer::get(InjectionContext* context)
	{
		using InterfaceType = trim_shared_ptr_t<trim_vector_t<T>>;

		std::unique_ptr<InjectionContext> contextPtr;

		// 最初のgetではInjecttionContextが指定されていないのでこのタイミングで生成
		if (context == nullptr) {
			contextPtr = std::make_unique<InjectionContext>(*this, TypeId::Get<InterfaceType>());
			context = contextPtr.get();
		}

		// 生成可能なサービスのRetrieverを取得
		Array<SPtr<IInstanceRetriever>> retrievers;
		findInstanceRetrievers(retrievers, TypeId::Get<InterfaceType>());

		// contextからサービスを生成
		Array<SPtr<InterfaceType>> instances;
		for (SPtr<IInstanceRetriever> retriever : retrievers) {

			if (retriever->interfaceType == TypeId::Get<InterfaceType>()) {
				instances.emplace_back(
					reinterpret_cast<InstanceRetriever<InterfaceType>*>(retriever.get())->forwardInstance(context)
				);
			}
		}

		return instances;
	}

	//@―---------------------------------------------------------------------------
	//! @brief	SPtr<T>用
	//@―---------------------------------------------------------------------------
	template<typename T>
	std::enable_if_t<
		!is_vector<T>::value&&
		is_shared_ptr<T>::value &&
		!std::is_reference<T>::value,
		T
	>
		ServiceContainer::get(InjectionContext* context)
	{
		using InterfaceType = trim_shared_ptr_t<T>;

		std::unique_ptr<InjectionContext> contextPtr;

		// 最初のgetではInjecttionContextが指定されていないのでこのタイミングで生成
		if (context == nullptr) {
			contextPtr = std::make_unique<InjectionContext>(*this, TypeId::Invalid());
			context = contextPtr.get();
		}

		// 生成可能なサービスのRetrieverを取得
		Array<SPtr<IInstanceRetriever>> retrievers;
		findInstanceRetrievers(retrievers, TypeId::Get<InterfaceType>());

		// contextからサービスを生成
		for (auto& retriever : retrievers) {
			if (retriever->interfaceType == TypeId::Get<InterfaceType>()) {
				try {
					return reinterpret_cast<InstanceRetriever<InterfaceType>*>(retriever.get())->forwardInstance(context);
				}
				catch (Exception e) {

				}
			}
		}

		return nullptr;
	}

	//@―---------------------------------------------------------------------------
	//! @brief	T用
	//@―---------------------------------------------------------------------------
	template<typename T>
	std::enable_if_t<
		!is_vector<T>::value &&
		!is_shared_ptr<T>::value &&
		!std::is_reference<T>::value,
		SPtr<T>
	>
		ServiceContainer::get(InjectionContext* context)
	{
		return get<SPtr<T>>(context);
	}

	//@―---------------------------------------------------------------------------
	//! @brief	InstanceRetrieversを検索
	//@―---------------------------------------------------------------------------
	inline void ServiceContainer::findInstanceRetrievers(Array<SPtr<IInstanceRetriever>>& instanceRetrievers, const TypeId& type) const {

		auto itr = m_retrievers.find(type);

		// retrieversのArrayに挿入
		if (itr != m_retrievers.end()) {
			auto currentRetrievers = itr->second;
			instanceRetrievers.insert(instanceRetrievers.end(), currentRetrievers.begin(), currentRetrievers.end());
		}

		// 親コンテナからも検索()
		if (m_parent != nullptr) {
			m_parent->findInstanceRetrievers(instanceRetrievers, type);
		}

	}

	//! @endcond
}