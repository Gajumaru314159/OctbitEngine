//***********************************************************
//! @file
//! @brief		グローバルインスタンス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Template/include.h>

namespace ob::core {

	enum class Entity : u64;

	//@―---------------------------------------------------------------------------
	//! @brief      ComponentManager
	//@―---------------------------------------------------------------------------
	template<class T>
	class ComponentManager {
	public:
		using this_type = ComponentManager;
	public:

		//@―---------------------------------------------------------------------------
		//! @brief      全ての要素を削除する
		//@―---------------------------------------------------------------------------
		void clear() {
			m_components.clear();
			m_entities.clear();
			m_lookup.clear();
		}

		//@―---------------------------------------------------------------------------
		//! @brief      別のマネージャーからコピーする
		//@―---------------------------------------------------------------------------
		void copy(const this_type& other) {
			clear();
			m_components = other.m_components;
			m_entities = other.m_entities;
			m_lookup = other.m_lookup;
		}

		//@―---------------------------------------------------------------------------
		//! @brief      別のマネージャーをマージしてから別のマネージャーをクリアする
		//@―---------------------------------------------------------------------------
		void merge(this_type& other) {
			m_components.reseve(size() + other.size());
			m_entities.reseve(size() + other.size());
			m_lookup.reseve(size() + other.size());

			for (size_t i = 0; i < other.size(); ++i) {
				auto entity = other.m_entities[i];
				m_entities.push_back(entity);
				m_lookup[entity] = m_components.size();
				m_components.push_back(std::move(other.m_components[i]));
			}

			other.clear();
		}

		//@―---------------------------------------------------------------------------
		//! @brief      Entityに対応する要素を生成する
		//@―---------------------------------------------------------------------------
		T& create(Entity entity) {
			OB_ASSERT_EXPR(!contains(entity));
			m_components.emplace_back();
			m_entities.push_back(entity);
			return m_components.back();
		}

		//@―---------------------------------------------------------------------------
		//! @brief      Entityに対応する要素を削除する
		//@―---------------------------------------------------------------------------
		void remove(Entity entity) {
			auto itr = m_lookup.find(entity);
			if (itr != m_lookup.end()) {
				auto index = itr->second;
				auto entity = m_entities[index];
				// 最後尾と入れ替え
				if (index + 1 < m_components.size()) {
					m_components[index] = std::move(components.back());
					m_entities[index] = m_entities.back();
					m_lookup[m_entities[index]] = index;
				}
				// 最後尾を削除
				m_components.pop_back();
				m_entities.pop_back();
				m_lookup.erase(itr);
			}
		}

		//@―---------------------------------------------------------------------------
		//! @brief      Entityに対応する要素が含まれているか
		//@―---------------------------------------------------------------------------
		bool contains(Entity entity)const {
			return m_lookup.find(entity) != m_lookup.end();
		}

		//@―---------------------------------------------------------------------------
		//! @brief      Entityに対応する要素を取得する
		//@―---------------------------------------------------------------------------
		T* at(Entity entity) {
			auto itr = m_lookup.find(entity);
			if (itr == m_lookup.end()) return nullptr;
			return m_components[itr->second];
		}

		//@―---------------------------------------------------------------------------
		//! @brief      Entityに対応する要素を取得する
		//@―---------------------------------------------------------------------------
		const T* at(Entity entity) const {
			auto itr = m_lookup.find(entity);
			if (itr == m_lookup.end()) return nullptr;
			return m_components[itr->second];
		}

		//@―---------------------------------------------------------------------------
		//! @brief      インデックスに対応する要素を取得する
		//@―---------------------------------------------------------------------------
		T* at(size_t index) {
			if (m_components.size() <= index) return nullptr;
			return m_components[index];
		}

		//@―---------------------------------------------------------------------------
		//! @brief      インデックスに対応する要素を取得する
		//@―---------------------------------------------------------------------------
		const T* at(size_t index) const {
			if (m_components.size() <= index) return nullptr;
			return m_components[index];
		}

		//@―---------------------------------------------------------------------------
		//! @brief      登録されているコンポーネント数を取得する
		//@―---------------------------------------------------------------------------
		size_t size()const {
			return m_components.size();
		}

		//@―---------------------------------------------------------------------------
		//! @brief      Entityに対応するインデックスを取得する
		//@―---------------------------------------------------------------------------
		size_t getIndex(Entity entity)const {
			auto itr = m_lookup.find(entity);
			if (itr == m_lookup.end())return ~0ull;
			return itr->second;
		}

		//@―---------------------------------------------------------------------------
		//! @brief      インデックスからエンティティを取得する
		//@―---------------------------------------------------------------------------
		Entity getEntity(size_t index)const {
			return m_entities.at(index);
		}

	private:
		Array<T> m_components;
		Array<Entity> m_entities;
		HashMap<Entity, size_t> m_lookup;
	};

	//@―---------------------------------------------------------------------------
	//! @brief      EntityとManagerを保持するホルダー
	//@―---------------------------------------------------------------------------
	template<class T>
	class ComponentHolder {
	public:
		using manager_type = ComponentManager<T>;
	public:
		ComponentHolder(Entity entity = ~0ull,manager_type* manager=nullptr) 
			: m_entity(entity)
			, m_manager(manager)
		{	
		}
		T* value()const {
			if (m_manager == nullptr)return nullptr;
			return m_manager->at(m_entity);
		}
	private:
		Entity m_entity;
		manager_type* m_manager = nullptr;
	};

}