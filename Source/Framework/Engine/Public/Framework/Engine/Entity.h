//***********************************************************
//! @file
//! @brief		Entity
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/DynamicCast.h>
#include <Framework/Engine/Forward.h>
#include <Framework/Engine/Component.h>
#include <Framework/Engine/ECSTypes.h>
#include <Framework/Engine/EntityHandle.h>
#include <Framework/Engine/PropertyNotifier.h>

namespace ob::engine {

	// ParentChangedEvent (Entity* oldValue,Entity* newValue)
	OB_EVENT_NOTIFIER(ParentChanged, Entity*, Entity*);

	//! @brief		Entity
	//! @details	
	class Entity:public PropertyNotifier {
	public:

		//! @brief		Entityを生成
		static Entity* Create(StringView name);

	public:

		//===============================================================
		// Info
		//===============================================================

		//! @brief		名前を取得
		const String& getName()const;

		//! @brief		名前を設定
		void setName(StringView);

		//! @brief		EntityHandle を取得
		const EntityHandle& getHandle()const { return m_handle; }

		//! @brief		アクティブ設定
		void setActive(bool);

		//! @brief		アクティブ取得
		bool isActive()const;

		//! @brief		破棄予約
		//! @details	Entityを破棄すると子Entityも再帰的に破棄されます。
		void requestRelease();

		//! @brief		所属シーン取得
		Scene* getScene()const;

		//! @brief		所属ワールド取得
		World* getWorld()const;


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

		//! @brief TypeからComponentを追加 
		Component* addComponent(Type type);
		//! @brief Componentを削除
		bool removeComponent(Type,s32 index);
		//! @brief Componentを削除
		bool removeComponent(Component*);
		//! @brief TypeからComponentを取得 
		Component* findComponent(Type type, s32 index = 0)const;
		//! @brief Comoponentを追加 
		template<class T>T* addComponent();
		//! @brief Comoponentを削除 
		template<class T>bool removeComponent(s32 index = 0) { return removeComponent(Type::Get<T>(),index); }
		//! @brief Componentを取得
		template<class T>T* findComponent(s32 index = 0)const { return reinterpret_cast<T*>(findComponent(Type::Get<T>(), index)); }
		//! @brief 特定の型のComponentに対して処理
		template<class T>void visitComponents(const Delegate<void(const T&)>& func)const { 
			for (auto& component : componens()) {
				if (auto c = DynamicCast<T>(*component)) {
					func(*c);
				}
			}
		}
		//! @brief Componentのリストを取得 
		const ComponentVector& componets()const;


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

		Component* addComponent(Component*,bool withInitialize);

		void visitComponents(const Delegate<void(Component*)>& func,Type)const;


		//! @brief          プロパティ値変更
		//! @param name     プロパティ名
		void raisePropertyChanged(StringView name);

	private:

		String					m_name;
		List<Entity*>			m_children;
		std::atomic<Entity*>	m_parent = nullptr;
		ComponentVector			m_components;
		TagSet					m_tags;
		
		bool					m_active : 1;
		bool					m_visible : 1;

		//LayerMask				m_layerMask;

		EntityHandle			m_handle;

		ParentChangedNotifier	m_parentChangedNotifier;

		World*					m_world = nullptr;
		std::atomic<Scene*>		m_scene = nullptr;
		SpinLock				m_childrenLock;
	};


	template<class T>
	inline T* Entity::addComponent() {
		return reinterpret_cast<T*>(addComponent(Type::Get<T>()));
	}

}
