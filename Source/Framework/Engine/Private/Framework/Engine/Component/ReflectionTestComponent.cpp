//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Engine/Component/ReflectionTestComponent.h>
#include <Framework/Core/Reflection/TypeBuilder.h>

OB_DEFINE_CLASS_INFO(ob::engine::ReflectionTestComponent) {

	desc("プロパティのリフレクション登録を検証するクラス");
	tag("Requirements", "ob::engine::TransformComponent");

	base<ob::engine::Component>();
	constructor<ob::engine::Entity&>("entity").desc("コンストラクタ");

	field("m_bool", &T::m_bool);
	field("m_s32", &T::m_s32);
	field("m_s16", &T::m_s16);
	field("m_s8", &T::m_s8);
	field("m_u32", &T::m_u32);
	field("m_u16", &T::m_u16);
	field("m_u8", &T::m_u8);
	field("m_f32", &T::m_f32);
	field("m_f64", &T::m_f64);
	field("m_vec2", &T::m_vec2);
	field("m_vec3", &T::m_vec3);
	field("m_vec4", &T::m_vec4);
	field("m_intVec2", &T::m_intVec2);
	field("m_intVec3", &T::m_intVec3);
	field("m_intVec4", &T::m_intVec4);
	field("m_rot", &T::m_rot);
	field("m_string", &T::m_string);
	field("m_color", &T::m_color);
	field("m_intColor", &T::m_intColor);
};

namespace ob::engine {

	ReflectionTestComponent::ReflectionTestComponent(Entity& entity)
		: Component(entity)
	{}

}