//***********************************************************
//! @file
//! @brief		シーン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/TypeInfoManager.h>
#include <Framework/Engine/Entity.h>
#include <Framework/Engine/Component.h>
#include <Framework/Engine/Engine.h>
#include <Framework/Engine/Entity/EntityManager.h>
#include <Framework/Core/Reflection/TypeBuilder.h>

OB_DEFINE_CLASS_INFO(ob::engine::Entity) {
	property("Active", &T::isActive, &T::setActive).desc("アクティブ");
	property("Name", &T::getName, &T::setName).desc("名前");
}


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
		setProperty(m_name, name, "Name");
	}

	//@―---------------------------------------------------------------------------
	//! @brief		コンポーネント追加
	//@―---------------------------------------------------------------------------
	Component* Entity::addComponent(Type type) {

		if (auto manager = TypeInfoManager::Get()) {
			if (auto info = manager->find(type)) {
				if (info->isSuperClassOf<Component>()) {
					if (auto ctor = info->findConstructor<Entity>()) {
						// 依存コンポーネントを生成
						if (auto requirements = info->findTag("Requirements")) {
							// TODO 循環参照検知
							addComponent(Type(*requirements));
						}

						// 生成
						if (auto component = ctor->invoke<Component>(*this)) {
							auto result = component.get();
							m_components.emplace_back(std::move(component));
							raisePropertyChanged("Components");
							return result;
						} else {
							LOG_WARNING("{}のコンストラクタが失敗しました。", type.name());
						}
					} else {
						LOG_WARNING("{}のコンストラクタが見つかりません。", type.name());
					}
				} else {
					LOG_WARNING("{}はComponentではありません。", type.name());
				}
			} else {
				LOG_WARNING("{}がTypeInfoManagerに登録されていません。", type.name());
			}
		} else {
			LOG_WARNING("TypeInfoManagerが初期化されていません。");
		}

		LOG_WARNING("{}をComponentとして追加できませんでした。", type.name());

		return nullptr;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		コンポーネント削除
	//@―---------------------------------------------------------------------------
	bool Entity::removeComponent(Type type,s32 index) {

		// TODO Componentの取得をインターフェイスで行う
		Component* found = nullptr;
		for (auto& component : m_components) {
			if (component->getComponentType() == type) {
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
				raisePropertyChanged("Components");
				return true;
			}
		}

		return false;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		コンポーネント検索
	//@―---------------------------------------------------------------------------
	Component* Entity::findComponent(Type type, s32 index)const {
		// TODO Componentの取得をインターフェイスで行う
		s32 count = 0;
		for (auto& component : m_components) {
			if (component->getComponentType() == type) {
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
	const ComponentVector& Entity::componets()const {
		return m_components;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		コンポーネント追加
	//@―---------------------------------------------------------------------------
	Component* Entity::addComponent(Component* component, bool withInitialize) {
		// TODO initializeの呼び出しフローを考える
		//if (component) {
		//	m_components.emplace_back(component);
		//	if (withInitialize) {
		//		component->initialize();
		//	}
		//	raisePropertyChanged("Components");
		//}
		return component;
	}

	//@―---------------------------------------------------------------------------
	//! @brief			コンポーネント走査
	//! @param func		走査関数
	//! @param type	走査するコンポーネントの型 (Type::Invalid()の場合全て走査)
	//@―---------------------------------------------------------------------------
	void Entity::visitComponents(const Delegate<void(Component*)>& func, Type type)const {
		for (auto& component : m_components) {
			// TODO DynamicCast
			if (component->getComponentType() == type || type == Type::Invalid()) {
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
		raisePropertyChanged("Parent");

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
	void Entity::addParentChangedEvent(ParentChangedHandle& handle, ParentChangedDelegate event) {
		m_parentChangedNotifier.add(handle, event);
	}


	//@―---------------------------------------------------------------------------
	//! @brief		所属シーン取得
	//@―---------------------------------------------------------------------------
	Scene* Entity::getScene()const {
		return m_scene;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		所属ワールド取得
	//@―---------------------------------------------------------------------------
	World* Entity::getWorld()const {
		return m_world;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		別Entityからのアクセスを許可
	//@―---------------------------------------------------------------------------
	void Entity::raisePropertyChanged(StringView name) {
		PropertyNotifier::raisePropertyChanged(name);
	}

}
