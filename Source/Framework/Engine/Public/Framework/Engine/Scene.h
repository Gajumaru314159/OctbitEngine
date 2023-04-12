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

	OB_EVENT_NOTIFIER(SceneEvent, const Ref<Scene>&);

	DEFINE_YES_NO(Recursive);
	DEFINE_YES_NO(Async);

	using SceneList = List<Ref<Scene>>;



	//@―---------------------------------------------------------------------------
	//! @brief  シーン
	//! @ref	https://docs.unrealengine.com/4.27/en-US/API/Runtime/Engine/Engine/ULevel/
	//@―---------------------------------------------------------------------------
	class Scene : public RefObject {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		生成
		//@―---------------------------------------------------------------------------
		static Ref<Scene> Create(StringView name);

	public:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		~Scene();

		const String& getName()const;
		Scene* getParent()const;

		bool isActive()const;

		//===============================================================
		// Scene
		//===============================================================
		const SceneList& getChildren()const;
		void addSubScene(const Ref<Scene>&);
		Scene* findScene(StringView name, Recursive recursive = Recursive::No);

		void setGlobalOffset(Vec3 offset);
		Vec3 getGlobalOffset()const;
		void setLocalOffset(Vec3 offset);
		Vec3 getLocalOffset()const;

		//===============================================================
		// Entity
		//===============================================================
		const EntityHandleList& getEntities()const;
		void addEntity(Entity*);
		Entity* findEntity(StringView name, Recursive recursive = Recursive::No);

	private:

		Scene(StringView name);

		void updateGlobalOffset();

	private:

		String		m_name;
		bool		m_active;
		f32			m_timeScale = 1.0f;

		Vec3		m_localOffset;

		Scene*		m_parent = nullptr;
		SceneList	m_children;
		EntityHandleList	m_entities;


		SpinLock	m_lock;

		EntityHandle m_hRootEntity;

	};

}
