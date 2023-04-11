//***********************************************************
//! @file
//! @brief		シーン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Entity.h>
#include <Framework/Engine/Component.h>
#include <Framework/Engine/Component/ComponentFactory.h>
#include <Framework/Engine/Engine.h>
#include <Framework/Engine/Entity/EntityManager.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief		生成
	//@―---------------------------------------------------------------------------
	Entity* Entity::Create(StringView name) {
		if (auto manager = EntityManager::Get()) {
			auto entity = new Entity(name);
			manager->add(*entity);
			return entity;
		}
		return nullptr;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	Entity::Entity(StringView name) {
		m_name = name;
		m_active = false;
		m_visible = false;

		for (s32 i = 100; 0<=i; --i) {

			if (i == 0) {
				LOG_FATAL("Entityの新規UUID割り当てに失敗。");
			}

			EntityHandle handle = UUID::Generate();
			if (auto existance = handle.get()) {
				continue;
			}

			m_handle = handle;
			break;
		}

		OB_DEBUG_CONTEXT(setNotificationSuppression(false));
	}


	//@―---------------------------------------------------------------------------
	//! @brief		名前設定
	//@―---------------------------------------------------------------------------
	const String& Entity::getName()const {
		return m_name;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		名前取得
	//@―---------------------------------------------------------------------------
	void Entity::setName(StringView name) {
		setProperty(m_name, name, TC("Name"));
	}

	//@―---------------------------------------------------------------------------
	//! @brief		コンポーネント追加
	//@―---------------------------------------------------------------------------
	Component* Entity::addComponent(TypeId typeId) {

		if (32 < m_components.size()) {
			LOG_ERROR("コンポーネントの最大数を超えました。 [name={},component={}]",m_name,typeId.name());
			return nullptr;
		}

		if (auto factory = ComponentFactory::Get()) {
			if (auto desc = factory->findCreator(typeId)) {

				// 依存コンポーネントを生成
				for (auto& depType : desc->getDependentComponentTypes()) {
					if (findComponent(depType) == nullptr) {
						if (addComponent(depType) == nullptr) {
							LOG_ERROR("依存するコンポーネントの生成に失敗 [{}=>{}]", typeId.name(), depType.name());
						}
					}
				}

				raisePropertyChanged(TC("Components"));
			}
		} else {
			LOG_WARNING("{}が生成されていません。", TypeId::Get<decltype(this)>().name());
		}

		return nullptr;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		コンポーネント削除
	//@―---------------------------------------------------------------------------
	bool Entity::removeComponent(TypeId typeId,s32 index) {

		// TODO Componentの取得をインターフェイスで行う
		Component* found = nullptr;
		for (auto& component : m_components) {
			if (component->getComponentTypeId() == typeId) {
				if (index <= 0) {
					found = component.get();
					break;
				}
				index--;
			}
		}
		return removeComponent(found);
	}

	//@―---------------------------------------------------------------------------
	//! @brief		コンポーネント削除
	//@―---------------------------------------------------------------------------
	bool Entity::removeComponent(Component* component) {

		for (auto itr = m_components.begin(); itr != m_components.end(); itr++) {
			if (itr->get() == component) {
				m_components.erase(itr);
				raisePropertyChanged(TC("Components"));
				return true;
			}
		}

		return false;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		コンポーネント検索
	//@―---------------------------------------------------------------------------
	Component* Entity::findComponent(TypeId typeId, s32 index)const {
		// TODO Componentの取得をインターフェイスで行う
		s32 count = 0;
		for (auto& component : m_components) {
			if (component->getComponentTypeId() == typeId) {
				if (count == index) {
					return component.get();
				}
				count++;
			}
		}
		return nullptr;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		コンポーネントリスト取得
	//@―---------------------------------------------------------------------------
	const ComponentList& Entity::componets()const {
		return m_components;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		コンポーネント追加
	//@―---------------------------------------------------------------------------
	Component* Entity::addComponent(Component* component) {
		if (component) {
			m_components.emplace_back(component);
			component->initialize();
			raisePropertyChanged("Components");
		}
		return component;
	}

	//@―---------------------------------------------------------------------------
	//! @brief			コンポーネント走査
	//! @param func		走査関数
	//! @param typeId	走査するコンポーネントの型 (TypeId::Invalid()の場合全て走査)
	//@―---------------------------------------------------------------------------
	void Entity::visitComponents(const Delegate<void(Component*)>& func, TypeId typeId)const {
		for (auto& component : m_components) {
			// TODO DynamicCast
			if (component->getComponentTypeId() == typeId || typeId == TypeId::Invalid()) {
				func(component.get());
			}
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief		タグ追加
	//@―---------------------------------------------------------------------------
	void Entity::addTag(StringView tag) {
		m_tags.emplace(tag);
	}


	//@―---------------------------------------------------------------------------
	//! @brief		タグ削除
	//@―---------------------------------------------------------------------------
	void Entity::removeTag(StringView tag) {
		auto found = m_tags.find(tag);
		if (found != m_tags.end()) {
			m_tags.erase(found);
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief		タグを持っているか
	//@―---------------------------------------------------------------------------
	bool Entity::hasTag(StringView tag) {
		return m_tags.count(tag);
	}

	//@―---------------------------------------------------------------------------
	//! @brief		解放予約
	//@―---------------------------------------------------------------------------
	void Entity::requestRelease() {
		if (auto manager = EntityManager::Get()) {
			manager->requestRemove(*this);
		} else {
			LOG_WARNING("EntityManagerが解放済みのため解放予約に失敗しました。");
		}		
	}

	//@―---------------------------------------------------------------------------
	//! @brief		アクティブ設定
	//@―---------------------------------------------------------------------------
	void Entity::setActive(bool value) {
		m_active = value;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		アクティブ取得
	//@―---------------------------------------------------------------------------
	bool Entity::isActive()const {
		return m_active;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		子Entity追加
	//@―---------------------------------------------------------------------------
	void Entity::addChild(Entity* child) {
		if (child) {
			child->setParent(this);
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief		子Entity追加
	//@―---------------------------------------------------------------------------
	void Entity::setParent(Entity* newParent,s32 index) {
		if (m_parent == newParent) {
			OB_NOTIMPLEMENTED();
		}
		Entity* oldParent = nullptr;

		{
			auto ancestor = newParent;
			while (ancestor) {
				if (ancestor == this) {
					LOG_ERROR("Entityの親子設定に失敗。循環を検知しました。 [parent={},this={}]", newParent->getName(), m_name);
				}
				ancestor = ancestor->m_parent.load();
			}
		}

		{
			// ロック
			ScopeLock lock(m_childrenLock);
			if (oldParent = m_parent.load()) {
				oldParent->m_childrenLock.lock();

				// 取り外し
				oldParent->m_children.remove(this);
			}

			if (newParent) {

				// 再設定
				{
					ScopeLock lock(newParent->m_childrenLock);

					if (index < 0)index = newParent->m_children.size();
					update_min<s32>(index, newParent->m_children.size());

					auto pos = newParent->m_children.begin();
					for (s32 i = 0; i < index; ++i)++pos;

					newParent->m_children.insert(pos, this);
				}

			}

			m_parent = newParent;

			if (oldParent) {
				oldParent->m_childrenLock.unlock();
			}
		}

		if(oldParent)oldParent->raisePropertyChanged("Children");
		if(newParent)newParent->raisePropertyChanged("Children");
		raisePropertyChanged(TC("Parent"));

		m_parentChangedNotifier.invoke(oldParent, newParent);
	}

	//@―---------------------------------------------------------------------------
	//! @brief		アクティブ取得
	//@―---------------------------------------------------------------------------
	const List<Entity*>& Entity::getChildren()const {
		return m_children;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		親変更イベントを購読
	//@―---------------------------------------------------------------------------
	void Entity::subscribeParentChanged(ParentChangedHandle& handle, ParentChangedDelegate event) {
		m_parentChangedNotifier.add(handle, event);
	}


	//@―---------------------------------------------------------------------------
	//! @brief		所属シーン取得
	//@―---------------------------------------------------------------------------
	Scene* Entity::getScene()const {
		return m_scene;
	}


	//@―---------------------------------------------------------------------------
	//! @brief		別Entityからのアクセスを許可
	//@―---------------------------------------------------------------------------
	void Entity::raisePropertyChanged(StringView name) {
		NotificationObject::raisePropertyChanged(name);
	}

}// namespcae ob
