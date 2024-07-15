//***********************************************************
//! @file
//! @brief		TransformComponent
//! @author		Gajumaru
//***********************************************************
#include <Framework/Engine/Component/TransformComponent.h>
#include <Framework/Core/Reflection/TypeBuilder.h>

//***********************************************************
//! @file
//! @brief		TransformComponent
//! @author		Gajumaru
//***********************************************************
#include <Framework/Engine/Component/TransformComponent.h>
#include <Framework/Core/Reflection/TypeRegister.h>

OB_DEFINE_CLASS_INFO(ob::engine::TransformComponent) {
	base<ob::engine::Component>();
	constructor<ob::engine::Entity&>("entity").desc("コンストラクタ");
	property("LocalPosition", &T::getLocalPosition, &T::setLocalPosition).desc("ローカル座標を設定");
	property("LocalScale", &T::getLocalScale, &T::setLocalScale).desc("ローカルスケールを設定");
	property("LocalRotation", &T::getLocalRotation, &T::setLocalRotation).desc("ローカル回転を設定");

	property("WorldPosition", &T::getWorldPosition).desc("ワールド座標を取得");
	property("WorldScale", &T::getWorldScale).desc("ワールドスケールを取得");
	property("WorldRotation", &T::getWorldRotation).desc("ワールド回転を取得");

	method("setWorldPosition", &T::setWorldPosition).desc("ワールド座標を設定");
	method("setWorldScale", &T::setWorldScale).desc("ワールドスケールを設定");
	method("setWorldRotation", &T::setWorldRotation).desc("ワールド回転を設定");
}

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	TransformComponent::TransformComponent(Entity& entity)
		: Component(entity)
	{
		m_local = Transform::Identity;
		m_world = Transform::Identity;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  初期化
	//@―---------------------------------------------------------------------------
	void TransformComponent::initialize(){
		getEntity().addParentChangedEvent(m_hParentChanged, {*this,&TransformComponent::onParentChanged});
	}

	//@―---------------------------------------------------------------------------
	//! @brief  Transform変更イベント
	//@―---------------------------------------------------------------------------
	void TransformComponent::addTransformChangedEvent(TransformChangedHandle& handle, TransformChangedDelegate func) {
		m_transformChangedNotifier.add(handle, func);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  Parent変更イベント
	//@―---------------------------------------------------------------------------
	void TransformComponent::addParentChangedEvent(ParentChangedHandle& handle, ParentChangedDelegate func) {
		m_parentChangedNotifier.add(handle, func);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ローカルTransform設定
	//@―---------------------------------------------------------------------------
	void TransformComponent::setLocal(const Transform& value) {
		m_local = value;
		computeWorldTransform();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ワールドTransform設定
	//@―---------------------------------------------------------------------------
	void TransformComponent::setWorld(const Transform& value) {
		m_world = value;
		computeLocalTransform();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ローカルTransformを計算
	//@―---------------------------------------------------------------------------
	void TransformComponent::computeLocalTransform() {
		if (m_parent) {
			m_local = m_parent->getWorld().inverse() * m_local;
		} else {
			m_local = m_world;
		}
		// 変更通知
		m_transformChangedNotifier.invoke(*this);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ワールドTransformを計算
	//@―---------------------------------------------------------------------------
	void TransformComponent::computeWorldTransform() {
		if (m_parent) {
			m_world = m_parent->getWorld() * m_local;
		} else {
			m_world = m_local;
		}
		// 変更通知
		m_transformChangedNotifier.invoke(*this);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  親変更イベント
	//@―---------------------------------------------------------------------------
	void TransformComponent::onParentChanged(Entity* oldParent, Entity* newParent) {
		// 親TransformComonentを毎回findComponentしなくていいようにキャッシュ
		if (newParent) {
			m_parent = newParent->findComponent<TransformComponent>();
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
	void TransformComponent::setLocalQuaternion(const Quat& value) {
		auto local = getLocal();
		local.rotation = value;
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

	//@―---------------------------------------------------------------------------
	//! @brief  ワールド座標設定
	//@―---------------------------------------------------------------------------
	void TransformComponent::setWorldPosition(const Vec3& value) {
		auto world = getWorld();
		world.position = value;
		setWorld(world);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ワールドスケール設定
	//@―---------------------------------------------------------------------------
	void TransformComponent::setWorldScale(const Vec3& value) {
		auto world = getWorld();
		world.scale = value;
		setWorld(world);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ワールド回転設定
	//@―---------------------------------------------------------------------------
	void TransformComponent::setWorldQuaternion(const Quat& value) {
		auto world = getWorld();
		world.rotation = value;
		setWorld(world);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  ワールド回転設定
	//@―---------------------------------------------------------------------------
	void TransformComponent::setWorldRotation(const Rot& value) {
		auto world = getWorld();
		world.rotation = value.toQuat();
		setWorld(world);
	}

}