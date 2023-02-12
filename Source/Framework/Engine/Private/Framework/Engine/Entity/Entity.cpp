//***********************************************************
//! @file
//! @brief		シーン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Entity.h>
#include <Framework/Engine/Component.h>
#include <Framework/Engine/Engine.h>
#include <Framework/Engine/Entity/EntityManager.h>

namespace ob::engine {

	Entity* Entity::Create() {

		if (auto manager = GEngine->get<EntityManager>()) {
			auto entity = new Entity();
			manager->add(*entity);
			return entity;
		}

		return nullptr;
	}


	//===============================================================
	// コンストラクタ / デストラクタ
	//===============================================================

	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------

	const String& Entity::name()const {
		return m_name;
	}

	void Entity::setName(StringView name) {
		m_name = name;
	}

	// Transform

	// Component
	Component* Entity::addComponent(TypeId typeId) {

		// リフレクションで生成
		Component* component = nullptr;

		// TODO 依存順に生成
		if (component) {

			component->m_entity = this;
			component->startup();

		} else {

		}

		return component;
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

	Component* Entity::addComponent(Component* component) {
		if (component) {
			m_components.emplace_back(component);
		}
		return component;
	}

	void Entity::visitComponents(const Delegate<void(Component*)>& func, TypeId typeId)const {
		for (auto& component : m_components) {
			// TODO DynamicCast
			if (component->getTypeId() == typeId) {
				func(component.get());
			}
		}
	}
	void Entity::addChild(Entity*) {

	}
}// namespcae ob
