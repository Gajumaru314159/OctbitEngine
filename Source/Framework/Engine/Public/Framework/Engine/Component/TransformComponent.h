//***********************************************************
//! @file
//! @brief		Transformコンポーネント
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component.h>
#include <Framework/Engine/Entity.h>

namespace ob::engine {

	class TransformComponent;
	OB_EVENT_NOTIFIER(TransformChanged, TransformComponent&);

	//@―---------------------------------------------------------------------------
	//! @brief  Transformコンポーネント
	//@―---------------------------------------------------------------------------
	class TransformComponent :public Component {
	public:

		OB_COMPONENT(TransformComponent);

		virtual ~TransformComponent() = default;


		//===============================================================
		// イベント
		//===============================================================

		// Transform変更イベント
		virtual void addTransformChangedEvent(TransformChangedHandle&, TransformChangedDelegate) = 0;
		// Parent変更イベント
		virtual void addParentChangedEvent(ParentChangedHandle&, ParentChangedDelegate) = 0;
		// Child変更イベント
		// Static変更イベント


		//===============================================================
		// アクセス
		//===============================================================
		virtual const Transform&	getLocal()const noexcept = 0;
		virtual void				setLocal(const Transform&) = 0;
		virtual const Transform&	getWorld()const noexcept = 0;
		virtual void				setWorld(const Transform&) = 0;

		//===============================================================
		// ユーティリティ
		//===============================================================
		void	setLocalPosition(const Vec3&);
		void	setLocalScale(const Vec3&);
		void	setLocalRotation(const Rot&);

		//void		setWorldPosition(const Vec3&);
		//const Vec3& getWorldPosition()const;

		// 
		//virtual void move(const Vec3&){}

	};

}
