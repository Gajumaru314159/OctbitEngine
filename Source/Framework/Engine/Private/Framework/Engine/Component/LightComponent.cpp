//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Engine/Component/LightComponent.h>
#include <Framework/Core/Reflection/TypeBuilder.h>

OB_DEFINE_CLASS_INFO(ob::engine::LightComponent) {
	desc("ライト基底クラス");
	base<ob::engine::Component>();

	property("Color", &T::getColor, &T::setColor).desc("ライトの色");
	property("Intensity", &T::getIntensity, &T::setIntensity).desc("ライトの強度").tag("Unit","lm");
}
OB_DEFINE_CLASS_INFO(ob::engine::DirectionalLightComponent) {
	desc("指向性ライトコンポーネント");
	base<ob::engine::LightComponent>();
	constructor<ob::engine::Entity&>("entity");
}
OB_DEFINE_CLASS_INFO(ob::engine::PointLightComponent) {
	desc("ポイントライトコンポーネント");
	base<ob::engine::LightComponent>();
	constructor<ob::engine::Entity&>("entity");

	property("Radius", &T::getRadius, &T::setRadius).desc("ライトの半径").tag("Unit", "m");
	property("Falloff", &T::getFalloff, &T::setFalloff).desc("ライトの減衰").tag("Unit", "lm/m");
}
OB_DEFINE_CLASS_INFO(ob::engine::SpotLightComponent) {
	desc("スポットライトコンポーネント");
	base<ob::engine::LightComponent>();
	constructor<ob::engine::Entity&>("entity");

	property("Radius", &T::getRadius, &T::setRadius).desc("ライトの半径").tag("Unit", "m");
	property("Falloff", &T::getFalloff, &T::setFalloff).desc("ライトの減衰").tag("Unit", "lm/m");
	property("Angle", &T::getAngle, &T::setAngle).desc("ライトの角度").tag("Unit", "deg");
	property("AngleFalloff", &T::getAngleFalloff, &T::setAngleFalloff).desc("ライトの角度減衰").tag("Unit", "lm/deg");
}

namespace ob::engine {

	LightComponent::LightComponent(Entity& entity)
		: Component(entity)
	{
		m_color = Color::White;
		m_intensity = 100;
	}


	void LightComponent::setColor(const Color& value) {
		setProperty(m_color,value,"Color");
	}

	void LightComponent::setIntensity(f32 value) {
		setProperty(m_intensity,value,"Intensity");
	}



	DirectionalLightComponent::DirectionalLightComponent(Entity& entity)
		: LightComponent(entity)
	{}


	PointLightComponent::PointLightComponent(Entity& entity)
		: LightComponent(entity)
	{
		m_radius = 1;
		m_falloff = 1;
	}

	void PointLightComponent::setRadius(f32 value) {
		setProperty(m_radius, value, "Radius");
	}

	void PointLightComponent::setFalloff(f32 value) {
		setProperty(m_falloff, value, "Falloff");
	}


	SpotLightComponent::SpotLightComponent(Entity & entity)
		: LightComponent(entity)
	{
		m_radius = 1;
		m_falloff = 1;
		m_angle = 45;
		m_angleFalloff = 0;
	}

	void SpotLightComponent::setRadius(f32 value) {
		setProperty(m_radius, value, "Radius");
	}

	void SpotLightComponent::setFalloff(f32 value) {
		setProperty(m_falloff, value, "Falloff");
	}

	void SpotLightComponent::setAngle(f32 value) {
		setProperty(m_angle, value, "Angle");
	}

	void SpotLightComponent::setAngleFalloff(f32 value) {
		setProperty(m_angleFalloff, value, "AngleFalloff");
	}

}