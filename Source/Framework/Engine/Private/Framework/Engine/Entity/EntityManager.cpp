//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Engine/Entity/EntityManager.h>
#include <Framework/Engine/Entity.h>
#include <Framework/Engine/Engine.h>
#include <Framework/Engine/Component/ComponentFactory.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief		取得
	//@―---------------------------------------------------------------------------
	EntityManager* EntityManager::Get() {
		auto manager = GEngine->get<EntityManager>();
		return manager;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	EntityManager::EntityManager(ComponentFactory& c) {

		m_entities.reserve(10000);

	}

	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	EntityManager::~EntityManager() {
		for (auto& [uuid, entity] : m_entities) {
			delete entity;
		}
		m_entities.clear();
	}

	//@―---------------------------------------------------------------------------
	//! @brief		Entityを管理に追加
	//@―---------------------------------------------------------------------------
	void EntityManager::add(Entity& entity) {
		ScopeLock lock(m_lock);
		// TODO マルチスレッド用のロックなし追加対応
		m_entities[entity.getHandle()] = &entity;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		Entityを管理から外す予約
	//@―---------------------------------------------------------------------------
	void EntityManager::requestRemove(const Entity& entity) {
		ScopeLock lock(m_lock);
		m_removeEntities.emplace_back(entity.getHandle());
	}

	//@―---------------------------------------------------------------------------
	//! @brief		Entityを管理に追加
	//@―---------------------------------------------------------------------------
	Entity* EntityManager::find(const EntityHandle& handle) {
		auto found = m_entities.find(handle);
		if (found != m_entities.end()) {
			return found->second;
		}
		return nullptr;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		更新
	//@―---------------------------------------------------------------------------
	void EntityManager::update() {
		ScopeLock lock(m_lock);
		for (auto& handle : m_removeEntities) {
			auto found = m_entities.find(handle);
			if (found != m_entities.end()) {
				deleteEnitiyRecursively(found->second);
			}
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief		再帰的にウィジェットを削除する
	//@―---------------------------------------------------------------------------
	void EntityManager::deleteEnitiyRecursively(Entity* entity) {

		if (entity) {

			for (auto& child : entity->getChildren()) {
				deleteEnitiyRecursively(child);
			}

			m_entities.erase(entity->getHandle());
			delete entity;

		}

	}

}