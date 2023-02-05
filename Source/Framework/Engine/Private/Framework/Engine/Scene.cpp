﻿//***********************************************************
//! @file
//! @brief		ワールド
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Scene.h>

namespace ob::engine {

	Ref<Scene> Scene::Create() {
		return nullptr;
	}



	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	Scene::Scene() {

	}
	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	Scene::~Scene() {
		m_name.clear();
	}

	//@―---------------------------------------------------------------------------
	//! @brief		名前取得
	//@―---------------------------------------------------------------------------
	const String& Scene::name()const {
		return m_name;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		親シーン取得
	//@―---------------------------------------------------------------------------
	Scene* Scene::parent()const {
		return m_parent;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		アクティブか
	//@―---------------------------------------------------------------------------
	bool Scene::isActive()const {
		return m_active;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		子シーンを取得
	//@―---------------------------------------------------------------------------
	const SceneList& Scene::children()const {
		return m_children;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		サブシーンを追加
	//@―---------------------------------------------------------------------------
	void Scene::addSubScene(Scene* scene) {
		ScopeLock lock(m_lock);
		m_appendedChildren.emplace_back(scene);
	}

	//@―---------------------------------------------------------------------------
	//! @brief		シーン検索
	//@―---------------------------------------------------------------------------
	Scene* Scene::findScene(StringView name, Recursive recursive) {

		for (auto& child: m_children) {

			if (child->name() == name) {
				return child.get();
			}

			if (recursive) {
				if (auto descendants = child->findScene(name)) {
					return descendants;
				}
			}

		}

		return nullptr;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		エンティティを取得
	//@―---------------------------------------------------------------------------
	const EntityList& Scene::entities()const {
		return m_entities;
	}
	//@―---------------------------------------------------------------------------
	//! @brief		エンティティを追加
	//@―---------------------------------------------------------------------------
	void Scene::addEntity(Entity*) {

	}
	//@―---------------------------------------------------------------------------
	//! @brief		エンティティを検索
	//@―---------------------------------------------------------------------------
	Entity* Scene::findEntity(StringView name, Recursive recursive) {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}

}// namespcae ob
