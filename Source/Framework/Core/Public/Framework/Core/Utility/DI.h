//***********************************************************
//! @file
//! @brief		Dependency Injection
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Reflection/TypeId.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  Dependency Injection
	//@―---------------------------------------------------------------------------
	class DI {
	public:
		using Getter = std::function<void* ()>;
	public:


		DI(){}

		~DI() {
			// 生成の逆順にしておく
			std::reverse(m_instances.begin(), m_instances.end());
		}

		//@―---------------------------------------------------------------------------
		//! @brief  サービスを追加
		//@―---------------------------------------------------------------------------
		template<class Base, class Derived>
		void add() {
			addImpl<Base, Derived>(getServiceCreator<Base, Derived>());
		}

		//@―---------------------------------------------------------------------------
		//! @brief  参照サービスを追加
		//@―---------------------------------------------------------------------------
		template<class T>
		void add(T& instance) {
			auto ptr = &instance;
			Getter getter = [ptr] {
				return ptr;
			};
			addImpl<T>(getter);
		}

		//@―---------------------------------------------------------------------------
		//! @brief  依存関係を解決してサービスを生成
		//@―---------------------------------------------------------------------------
		void build() {
			m_locked = true;
			for (auto& [typeId, getters] : m_getters) {
				buildService(typeId);
			}
		}

		//@―---------------------------------------------------------------------------
		//! @brief  サービスを取得
		//@―---------------------------------------------------------------------------
		template<class T>
		T& get() const {
			auto typeId = TypeId::Get<T>();
			if (!m_locked) {
				LOG_FATAL("DI::build()前にサービスは取得できません。 [{}]", typeId.name());
			}

			auto found = m_services.find(typeId);
			if (m_found == m_services.end() || m_found) {
				OB_ASSERT(pointer, "DIコンテナに登録されていないクラスです。[{}]", TypeId::Get<T>().name());
			}

			return *reinterpret_cast<T*>(found->second);
		}

	private:

		// ベース
		struct Holder {
			virtual ~Holder() = default;
			virtual void* get()const=0;
		};		

		// UPtr用
		template<class T>
		struct HolderTemplate{
			template<class... Args>
			HolderTemplate(Args&&... args) {
				m_instance = std::make_unique<T>(std::forward(args...));
			}
			void* get()const override { return m_instance.get(); }
		private:
			UPtr<T> m_instance;
		};

	private:

		//@―---------------------------------------------------------------------------
		//! @brief  サービス取得関数を直接追加
		//@―---------------------------------------------------------------------------
		template<class Base, class Derived = Base>
		void addImpl(Getter getter) {
			static_assert(std::is_base_of<Base, Derived>());

			if (m_locked) {
				LOG_WARNING("ビルド済みのDIには取得関数を登録できません。[{}]", TypeId::Get<Derived>().name());
				return;
			}

			if (!getter) {
				LOG_WARNING("DIに空の取得関数を登録しようとしました。登録をスキップします。[{}]", TypeId::Get<Derived>().name());
				return;
			}

#if 0
			Array<TypeId> dependencies = Derived::GetDIDependency();
#endif
			auto typeId = TypeId::Get<Base>();
			auto& getters = m_getters[typeId];
			getters.emplace_back(std::move(getter));
		}

		//@―---------------------------------------------------------------------------
		//! @brief  サービスを取得
		//@―---------------------------------------------------------------------------
		void buildService(TypeId typeId){

			// 生成済み
			if (m_services.count(typeId)) {
				return;
			}

			// 取得関数から取得
			auto found = m_getters.find(typeId);
			if (found != m_getters.end()) {
				auto& getters = found->second;
				for (auto& getter : getters) {
					if (auto instance = getter()) {
						m_services[typeId] = instance;
						return;
					}
				}
			}

			LOG_FATAL("DIに使用可能な取得関数が登録されていません。 [{}]", typeId.name());
		}

		//@―---------------------------------------------------------------------------
		//! @brief  サービス
		//@―---------------------------------------------------------------------------
		template<class Base,class Derived>
		Getter getServiceCreator() {
			Getter creator = [this]() {

				auto typeId = TypeId::Get<Base>();

				// 生成済み
				auto found = m_indices.find(typeId);
				if (found == m_indices.end()) {
					return m_instances.at(found->second)->get();
				}
				
				// 生成
				// TODO 依存順を解決する
				auto holder = std::make_unique<HolderTemplate<Derived>>();
				auto ptr = holder->get();

				// マップに登録
				m_indices[typeId] = m_instances.size();
				m_instances.emplace_back(std::move(holder));

				return ptr;
			};
			return Getter;
		}

	private:

		HashMap<TypeId, void*>			m_services;

		HashMap<TypeId, Array<Getter>>	m_getters;
		HashMap<TypeId, size_t>			m_indices;
		Array<UPtr<Holder>>				m_instances;
		bool							m_locked = false;
	};






	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond
	//! 

	//! @endcond
}// namespcae ob