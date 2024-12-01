//***********************************************************
//! @file
//! @brief		Transformコンポーネント
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component.h>

namespace ob::engine {

	//! @brief  Transformコンポーネント
	class LightComponent :public Component {
	public:

		OB_RTTI();

		LightComponent(Entity&);

		void setColor(const Color& value);
		auto getColor()const { return m_color; }

		void setIntensity(f32 value);
		auto getIntensity()const  { return m_intensity; }

	private:

		Color				m_color;
		f32					m_intensity;
		f32					m_radius;

		// 減衰
		// 間接光


		// Culling


		// Shadow

		// Cookie

	};


	class DirectionalLightComponent : public LightComponent {
	public:
		OB_RTTI();
		DirectionalLightComponent(Entity&);
	private:
	};

	class PointLightComponent : public LightComponent {
	public:
		OB_RTTI();
		PointLightComponent(Entity&);

		void setRadius(f32 value);
		auto getRadius()const { return m_radius; }
		void setFalloff(f32 value);
		auto getFalloff()const { return m_falloff; }

	private:
		f32					m_radius;
		f32					m_falloff;
	};


	class SpotLightComponent : public LightComponent {
	public:
		OB_RTTI();
		SpotLightComponent(Entity&);

		void setRadius(f32 value);
		auto getRadius()const { return m_radius; }
		void setFalloff(f32 value);
		auto getFalloff()const { return m_falloff; }
		void setAngle(f32 value);
		auto getAngle()const { return m_angle; }
		void setAngleFalloff(f32 value);
		auto getAngleFalloff()const { return m_angleFalloff; }
	private:
		f32					m_radius;
		f32					m_falloff;
		f32					m_angle;
		f32					m_angleFalloff;
	};

}
