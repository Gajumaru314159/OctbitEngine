//***********************************************************
//! @file
//! @brief		コンポーネント
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component/TransformComponent.h>

namespace ob::engine {
	
	//@―---------------------------------------------------------------------------
	//! @brief  コンポーネント
	//@―---------------------------------------------------------------------------
	class TransformComponentImpl : public TransformComponent {
	public:

		OB_RTTI();
		
		TransformComponentImpl();

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

		ParentChangedHandle m_hParentChanged;
		TransformComponent* m_parent=nullptr;

		Transform m_local;
		Transform m_world;

	};
	
}// namespcae ob
