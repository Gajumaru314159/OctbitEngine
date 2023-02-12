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
	//! @brief		Entity
	//! @details	
	//@―---------------------------------------------------------------------------
	class Entity {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		Entityを生成
		//@―---------------------------------------------------------------------------
		static Entity* Create();

	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

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
		void removeComponent(Component*);
		//! @brief TypeIdからComponentを取得 
		Component* findComponent(TypeId typeId, s32 index = 0)const;
		//! @brief Comoponentを追加 
		template<class T>T* addComponent(){ return reinterpret_cast<T*>(addComponent(new T)); }
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
