//***********************************************************
//! @file
//! @brief		Dependency Injection
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Exception/Exception.h>
#include <Framework/Core/Reflection/TypeId.h>

namespace ob::core {

    constexpr size_t MAX_INJECTION = 16;    // サービスのコンストラクタ引数の最大数

    class ServiceInjector;      // 依存関係定義
    class ServiceContainer;     // 生成ごとのインスタンス管理
    
    template<class T>
    class ServiceBuilder;       // 生成情報登録
    class ServiceBuilderBase;   // 生成情報登録(基底)


    //@―---------------------------------------------------------------------------
    //! @brief  サービス依存注入クラス
    //! @details ```cpp
    //!         ServiceInjector injector;
    //!         injector.bind<Hoge>();
    //!         injector.bind<Fuga>().as<IFuga>();
    //!         ServiceContainer container;
    //!         auto hoge = container.create<Hoge>(container);
    //!         ```
    //@―---------------------------------------------------------------------------
    class ServiceInjector {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  生成可能なクラスTをバインド
        //@―---------------------------------------------------------------------------
        template<class T>
        typename ServiceBuilder<T>& bind();

        //@―---------------------------------------------------------------------------
        //! @brief  サービスを生成
        //! @param container 生成されたサービスを管理させるコンテナの参照
        //@―---------------------------------------------------------------------------
        template<class T>
        T* create(ServiceContainer& container)const;

    private:
        template<class T> friend class ServiceBuilder;  // for m_builders
        HashMap<TypeId, UPtr<ServiceBuilderBase>>   m_builders;
        HashMap<TypeId, Array<TypeId>>          m_builderMap;
    };


    namespace detail {


        // remove_cvr_t
        template<class T>
        using base_type = std::remove_cv_t<std::remove_reference_t<T>>;

        // T(Impl) -> U(Interface) 変換
        template<class T>
        struct arg_resolver {
            const ServiceInjector& injector;
            ServiceContainer& container;

            // コピーコンストラクタの場合無効化
            template<class U> using is_copy_constructor = std::is_same<base_type<T>, base_type<U>>;
            template<class U> using no_copy_constructor = std::enable_if_t<is_copy_constructor<U>::value == false>;

            // 型変換(参照)
            template<class U, class = no_copy_constructor<U>>
            operator U& () const {
                return *injector.create<U>(container);
            }
            // 型変換(ポインタ)
            template<class U, class = no_copy_constructor<U>>
            operator U* () const {
                return injector.create<U>(container);
            }
        };

        // arg_types
        template<class...>
        struct arg_types {};

        // arg_resolver
        template<class T, int... ArgIndex>   // template<class T,int ArgIndex> ?
        using arg_type = arg_resolver<T>;


        // sizeof...(ArgIndex) の引数で構築可能か
        template<class T, int... ArgIndex>
        using can_construct = std::is_constructible<T, arg_type<T, ArgIndex>...>;

        // 構築可能
        template<class T, int... ArgIndex>
        using can_construct_true = std::enable_if_t<can_construct<T, ArgIndex...>::value == true>;

        // 構築不可能
        template<class T, int... ArgIndex>
        using can_construct_false = std::enable_if_t<can_construct<T, ArgIndex...>::value == false>;



        // ベース
        template <class T, class ArgsSequence, class Constructable = void>
        struct constructor {};

        // つくれる場合
        template <class T, int... ArgIndex>
        struct constructor<T, std::index_sequence<ArgIndex...>, can_construct_true<T, ArgIndex...>> {
            using args = arg_types<arg_type<T, ArgIndex>...>;
            //static constexpr size_t arg_size = sizeof...(ArgIndex);
        };

        // つくれない場合
        template <class T, int... ArgIndex>
        struct constructor<T, std::index_sequence<ArgIndex...>, can_construct_false<T, ArgIndex...>> {
            using next = constructor<T, std::make_index_sequence<sizeof...(ArgIndex) - 1>>;
            using args = typename next::args;
        };


        // 構築可能なコンストラクタの引数情報
        // arg_list<T> = arg_types<arg_type<T,0>,...>;
        template<class T>
        using arg_list = typename constructor<T, std::make_index_sequence<MAX_INJECTION>>::args;


        // ファクトリべース
        template<class T, class>
        struct FactoryBase;

        // 引数の数を取り出したファクトリ
        // Args = arg_array<U,0>;
        template<class T, class... Args>
        struct FactoryBase<T, arg_types<Args...>> {
            static void* Create(const ServiceInjector& injector, ServiceContainer& container) {
                return (void*)new T(Args{ injector,container }...);
            }
        };

        // ファクトリ
        template<class T>
        using Factory = FactoryBase<T, arg_list<T>>;

    }


    //@―---------------------------------------------------------------------------
    //! @brief  生成可能なクラスTをバインド
    //@―---------------------------------------------------------------------------
    template<class T>
    ServiceBuilder<T>& ServiceInjector::bind() {
        auto builder = new ServiceBuilder<T>(*this);
        m_builders[TypeId::Get<T>()].reset(builder);
        return *builder;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  サービスを生成
    //! @param container 生成されたサービスを管理させるコンテナの参照
    //@―---------------------------------------------------------------------------
    template<class T>
    T* ServiceInjector::create(ServiceContainer& container)const {
        // 生成済み
        if (auto instance = container.get<T>())
            return instance;
        // 抽象->具象
        Array<TypeId> fallback;
        auto& concretes = try_find(m_builderMap, TypeId::Get<T>(), fallback);
        // 生成
        for (auto& concrete : concretes) {
            auto& builder = m_builders.find(concrete)->second;
            try {
                return reinterpret_cast<T*>(builder->create(container));
            } catch (Exception e) {
                // 生成キャンセル
            }
        }
        return nullptr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      サービスビルダー基底
    //@―---------------------------------------------------------------------------
    class ServiceBuilderBase {
    public:
        virtual ~ServiceBuilderBase() = default;
    private:
        friend class ServiceInjector;
        virtual void* create(ServiceContainer&) = 0;
    };


    //@―---------------------------------------------------------------------------
    //! @brief      サービスビルダー
    //@―---------------------------------------------------------------------------
    template<class T>
    class ServiceBuilder :public ServiceBuilderBase {
    public:
        //@―---------------------------------------------------------------------------
        //! @brief      キャスト可能な基底クラスを追加
        //@―---------------------------------------------------------------------------
        template<class... U>
        ServiceBuilder& as() {
            static_assert((std::is_base_of_v<U, T> && ...), "U must be a base class of T.");
            TypeId types[] = { TypeId::Get<U>() ... };
            for (auto& type : types) {
                m_bases.emplace(type);
                m_injector.m_builderMap[type].emplace_back(TypeId::Get<T>());
            }
            return *this;
        }
    private:
        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //@―---------------------------------------------------------------------------
        ServiceBuilder(ServiceInjector& injector)
            : m_injector(injector)
        {
            m_bases.emplace(TypeId::Get<T>());
            m_injector.m_builderMap[TypeId::Get<T>()].emplace_back(TypeId::Get<T>());
        }
        //@―---------------------------------------------------------------------------
        //! @brief      サービス生成
        //@―---------------------------------------------------------------------------
        void* create(ServiceContainer& container) {
            // 生成
            auto holder = new detail::ServiceHolder<T>(reinterpret_cast<T*>(detail::Factory<T>::Create(m_injector, container)));
            if (holder->get() == nullptr) {
                delete holder;
                return nullptr;
            }
            // 基底クラスをマッピング
            auto index = container.m_services.size();
            for (auto& type : m_bases) {
                container.m_indices.emplace(type, index);
            }
            return container.m_services.emplace_back(holder)->get();
        }
    private:
        friend class ServiceInjector;
        ServiceInjector& m_injector;
        HashSet<TypeId> m_bases;
    };


    namespace detail {

        //@―---------------------------------------------------------------------------
        //! @brief      サービスホルダー基底
        //@―---------------------------------------------------------------------------
        struct ServiceHolderBase {
            virtual ~ServiceHolderBase() = default;
            virtual void* get()const = 0;
        };

        //@―---------------------------------------------------------------------------
        //! @brief      サービスホルダー
        //@―--------------------------------------------------------------------------- 
        template<class T>
        class ServiceHolder : public ServiceHolderBase {
        public:
            ServiceHolder(T* instance) {
                m_instance = instance;
            }
            ~ServiceHolder() {
                if (m_instance) {
                    delete m_instance;
                    m_instance = nullptr;
                }
            }
            void* get()const override {
                return m_instance;
            }
        private:
            T* m_instance = nullptr;
        };

    }


    //@―---------------------------------------------------------------------------
    //! @brief      サービス管理クラス
    //@―---------------------------------------------------------------------------
    class ServiceContainer {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //@―---------------------------------------------------------------------------
        ServiceContainer() = default;

        //@―---------------------------------------------------------------------------
        //! @brief      デストラクタ
        //@―---------------------------------------------------------------------------
        ~ServiceContainer() {
            for (auto itr = m_services.rbegin(); itr != m_services.rend(); ++itr) {
                itr->reset();
            }
        }

        //@―---------------------------------------------------------------------------
        //! @brief      サービス取得
        //@―---------------------------------------------------------------------------
        template<class T>
        T* get()const {
            auto found = m_indices.find(TypeId::Get<T>());
            if (found == m_indices.end()) return nullptr;
            return reinterpret_cast<T*>(m_services.at(found->second)->get());
        }

    private:
        template<class T> friend class ServiceBuilder;
        Array<UPtr<detail::ServiceHolderBase>> m_services;
        HashMap<TypeId, size_t> m_indices;
    };

}