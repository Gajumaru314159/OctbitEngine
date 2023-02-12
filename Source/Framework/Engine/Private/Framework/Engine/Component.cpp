//***********************************************************
//! @file
//! @brief		コンポーネント
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component.h>
#include <Framework/Engine/Reflection/TypeBuilder.h>

namespace ob::engine {


	Component::~Component() = default;
	Component::Component() = default;

	//@―---------------------------------------------------------------------------
	//! @brief  エンティティ取得
	//@―---------------------------------------------------------------------------
	Entity& Component::getEntity()const {
		OB_ASSERT(m_entity, "EntityにはComponent::startup()前にアクセスできません。");
		return *m_entity;
	}


}// namespcae ob

namespace test {
	enum class Test {
		A,
		B,
	};
}

OB_DEFINE_CLASS_TYPE_INFO(ob::engine::Component) {

	using Type = ::ob::engine::Component;

	tag(TC(""), TC(""));

	function(TC("getEntity"), &Type::getEntity);

};