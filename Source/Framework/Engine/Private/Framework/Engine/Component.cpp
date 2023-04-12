//***********************************************************
//! @file
//! @brief		コンポーネント
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component.h>
#include <Framework/Engine/Reflection/TypeBuilder.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	Component::Component() {
		OB_DEBUG_CONTEXT(setNotificationSuppression(false));
	}

	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	Component::~Component() = default;

	//@―---------------------------------------------------------------------------
	//! @brief  エンティティ取得
	//@―---------------------------------------------------------------------------
	Entity& Component::getEntity()const {
		OB_ASSERT(m_entity, "EntityにはComponent::initialize()前にアクセスできません。");
		return *m_entity;
	}


}

OB_DEFINE_CLASS_TYPE_INFO(ob::engine::Component) {

	using Type = ::ob::engine::Component;

	tag(TC(""), TC(""));

	function(TC("getEntity"), &Type::getEntity);

};