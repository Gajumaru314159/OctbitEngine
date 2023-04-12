//***********************************************************
//! @file
//! @brief		Entity
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Forward.h>
#include <Framework/Engine/Component.h>
#include <Framework/Engine/ECSTypes.h>
#include <Framework/Engine/EntityHandle.h>
#include <Framework/Engine/PropertyNotifier.h>

namespace ob::engine {

	// ParentChangedEvent (Entity* oldValue,Entity* newValue)
	OB_EVENT_NOTIFIER(ParentChanged, Entity*, Entity*);

	//@―---------------------------------------------------------------------------
	//! @brief		Entity
	//! @details	
	//@―---------------------------------------------------------------------------
	class Entity:public PropertyNotifier {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		Entityを生成
		//@―---------------------------------------------------------------------------
		static Entity* Create(StringView name);

	public:

		//===============================================================
		// Info
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief		名前を取得
		//@―---------------------------------------------------------------------------
		const String& getName()const;

		//@―---------------------------------------------------------------------------
		//! @brief		名前を設定
		//@―---------------------------------------------------------------------------
		void setName(StringView);

		//@―---------------------------------------------------------------------------
		//! @brief		EntityHandle を取得
		//@―---------------------------------------------------------------------------
		const EntityHandle& getHandle()const { return m_handle; }

		//@―---------------------------------------------------------------------------
		//! @brief		アクティブ設定
		//@―---------------------------------------------------------------------------
		void setActive(bool);

		//@―---------------------------------------------------------------------------
		//! @brief		アクティブ取得
		//@―---------------------------------------------------------------------------
		bool isActive()const;

		//@―---------------------------------------------------------------------------
		//! @brief		破棄予約
		//! @details	Entityを破棄すると子Entityも再帰的に破棄されます。
		//@―---------------------------------------------------------------------------
		void requestRelease();

		//@―---------------------------------------------------------------------------
		//! @brief		所属シーン取得
		//@―---------------------------------------------------------------------------
		Scene* getScene()const;


		//===============================================================
		// Hierarchy
		//===============================================================
		void addChild(Entity*);
		void setParent(Entity* newParent, s32 index = -1);
		const List<Entity*>& getChildren()const;

		void addParentChangedEvent(ParentChangedHandle&, ParentChangedDelegate);

		//===============================================================
		// Component
		//===============================================================

		//! @brief TypeIdからComponentを追加 
		Component* addComponent(TypeId typeId);
		//! @brief Componentを削除
		bool removeComponent(TypeId,s32 index);
		//! @brief Componentを削除
		bool removeComponent(Component*);
		//! @brief TypeIdからComponentを取得 
		Component* findComponent(TypeId typeId, s32 index = 0)const;
		//! @brief Comoponentを追加 
		template<class T>T* addComponent();
		//! @brief Comoponentを削除 
		template<class T>bool removeComponent(s32 index = 0) { return removeComponent(TypeId::Get<T>(),index); }
		//! @brief Componentを取得
		template<class T>T* findComponent(s32 index = 0)const { return reinterpret_cast<T*>(findComponent(TypeId::Get<T>(), index)); }
		//! @brief 特定の型のComponentに対して処理
		template<class T>void visitComponents(const Delegate<void(const T&)>& func)const { 
			for (auto& component : componens()) {
				// TODO DynamicCast
				if (component->getComponentTypeId() == TypeId::Get<T>()) {
					func(*reinterpret_cast<T*>(component.get()));
				}
			}
		}
		//! @brief Componentのリストを取得 
		const ComponentList& componets()const;


		//===============================================================
		// Tag
		//===============================================================
		
		//! @brief タグを追加
		void addTag(StringView);
		//! @brief タグを削除 
		void removeTag(StringView);
		//! @brief タグを持っているか 
		bool hasTag(StringView);

	private:

		Entity(StringView name);

		Component* addComponent(Component*);

		void visitComponents(const Delegate<void(Component*)>& func,TypeId)const;


		//@―---------------------------------------------------------------------------
		//! @brief          プロパティ値変更
		//! @param name     プロパティ名
		//@―---------------------------------------------------------------------------
		void raisePropertyChanged(StringView name);

	private:

		String					m_name;
		List<Entity*>			m_children;
		std::atomic<Entity*>	m_parent = nullptr;
		ComponentList			m_components;
		TagSet					m_tags;
		
		bool					m_active : 1;
		bool					m_visible : 1;

		//LayerMask				m_layerMask;

		EntityHandle			m_handle;

		ParentChangedNotifier	m_parentChangedNotifier;

		std::atomic<Scene*>		m_scene = nullptr;
		SpinLock				m_childrenLock;
	};


	template<class T>
	inline T* Entity::addComponent() {
		return reinterpret_cast<T*>(addComponent(TypeId::Get<T>()));
	}

}
