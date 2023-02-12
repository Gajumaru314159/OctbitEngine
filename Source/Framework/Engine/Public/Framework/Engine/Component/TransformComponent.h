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

	template<typename... Args>
	class EventProxy {
		using notifier_type = EventNotifier <Args...>;                                  //!< 型

		using delegate_type = typename notifier_type::delegate_type;                          //!< デリゲート型
		using function_type = typename notifier_type::function_type;                                        //!< 関数型
		using method_type = typename notifier_type::mothod_type;                //!< メンバ関数ポインタ型
		using const_method_type = typename notifier_type::const_method_type;     //!< constメンバ関数ポインタ型

		using Handle = typename HandleList<delegate_type>::Handle;                  //!< イベントハンドル型
	public:

		EventProxy(notifier_type& notifier):m_notifier(notifier){}

		template<class TFunc>
		void add(Handle& handle, TFunc func) { m_notifier.add(handle,func); }

	private:
		notifier_type& m_notifier;
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
