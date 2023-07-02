//***********************************************************
//! @file
//! @brief		コンポーネント
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component/TransformComponent.h>
#include <Framework/Engine/Forward.h>

namespace ob::engine {
	
	//@―---------------------------------------------------------------------------
	//! @brief  コンポーネント
	//@―---------------------------------------------------------------------------
	class TransformComponentImpl : public TransformComponent {
	public:

		//OB_RTTI();
		
		TransformComponentImpl();

		// Transform変更イベント
		void addTransformChangedEvent(TransformChangedHandle&, TransformChangedDelegate)override;
		// Parent変更イベント
		void addParentChangedEvent(ParentChangedHandle&, ParentChangedDelegate)override;

		const Transform&	getLocal()const noexcept { return m_local; }
		void				setLocal(const Transform&);

		const Transform&	getWorld()const noexcept { return m_local; }
		void				setWorld(const Transform&);

	private:

		void	initialize() override;

		void computeLocalTransform();
		void computeWorldTransform();

		void onParentChanged(Entity* oldParent, Entity* newParent);
		
	private:

		TransformChangedNotifier m_transformChangedNotifier;
		ParentChangedNotifier m_parentChangedNotifier;

		ParentChangedHandle m_hParentChanged;
		TransformComponent* m_parent=nullptr;

		Transform m_local;
		Transform m_world;

	};
	
}
