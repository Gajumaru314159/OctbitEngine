//***********************************************************
//! @file
//! @brief		ワールド
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/TypeBuilder.h>

namespace ob::engine {
	auto s_aaa = TypeRegister::Link<
		class Entity,
		class Component,
		class TransformComponent,
		class ReflectionTestComponent,
		class MeshComponent,
		class FlyCameraComponent,
		class LightComponent,
		class PointLightComponent,
		class SpotLightComponent,
		class DirectionalLightComponent
	>;
	//OB_REGISTER_RTTI(class Entity);
	//OB_REGISTER_RTTI(class Component);
	//OB_REGISTER_RTTI(class TransformComponent);
	//OB_REGISTER_RTTI(class ReflectionTestComponent);
	//OB_REGISTER_RTTI(class MeshComponent);
	//OB_REGISTER_RTTI(class FlyCameraComponent);
	//OB_REGISTER_RTTI(class LightComponent);
	//OB_REGISTER_RTTI(class PointLightComponent);
	//OB_REGISTER_RTTI(class SpotLightComponent);
	//OB_REGISTER_RTTI(class DirectionalLightComponent);
}