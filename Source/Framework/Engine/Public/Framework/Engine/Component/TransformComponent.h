//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component.h>
#include <Framework/Engine/Entity.h>

namespace ob::engine {

	class TransformComponent;
	OB_EVENT_NOTIFIER(TransformChanged, TransformComponent&);

	//! @brief  Transformコンポーネント
	class TransformComponent :public Component {
	public:

		OB_RTTI();

		TransformComponent(Entity&);

		~TransformComponent() = default;


		//===============================================================
		// イベント
		//===============================================================

		// Transform変更イベント
		void addTransformChangedEvent(TransformChangedHandle&, TransformChangedDelegate);
		// Parent変更イベント
		void addParentChangedEvent(ParentChangedHandle&, ParentChangedDelegate);
		// Child変更イベント
		// Static変更イベント


		//===============================================================
		// アクセス
		//===============================================================
		const Transform&	getLocal()const noexcept { return m_local; }
		void				setLocal(const Transform&);
		const Transform&	getWorld()const noexcept { return m_world; }
		void				setWorld(const Transform&);

		//===============================================================
		// ユーティリティ
		//===============================================================
		void		setLocalPosition(const Vec3&);
		void		setLocalScale(const Vec3&);
		void		setLocalQuaternion(const Quat&);
		void		setLocalRotation(const Rot&);

		const Vec3& getLocalPosition()const;
		const Vec3& getLocalScale()const;
		const Quat& getLocalQuaternion()const;
		Rot			getLocalRotation()const;

		void		setWorldPosition(const Vec3&);
		void		setWorldScale(const Vec3&);
		void		setWorldQuaternion(const Quat&);
		void		setWorldRotation(const Rot&);

		const Vec3& getWorldPosition()const;
		const Vec3& getWorldScale()const;
		const Quat& getWorldQuaternion()const;
		Rot			getWorldRotation()const;

	private:

		void initialize() override;

		void computeLocalTransform();
		void computeWorldTransform();

		void onParentChanged(Entity* oldParent, Entity* newParent);

	private:
		TransformChangedNotifier m_transformChangedNotifier;
		ParentChangedNotifier m_parentChangedNotifier;

		ParentChangedHandle m_hParentChanged;
		TransformComponent* m_parent = nullptr;

		Transform m_local;
		Transform m_world;
	};


	inline const Vec3&	TransformComponent::getLocalPosition()const { return m_local.position; }
	inline const Vec3&	TransformComponent::getLocalScale()const { return m_local.scale; }
	inline const Quat&	TransformComponent::getLocalQuaternion()const { return m_local.rotation; }
	inline Rot			TransformComponent::getLocalRotation()const { return Rot(m_local.rotation); }

	inline const Vec3&	TransformComponent::getWorldPosition()const { return m_world.position; }
	inline const Vec3&	TransformComponent::getWorldScale()const { return m_world.scale; }
	inline const Quat&	TransformComponent::getWorldQuaternion()const { return m_world.rotation; }
	inline Rot			TransformComponent::getWorldRotation()const { return Rot(m_world.rotation); }

}
