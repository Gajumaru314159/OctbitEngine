//***********************************************************
//! @file
//! @brief		コンポーネント
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component.h>
#include <Framework/Engine/Entity.h>

namespace ob::engine {

	class TransformComponent :public Component {
	public:

		OB_COMPONENT(TransformComponent);

		virtual ~TransformComponent() = default;


		//===============================================================
		// イベント
		//===============================================================

		// Transform変更イベント
		// Parent変更イベント
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

}// namespcae ob
