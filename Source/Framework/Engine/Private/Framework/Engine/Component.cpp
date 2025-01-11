//***********************************************************
//! @file
//! @brief		コンポーネント
//! @author		Gajumaru
//***********************************************************
#include <Framework/Engine/Component.h>
#include <Framework/Engine/Entity.h>
#include <Framework/Core/Reflection/TypeBuilder.h>

OB_DEFINE_CLASS_INFO(ob::engine::Component) {
	// constructor<ob::engine::Entity&>("entity").desc("コンストラクタ");
}

namespace ob::engine {

	//! @brief  コンストラクタ
	Component::Component(Entity& entity)
		: m_entity(entity)
	{
		OB_DEBUG_CONTEXT(setNotificationSuppression(false));
	}

	//! @brief  デストラクタ
	Component::~Component() = default;

	//! @brief  エンティティ取得
	Entity& Component::getEntity()const {
		return m_entity;
	}

	//! @brief		所属シーン取得
	Scene* Component::getScene()const {
		return getEntity().getScene();
	}

	//! @brief		所属ワールド取得
	World* Component::getWorld()const {
		return getEntity().getWorld();
	}

}