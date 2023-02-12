//***********************************************************
//! @file
//! @brief		シーン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Forward.h>
#include <Framework/Engine/Component.h>
#include <Framework/Engine/EntityHandle.h>

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

	using EntityList = List<UPtr<class Entity>>;


	//@―---------------------------------------------------------------------------
	//! @brief  シーン
	//@―---------------------------------------------------------------------------
	class Entity {
	public:

		static Entity* Create();

	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		const EntityHandle handle()const { return m_handle; }

		const String& name()const;
		void setName(StringView);

		// Transform

		// Component
		Component* addComponent(TypeId typeId);
		void removeComponent(Component*);
		Component* findComponent(TypeId typeId, s32 index = 0)const;
		template<class T>T* addComponent(){ return reinterpret_cast<T*>(addComponent(new T)); }
		template<class T>T* findComponent(s32 index = 0)const { return reinterpret_cast<T*>(findComponent(TypeId::Get<T>(), index)); }
		template<class T>void visitComponents(const Delegate<void(const T&)>& func)const { 

			for (auto& component : componens()) {
				// TODO DynamicCast
				if (component->getTypeId() == TypeId::Get<T>()) {
					func(*reinterpret_cast<T*>(component.get()));
				}
			}
		}

		const ComponentList& componets()const;

		// Tag
		void addTag(StringView);
		void removeTag(StringView);
		bool hasTag(StringView);

		// Create
		void destroy();
		bool destoyed()const;

		// Activity
		void setActive(bool);
		bool isActive()const;

		// Hierarchy
		void addChild(Entity*);

	private:

		Entity();

		Component* addComponent(Component*);

		void visitComponents(const Delegate<void(Component*)>& func,TypeId)const;


	private:


		Scene* m_scene;
		Entity* m_parent;

		String					m_name;
		Set<String,std::less<>>	m_tags;
		List<UPtr<Component>>	m_components;
		EntityHandle			m_handle;
		Layer					m_layer;

		bool	m_active : 1;
		bool	m_visible : 1;

	};

}// namespcae ob
