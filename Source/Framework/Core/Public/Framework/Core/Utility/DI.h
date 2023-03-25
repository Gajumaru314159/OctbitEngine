//***********************************************************
//! @file
//! @brief		Dependency Injection
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Reflection/TypeId.h>

namespace ob::core {

	class ServiceContainer;

	namespace detail {

		//@―---------------------------------------------------------------------------
		//! @brief  サービスを保持するクラス
		//@―---------------------------------------------------------------------------
		class ServiceHolderBase {
		public:
			ServiceHolderBase(TypeId typeId, void* instance) :m_typeId(typeId), m_instance(instance) {}
			virtual ~ServiceHolderBase() {}
			void* get()const { return m_instance; }
		protected:
			TypeId m_typeId;
			void* m_instance = nullptr;
		};

		//@―---------------------------------------------------------------------------
		//! @brief  サービスを保持するクラス
		//@―---------------------------------------------------------------------------
		template<class T>
		class ServiceHolder : public ServiceHolderBase {
		public:
			template<class = std::enable_if_t<std::is_constructible_v<T,ServiceContainer&>>>
			ServiceHolder(ServiceContainer& container) : ServiceHolderBase(TypeId::Get<T>(), new T(container)) {}

			ServiceHolder(T* instance) : ServiceHolderBase(TypeId::Get<T>(), instance), m_constructed(false) {}

			~ServiceHolder() {
				if (m_constructed) {
					delete reinterpret_cast<T*>(m_instance);
				}
				m_instance = nullptr;
			}
		private:
			bool m_constructed = true;
		};



		//@―---------------------------------------------------------------------------
		//! @brief  サービスビルダーの基底
		//@―---------------------------------------------------------------------------
		class ServiceBuilderBase {
		public:
			virtual ~ServiceBuilderBase() = default;
		protected:
			ServiceBuilderBase() = default;
		private:
			virtual UPtr<ServiceHolderBase> create(ServiceContainer&) = 0;
		protected:
			friend class ServiceContainer;
			HashSet<TypeId> m_bases;
			HashSet<TypeId> m_requires;
			HashSet<TypeId> m_optionals;
			HashSet<TypeId> m_dependencies;
		};

	}

	//@―---------------------------------------------------------------------------
	//! @brief  サービスビルダー
	//@―---------------------------------------------------------------------------
	template<class T>
	class ServiceBuilderNew :public detail::ServiceBuilderBase {
	public:
		//@―---------------------------------------------------------------------------
		//! @brief  生成
		//@―---------------------------------------------------------------------------
		ServiceBuilderNew() {}

		//@―---------------------------------------------------------------------------
		//! @brief  デストラクタ
		//@―---------------------------------------------------------------------------
		~ServiceBuilderNew(){}

		//@―---------------------------------------------------------------------------
		//! @brief  サービスのインターフェイスを登録
		//@―---------------------------------------------------------------------------
		template<class U>
		ServiceBuilderNew& as() {
			static_assert(std::is_base_of_v<U, T>, "U must be a base class of T.");
			if (!std::is_same_v<U, T>) {
				m_bases.emplace(TypeId::Get<U>());
			}
			return *this;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  サービスの必須依存先を追加
		//@―---------------------------------------------------------------------------
		template<class T>
		ServiceBuilderNew& require() {
			m_requires.emplace(TypeId::Get<T>());
			m_dependencies.emplace(TypeId::Get<T>());
			return *this;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  サービスの任意依存先を追加
		//@―---------------------------------------------------------------------------
		template<class T>
		ServiceBuilderNew& optional() {
			m_optionals.emplace(TypeId::Get<T>());
			m_dependencies.emplace(TypeId::Get<T>());
			return *this;
		}
	private:
		//@―---------------------------------------------------------------------------
		//! @brief  ホルダーを生成
		//@―---------------------------------------------------------------------------
		UPtr<detail::ServiceHolderBase> create(ServiceContainer& container) {
			return std::make_unique<detail::ServiceHolder<T>>(container);
		}
	};


	//@―---------------------------------------------------------------------------
	//! @brief  サービスビルダー
	//@―---------------------------------------------------------------------------
	template<class T>
	class ServiceBuilderRef :public detail::ServiceBuilderBase {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  生成
		//@―---------------------------------------------------------------------------
		ServiceBuilderRef(T& instance) : m_instance(&instance) {}

		//@―---------------------------------------------------------------------------
		//! @brief  デストラクタ
		//@―---------------------------------------------------------------------------
		~ServiceBuilderRef() {}

		//@―---------------------------------------------------------------------------
		//! @brief  サービスのインターフェイスを登録
		//@―---------------------------------------------------------------------------
		template<class U>
		ServiceBuilderRef& as() {
			static_assert(std::is_base_of_v<U, T>, "U must be a base class of T.");
			if (!std::is_same_v<U, T>) {
				m_bases.emplace(TypeId::Get<U>());
			}
			return *this;
		}

	private:
		//@―---------------------------------------------------------------------------
		//! @brief  ホルダーを生成
		//@―---------------------------------------------------------------------------
		UPtr<detail::ServiceHolderBase> create(ServiceContainer& container) {
			return std::make_unique<detail::ServiceHolder<T>>(m_instance);
		}
	private:
		T* m_instance = nullptr;
	};


	//@―---------------------------------------------------------------------------
	//! @brief      依存グラフ
	//! @details    依存グラフは有向非巡回グラフ（directed acyclic graph, DAG）
	//!             である必要があります。つまり依存が循環してはいけません。
	//@―---------------------------------------------------------------------------
	class DependencyGraph {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  サービスを追加
		//@―---------------------------------------------------------------------------
		template<typename T>
		ServiceBuilderNew<T>& add() {
			auto& builder = m_builders[TypeId::Get<T>()] = std::make_unique<ServiceBuilderNew<T>>();
			return *reinterpret_cast<ServiceBuilderNew<T>*>(builder.get());
		}

		//@―---------------------------------------------------------------------------
		//! @brief  サービスを追加
		//@―---------------------------------------------------------------------------
		template<typename T>
		ServiceBuilderRef<T>& add(T& instance) {
			auto& builder = m_builders[TypeId::Get<T>()] = std::make_unique<ServiceBuilderRef<T>>(instance);
			return *reinterpret_cast<ServiceBuilderRef<T>*>(builder.get());
		}

	private:
		friend class ServiceContainer;
		HashMap<TypeId, UPtr<detail::ServiceBuilderBase>>  m_builders;
	};


	//@―---------------------------------------------------------------------------
	//! @brief  DIサービスを格納するコンテナ
	//@―---------------------------------------------------------------------------
	class ServiceContainer {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  デフォルトコンストラクタ
		//@―---------------------------------------------------------------------------
		ServiceContainer() = default;

		//@―---------------------------------------------------------------------------
		//! @brief  依存グラフを指定して生成
		//@―---------------------------------------------------------------------------
		ServiceContainer(const DependencyGraph& graph) {

			HashMap<TypeId, HashSet<TypeId>> nodes;

			// 全ノードをリストアップ
			for (auto& [typeId, builder] : graph.m_builders) {
				auto& node = nodes[typeId];
				// 依存方向
				for (auto& dependency : builder->m_dependencies) {
					node.emplace(dependency);
					nodes[dependency];
				}
				// as は反対方向
				for (auto& base : builder->m_bases) {
					nodes[base].emplace(typeId);
				}
			}

			// 入力次数をカウント
			HashMap<TypeId, s32> indegree;
			for (auto& [typeId, builder] : graph.m_builders) {
				for (auto& dependency : builder->m_dependencies) {
					indegree[dependency] += 1;
				}
				for (auto& base : builder->m_bases) {
					indegree[typeId] += 1;
				}
			}

			// 誰からも参照されていないノードをキューに追加
			Queue<TypeId> que;
			for (auto& [typeId,node]: nodes) {
				if (indegree[typeId] == 0) {
					que.push(typeId);
				}
			}

			// トポロジカルソート
			Array<TypeId> orders;
			while (que.empty() == false) {
				TypeId typeId = que.front();
				que.pop();

				for (auto& node: nodes[typeId]) {
					indegree[node]--;
					if (indegree[node] == 0) {
						que.push(node);
					}
				}
				orders.push_back(typeId);
			}

			// 構築
			std::reverse(orders.begin(), orders.end());
			for (auto& typeId : orders) {

				auto found = graph.m_builders.find(typeId);
				if (found != graph.m_builders.end()) {

					auto& builder = *(found->second);

					m_indices[typeId] = m_services.size();

					for (auto& asTypeId : builder.m_bases) {
						m_indices[asTypeId] = m_services.size();
					}

					m_services.emplace_back(builder.create(*this));

				}

			}
		}

		//@―---------------------------------------------------------------------------
		//! @brief  デストラクタ
		//@―---------------------------------------------------------------------------
		~ServiceContainer() {
			// 逆順に解放
			std::reverse(m_services.begin(), m_services.end());
			for (auto& service : m_services) {
				service = {};
			}
		}

		//@―---------------------------------------------------------------------------
		//! @brief  取得
		//@―---------------------------------------------------------------------------
		template<class T>
		T* get()const {
			auto found = m_indices.find(TypeId::Get<T>());
			if (found == m_indices.end()) {
				return nullptr;
			}
			return reinterpret_cast<T*>(m_services.at(found->second)->get());
		}

		//@―---------------------------------------------------------------------------
		//! @brief  参照
		//@―---------------------------------------------------------------------------
		template<class T>
		T& ref()const {
			auto instance = get<T>();
			OB_ASSERT(instance, "{}が未生成です。DependencyGraphの設定を確認してください。", TypeId::Get<T>().name());
			return *instance;
		}

	public:
		// コピー禁止
		ServiceContainer(const ServiceContainer&) = delete;
		ServiceContainer& operator=(const ServiceContainer&) = delete;
	private:
		Array<UPtr<detail::ServiceHolderBase>>  m_services;
		HashMap<TypeId, size_t>					m_indices;
	};

}