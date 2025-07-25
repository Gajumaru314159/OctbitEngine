//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component.h>
#include <Framework/Engine/Entity.h>
#include <Framework/Core/Reflection/TypeBuilder.h>

namespace ob::engine {

	//! @brief  Transformコンポーネント
	class ReflectionTestComponent :public Component {
	public:

		OB_RTTI();

		ReflectionTestComponent(Entity& entity);
		~ReflectionTestComponent() = default;

		bool m_bool = false;
		s32 m_s32 = 0;
		s16 m_s16 = 0;
		s8 m_s8 = 0;
		u32 m_u32 = 0;
		u16 m_u16 = 0;
		u8 m_u8 = 0;
		f32 m_f32 = 0.0f;
		f64 m_f64 = 0.0;
		Vec2 m_vec2 = Vec2::Zero;
		Vec3 m_vec3 = Vec3::Zero;
		Vec4 m_vec4 = Vec4::Zero;
		IntVec2 m_intVec2 = IntVec2::Zero;
		IntVec3 m_intVec3 = IntVec3::Zero;
		IntVec4 m_intVec4 = IntVec4::Zero;
		Rot m_rot = Rot::Identity;
		String m_string = "";
		Color m_color = Color::White;
		IntColor m_intColor = IntColor::White;

	};

}
