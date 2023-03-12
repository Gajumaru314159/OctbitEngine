//***********************************************************
//! @file
//! @brief		シーン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Forward.h>
#include <Framework/Engine/Component.h>
#include <Framework/Engine/EntityHandle.h>
#include <Framework/Engine/INotifyPropertyChanged.h>

namespace ob::engine {

	class Scene;
	class Component;

	enum class EntityFlag {
		Active,
		Visible,
	};
	using EntityFlags = BitFlags<EntityFlag>;

	enum class StaticFlag {
		GI,
		Occluder,
		Occludee,
		Batching,
		Navigation,
		ReflectionProbe,
	};
	using StaticFlags = BitFlags<EntityFlag>;



	class Layer {
	public:
		Layer() = default;
		Layer(s32 index);
		s32 index()const;
		StringView name()const;
	public:
		static Layer FromName(StringView name);
	private:
		s32 m_index;
	};

	using ComponentList = List<UPtr<Component>>;

	using EntityHandleList = List<EntityHandle>;


	//@―---------------------------------------------------------------------------
	//! @brief		Entity
	//! @details	
	//@―---------------------------------------------------------------------------
	class Entity:public NotificationObject {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		Entityを生成
		//@―---------------------------------------------------------------------------
		static Entity* Create(StringView name);

	public:


		//@―---------------------------------------------------------------------------
		//! @brief		EntityHandle を取得
		//@―---------------------------------------------------------------------------
		const EntityHandle handle()const { return m_handle; }

		//@―---------------------------------------------------------------------------
		//! @brief		名前を取得
		//@―---------------------------------------------------------------------------
		const String& name()const;

		//@―---------------------------------------------------------------------------
		//! @brief		名前を設定
		//@―---------------------------------------------------------------------------
		void setName(StringView);

		// Transform

		// Component

		//! @brief TypeIdからComponentを追加 
		Component* addComponent(TypeId typeId);
		//! @brief Componentを削除
		bool removeComponent(TypeId,s32 index);
		//! @brief Componentを削除
		bool removeComponent(Component*);
		//! @brief TypeIdからComponentを取得 
		Component* findComponent(TypeId typeId, s32 index = 0)const;
		//! @brief Comoponentを追加 
		template<class T>T* addComponent() { return reinterpret_cast<T*>(addComponent(new T)); }
		//! @brief Comoponentを削除 
		template<class T>bool removeComponent(s32 index = 0) { return removeComponent(TypeId::Get<T>(),index); }
		//! @brief Componentを取得
		template<class T>T* findComponent(s32 index = 0)const { return reinterpret_cast<T*>(findComponent(TypeId::Get<T>(), index)); }
		//! @brief 特定の型のComponentに対して処理
		template<class T>void visitComponents(const Delegate<void(const T&)>& func)const { 
			for (auto& component : componens()) {
				// TODO DynamicCast
				if (component->getTypeId() == TypeId::Get<T>()) {
					func(*reinterpret_cast<T*>(component.get()));
				}
			}
		}
		//! @brief Componentのリストを取得 
		const ComponentList& componets()const;

		// Tag
		
		//! @brief タグを追加
		void addTag(StringView);
		//! @brief タグを削除 
		void removeTag(StringView);
		//! @brief タグを持っているか 
		bool hasTag(StringView);

		// Create

		//@―---------------------------------------------------------------------------
		//! @brief		破棄する
		//! @details	Entityを破棄すると子Entityも再帰的に破棄されます。
		//@―---------------------------------------------------------------------------
		void requestRelease();


		// Activity
		void setActive(bool);
		bool isActive()const;

		// Hierarchy
		void addChild(Entity*);
		void setParent(Entity* newParent,s32 index=-1);

		const List<Entity*>& getChildren()const;


		// Scene
		Scene* getScene()const;

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


		Scene* m_scene = nullptr;
		std::atomic<Entity*>	m_parent = nullptr;

		String					m_name;
		Set<String,std::less<>>	m_tags;
		List<UPtr<Component>>	m_components;
		EntityHandle			m_handle;
		Layer					m_layer;

		SpinLock				m_childrenLock;
		List<Entity*>			m_children;

		bool	m_active : 1;
		bool	m_visible : 1;

	};


	class EntityAccessor{
	public:
		EntityAccessor() = default;
		EntityAccessor(Entity&);
		EntityAccessor(Entity*);
		EntityAccessor(const EntityHandle&);
		Entity* value()const;
	private:
		Entity* m_entity=nullptr;
	};

	class EntityUtil {
	public:

		static Array<EntityHandle> GetChildHandeles(EntityAccessor);

	};

}// namespcae ob
