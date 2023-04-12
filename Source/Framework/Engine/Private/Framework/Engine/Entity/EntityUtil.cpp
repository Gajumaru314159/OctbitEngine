//***********************************************************
//! @file
//! @brief		Entity便利関数
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/EntityUtil.h>
#include <Framework/Engine/Entity.h>

namespace ob::engine {

	//===============================================================
	// EntityAccessor
	//===============================================================
	EntityAccessor::EntityAccessor(Entity* value) :m_entity(value) {}
	EntityAccessor::EntityAccessor(const EntityHandle& value) :m_entity(value.get()) {}
	EntityAccessor::EntityAccessor(Entity& value):m_entity(&value){}
	Entity* EntityAccessor::value()const {return m_entity;}



	//===============================================================
	// EntityUtil
	//===============================================================

	//@―---------------------------------------------------------------------------
	//! @brief		子のハンドルのリストを取得
	//@―---------------------------------------------------------------------------
	Array<EntityHandle> EntityUtil::GetChildHandeles(EntityAccessor src) {
		Array<EntityHandle> handles;
		if (src.value()) {
			for (auto& child : src.value()->getChildren()) {
				handles.emplace_back(child->getHandle());
			}
		}
		return std::move(handles);
	}

}
