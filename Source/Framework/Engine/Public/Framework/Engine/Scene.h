//***********************************************************
//! @file
//! @brief		シーン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Utility/Ref.h>
#include <Framework/Engine/Forward.h>
#include <Framework/Engine/Entity.h>

namespace ob::engine {

	using SceneList = List<UPtr<class Scene>>;

	DEFINE_YES_NO(Recursive);
	DEFINE_YES_NO(Async);

	//@―---------------------------------------------------------------------------
	//! @brief  シーン
	//@―---------------------------------------------------------------------------
	class Scene : public RefObject {
	public:

		static Ref<Scene> Create(StringView name);
		//static Ref<Scene> Load();

	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		Scene();
		~Scene();

		void load();

		const String& name()const;
		Scene* parent()const;

		bool isActive()const;

		const SceneList& children()const;
		void addSubScene(Scene*);
		Scene* findScene(StringView name, Recursive recursive = Recursive::No);

		const EntityList& entities()const;
		void addEntity(Entity*);
		Entity* findEntity(StringView name, Recursive recursive = Recursive::No);

	private:

		String		m_name;
		bool		m_active;
		f32			m_timeScale = 1.0f;

		Scene*		m_parent;
		SceneList	m_children;
		EntityList	m_entities;


		SpinLock	m_lock;
		SceneList	m_appendedChildren;

	};

}// namespcae ob
