//***********************************************************
//! @file
//! @brief		シーン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Forward.h>
#include <Framework/Engine/Component.h>
#include <Framework/Engine/EntityHandle.h>
#include <Framework/Engine/INotifyPropertyChanged.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief		Entityアクセスを共通化するクラス
	//@―---------------------------------------------------------------------------
	class EntityAccessor{
	public:
		EntityAccessor() = default;
		EntityAccessor(Entity&);
		EntityAccessor(Entity*);
		EntityAccessor(const EntityHandle&);
		Entity* value()const;
	private:
		Entity* m_entity=nullptr;
	};


	//@―---------------------------------------------------------------------------
	//! @brief		Entity便利関数
	//@―---------------------------------------------------------------------------
	class EntityUtil {
	public:

		static Array<EntityHandle> GetChildHandeles(EntityAccessor);

	};

}// namespcae ob
