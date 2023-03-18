﻿//***********************************************************
//! @file
//! @brief		TransformComponentImpl
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component/TransformComponentImpl.h>
#include <Framework/Engine/Entity.h>
#include <Framework/Engine/Reflection/TypeBuilder.h>

namespace ob::engine {
	
	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	TransformComponentImpl::TransformComponentImpl() {
		m_local = Transform::Identity;
		m_world = Transform::Identity;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  初期化
	//@―---------------------------------------------------------------------------
	void TransformComponentImpl::initialize(){
		getEntity().subscribeParentChanged(m_hParentChanged, {*this,&TransformComponentImpl::onParentChanged});
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ローカルTransform設定
	//@―---------------------------------------------------------------------------
	void TransformComponentImpl::setLocal(const Transform& value) {
		m_local = value;
		computeWorldTransform();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ワールドTransform設定
	//@―---------------------------------------------------------------------------
	void TransformComponentImpl::setWorld(const Transform& value) {
		m_world = value;
		computeLocalTransform();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ローカルTransformを計算
	//@―---------------------------------------------------------------------------
	void TransformComponentImpl::computeLocalTransform() {
		if (m_parent) {
			m_local = m_parent->getWorld().inverse() * m_local;
		} else {
			m_local = m_world;
		}
		// 変更通知

	}

	//@―---------------------------------------------------------------------------
	//! @brief  ワールドTransformを計算
	//@―---------------------------------------------------------------------------
	void TransformComponentImpl::computeWorldTransform() {
		if (m_parent) {
			m_world = m_parent->getWorld() * m_local;
		} else {
			m_world = m_local;
		}
		// 変更通知
	}


	//@―---------------------------------------------------------------------------
	//! @brief  親変更イベント
	//@―---------------------------------------------------------------------------
	void TransformComponentImpl::onParentChanged(Entity* oldParent, Entity* newParent) {
		// 親TransformComonentを毎回findComponentしなくていいようにキャッシュ
		if (newParent) {
			m_parent = newParent->findComponent<TransformComponentImpl>();
		} else {
			m_parent = nullptr;
		}
	}





	//@―---------------------------------------------------------------------------
	//! @brief  ローカル座標設定
	//@―---------------------------------------------------------------------------
	void TransformComponent::setLocalPosition(const Vec3& value) {
		auto local = getLocal();
		local.position = value;
		setLocal(local);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ローカルスケール設定
	//@―---------------------------------------------------------------------------
	void TransformComponent::setLocalScale(const Vec3& value) {
		auto local = getLocal();
		local.scale = value;
		setLocal(local);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ローカル回転設定
	//@―---------------------------------------------------------------------------
	void TransformComponent::setLocalRotation(const Rot& value) {
		auto local = getLocal();
		local.rotation = value.toQuat();
		setLocal(local);
	}

}// namespcae ob

OB_DEFINE_CLASS_TYPE_INFO(ob::engine::TransformComponentImpl) {

	using Type = ::ob::engine::Component;

	tag(TC("DisallowMultipleComponent"));

	function(TC("getEntity"), &Type::getEntity);

};