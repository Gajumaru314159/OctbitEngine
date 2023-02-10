﻿//***********************************************************
//! @file
//! @brief		Entity管理
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Forward.h>
#include <Framework/Engine/IModule.h>
#include <Framework/Engine/EntityHandle.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief  Entity管理
	//@―---------------------------------------------------------------------------
	class EntityManager:public IModule {
	public:
		//@―---------------------------------------------------------------------------
		//! @brief		取得
		//@―---------------------------------------------------------------------------
		static EntityManager& Get();
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		EntityManager();

		//@―---------------------------------------------------------------------------
		//! @brief		デストラクタ
		//@―---------------------------------------------------------------------------
		~EntityManager();

		//@―---------------------------------------------------------------------------
		//! @brief		Entityを管理に追加
		//@―---------------------------------------------------------------------------
		void add(Entity& entity);

		//@―---------------------------------------------------------------------------
		//! @brief		Entityを管理から外す予約
		//@―---------------------------------------------------------------------------
		void remove(const Entity& entity);

		//@―---------------------------------------------------------------------------
		//! @brief		Entityを管理に追加
		//@―---------------------------------------------------------------------------
		Entity* find(const EntityHandle& handle);

		//@―---------------------------------------------------------------------------
		//! @brief		更新
		//@―---------------------------------------------------------------------------
		void update()override;

	private:
		
		Mutex m_lock;
		HashMap<EntityHandle, Entity*> m_entities;
		Array<EntityHandle> m_removeEntities;

	};

}// namespcae ob