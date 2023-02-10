//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Engine/EntityHandle.h>
#include <Framework/Engine/Entity/EntityManager.h>
#include <Framework/Engine/Engine.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief		UUIDから生成
	//@―---------------------------------------------------------------------------
	EntityHandle::EntityHandle(const UUID& uuid) {
		m_uuid = uuid;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		クリア
	//@―---------------------------------------------------------------------------
	void EntityHandle::clear() {
		m_uuid = {};
	}

	//@―---------------------------------------------------------------------------
	//! @brief		Entityを取得
	//@―---------------------------------------------------------------------------
	Entity* EntityHandle::get()const {
		if (auto manager = GEngine->get<EntityManager>()) {
			return manager->find(m_uuid);
		}
		return nullptr;
	}

}// namespcae ob
