//***********************************************************
//! @file
//! @brief		コンポーネント
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component.h>
#include <Framework/Engine/EntityHandle.h>

namespace ob::engine {
	
	class ITransformComponent {
	public:
		virtual ~ITransformComponent() = default;

		// Transform変更イベント
		// Parent変更イベント
		// Child変更イベント
		
		virtual const Transform&	getLocal()const noexcept = 0;
		virtual void				setLocal(const Transform&) = 0;
		virtual const Transform&	getWorld()const noexcept = 0;
		virtual void				setWorld(const Transform&) = 0;

		//virtual void				setWorldPosition(const Vec3&) = 0;
		//virtual const Vec3&			getWorldPosition()const = 0;

		// 
		//virtual void move(const Vec3&){}

		virtual const EntityHandle& getParent()const noexcept = 0;
		virtual ITransformComponent*getParentTransform()const noexcept = 0;
		virtual void				setParent(const EntityHandle&) = 0;
		virtual void				setParentRelative(const EntityHandle&) = 0;

		//virtual Array<EntityHandle> getChildren();
		//virtual Array<EntityHandle> getAllDescendants();
		//virtual Array<EntityHandle> getEntityAndAllDescendants();
	};



	class ITransformNotification {
	public:
		using TransformChanged = EventNotifier<>;
		using ParentChanged = EventNotifier<>;
		using ChildChanged = EventNotifier<>;
	public:

		virtual TransformChanged::Proxy getTransformChangedEvent() = 0;
		virtual ParentChanged::Proxy getParentChangedEvent() = 0;
		virtual ChildChanged::Proxy getChildChangedEvent() = 0;

		// Transform変更イベント
		// Parent変更イベント
		// Child変更イベント
		// Static変更イベント

	};


	//@―---------------------------------------------------------------------------
	//! @brief  コンポーネント
	//@―---------------------------------------------------------------------------
	class TransformComponent
		: public Component
		, public ITransformComponent {
		friend class Entity;
	public:

		OB_RTTI();
		
		TransformComponent();

		const Transform&	getLocal()const noexcept { return m_local; }
		void				setLocal(const Transform&);

		const Transform&	getWorld()const noexcept { return m_local; }
		void				setWorld(const Transform&);

		const EntityHandle& getParent()const noexcept { return m_parent; }
		void				setParent(const EntityHandle&);
		ITransformComponent* getParentTransform()const noexcept override;
		void				setParentRelative(const EntityHandle&);

	public:

		void	setLocalPosition(const Vec3&);
		void	setLocalScale(const Vec3&);
		void	setLocalRotation(const Rot&);

	private:

		void computeLocalTransform();
		void computeWorldTransform();
		
	private:

		// 変更イベント？
		EntityHandle m_parent;
		ITransformComponent* m_parentTR=nullptr;
		Transform m_local;
		Transform m_world;

	};
	
}// namespcae ob
