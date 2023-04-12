//***********************************************************
//! @file
//! @brief		シーン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Scene.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief		生成
	//@―---------------------------------------------------------------------------
	Ref<Scene> Scene::Create(StringView name) {

		auto scene = new Scene(name);

		return scene;
	}


	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	Scene::Scene(StringView name) {
		m_name = name;
		if (auto entity = Entity::Create(Format(TC("Root({})"),name))) {
			m_hRootEntity = entity->getHandle();
		}

	}

	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	Scene::~Scene() {
		if (auto entity=m_hRootEntity.get()) {
			entity->requestRelease();
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief		名前取得
	//@―---------------------------------------------------------------------------
	const String& Scene::getName()const {
		return m_name;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		親シーン取得
	//@―---------------------------------------------------------------------------
	Scene* Scene::getParent()const {
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
	const SceneList& Scene::getChildren()const {
		return m_children;
	}

	//@―---------------------------------------------------------------------------
	//! @brief		サブシーンを追加
	//@―---------------------------------------------------------------------------
	void Scene::addSubScene(const Ref<Scene>& child) {
		if (!child)
			return;

		ScopeLock lock(m_lock);

		// TODO シーンを共有できないようにする
		if (child->getParent()) {
			LOG_ERROR("シーンの追加に失敗。登録済みのシーンは登録できません。[child={},this={}]", child->getName(),m_name);
		}

		auto ancestor = m_parent;
		while (ancestor) {
			if (ancestor == child.get()) {
				LOG_ERROR("シーンの追加に失敗。循環を検知しました。 [child={},this={}]", child->getName(), m_name);
			}
			ancestor = ancestor->getParent();
		}

		child->m_parent = this;
		m_children.emplace_back(child);
	}

	//@―---------------------------------------------------------------------------
	//! @brief		シーン検索
	//@―---------------------------------------------------------------------------
	Scene* Scene::findScene(StringView name, Recursive recursive) {

		for (auto& child: m_children) {

			if (child->getName() == name) {
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


	void Scene::setGlobalOffset(Vec3 offset) {
		if (offset == getGlobalOffset())
			return;

		m_localOffset = offset;

		if (m_parent) {
			m_localOffset -= m_parent->getGlobalOffset();
		}

		updateGlobalOffset();
	}

	Vec3 Scene::getGlobalOffset()const {
		if (m_parent) {
			return m_parent->getGlobalOffset() + m_localOffset;
		} else {
			return m_localOffset;
		}
	}

	void Scene::setLocalOffset(Vec3 offset) {
		if (m_localOffset == offset)
			return;
		m_localOffset = offset;

		updateGlobalOffset();
	}
	Vec3 Scene::getLocalOffset()const {
		return m_localOffset;
	}

	void Scene::updateGlobalOffset() {
		// ワールドオフセット変更時の処理
		for (auto& child : m_children) {
			child->updateGlobalOffset();
		}
	}



	//@―---------------------------------------------------------------------------
	//! @brief		エンティティを取得
	//@―---------------------------------------------------------------------------
	const EntityHandleList& Scene::getEntities()const {
		return m_entities;
	}
	//@―---------------------------------------------------------------------------
	//! @brief		エンティティを追加
	//@―---------------------------------------------------------------------------
	void Scene::addEntity(Entity* entity) {
		if (auto root = m_hRootEntity.get()) {
			root->addChild(entity);
			m_entities.emplace_back(entity->getHandle());
		} else {
			LOG_ERROR("ルートウィジェットが削除されています。 [this={}]",m_name);
		}
	}
	//@―---------------------------------------------------------------------------
	//! @brief		エンティティを検索
	//@―---------------------------------------------------------------------------
	Entity* Scene::findEntity(StringView name, Recursive recursive) {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}

}
