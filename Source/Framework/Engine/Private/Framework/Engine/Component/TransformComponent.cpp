//***********************************************************
//! @file
//! @brief		コンポーネント
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component/TransformComponent.h>
#include <Framework/Engine/Reflection/TypeBuilder.h>

namespace ob::engine {
	
	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	TransformComponent::TransformComponent() {
		m_local = Transform::Identity;
		m_world = Transform::Identity;
		//m_parentWorld = Transform::Identity;
	}

	void TransformComponent::setLocal(const Transform& value) {
		m_local = value;
		computeWorldTransform();
	}

	void TransformComponent::setWorld(const Transform& value) {
		m_world = value;
		computeLocalTransform();
	}

	void TransformComponent::setParent(const EntityHandle&) {

	}

	ITransformComponent* TransformComponent::getParentTransform()const noexcept {
		return m_parentTR;
	}

	void TransformComponent::setParentRelative(const EntityHandle&) {

	}


	void TransformComponent::setLocalPosition(const Vec3& value) {
		m_local.position = value;
		computeWorldTransform();
	}
	void TransformComponent::setLocalScale(const Vec3& value) {
		m_local.scale = value;
		computeWorldTransform();
	}
	void TransformComponent::setLocalRotation(const Rot& value) {
		m_local.rotation = value.toQuat();
		computeWorldTransform();
	}

	void TransformComponent::computeLocalTransform() {
		//if (m_parentTM) {
		//	m_local = m_prenttTTM->getWorldTM().inverse() * m_local;
		//} else if (!m_parentActive) {
		//	m_local = m_world;
		//}
		// 変更通知


	}

	void TransformComponent::computeWorldTransform() {
		//if (m_parentTM) {
		//	m_world = m_prenttTTM->getWorldTM() * m_localTM;
		//} else if(!m_parentActive) {
		//	m_world = m_local;
		//}
		// 変更通知
	}

}// namespcae ob

OB_DEFINE_CLASS_TYPE_INFO(ob::engine::TransformComponent) {

	using Type = ::ob::engine::Component;

	tag(TC(""), TC(""));

	function(TC("getEntity"), &Type::getEntity);

};
