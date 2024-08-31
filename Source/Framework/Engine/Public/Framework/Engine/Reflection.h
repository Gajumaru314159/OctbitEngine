//***********************************************************
//! @file
//! @brief		ワールド
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/TypeRegister.h>
#include <Framework/Core/Reflection/PrimitiveRegister.h>

OB_REGISTER_RTTI(ob::engine::Entity);
OB_REGISTER_RTTI(ob::engine::Component);
OB_REGISTER_RTTI(ob::engine::TransformComponent);

OB_REGISTER_RTTI(ob::engine::ReflectionTestComponent);
OB_REGISTER_RTTI(ob::engine::MeshComponent);
OB_REGISTER_RTTI(ob::engine::FlyCameraComponent);
OB_REGISTER_RTTI(ob::engine::LightComponent);
OB_REGISTER_RTTI(ob::engine::PointLightComponent);
OB_REGISTER_RTTI(ob::engine::SpotLightComponent);
OB_REGISTER_RTTI(ob::engine::DirectionalLightComponent);