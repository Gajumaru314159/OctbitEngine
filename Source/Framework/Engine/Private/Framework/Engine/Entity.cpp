//***********************************************************
//! @file
//! @brief		シーン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Entity.h>
#include <Framework/Engine/Component.h>

namespace ob::engine {

	Entity* Entity::Create() {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}


	//===============================================================
	// コンストラクタ / デストラクタ
	//===============================================================

	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------

	const UUID Entity::uuid()const {
		return m_uuid;
	}

	const String& Entity::name()const {
		return m_name;
	}

	void Entity::setName(StringView name) {
		m_name = name;
	}

	// Transform

	// Component
	void Entity::addComponent(TypeId typeId) {
		OB_NOTIMPLEMENTED();
	}
	void Entity::removeComponent(Component* component) {
		//auto found = std::find(m_components.begin(), m_components.end(), component);
		//if (found != m_components.end()) {
		//	OB_NOTIMPLEMENTED();
		//}
	}

	Component* Entity::findComponent(TypeId typeId, s32 index)const {
		s32 count = 0;
		for (auto& component : m_components) {
			if (component->getTypeId() == typeId) {
				if (count == index) {
					return component.get();
				}
				count++;
			}
		}
		return nullptr;
	}

	const ComponentList& Entity::componets()const {
		return m_components;
	}

	// Tag
	void Entity::addTag(StringView tag) {
		m_tags.emplace(tag);
	}
	void Entity::removeTag(StringView tag) {
		auto found = m_tags.find(tag);
		if (found != m_tags.end()) {
			m_tags.erase(found);
		}
	}
	bool Entity::hasTag(StringView tag) {
		return m_tags.count(tag);
	}

	// Create
	void Entity::destroy() {
		OB_NOTIMPLEMENTED();
	}
	bool Entity::destoyed()const {
		OB_NOTIMPLEMENTED();
		return false;
	}

	// Activity
	void Entity::setActive(bool value) {
		m_active = value;
	}
	bool Entity::isActive()const {
		return m_active;
	}

	Entity::Entity() {
		m_active = false;
		m_visible = false;
	}

	void Entity::visitComponents(const Delegate<void(Component*)>& func, TypeId typeId)const {
		for (auto& component : m_components) {
			// TODO DynamicCast
			if (component->getTypeId() == typeId) {
				func(component.get());
			}
		}
	}

}// namespcae ob
