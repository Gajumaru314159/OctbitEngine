//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component.h>

namespace ob::engine {

	//! @brief  フライカメラ制御
	class FlyCameraComponent : public engine::Component {
	public:
		OB_RTTI();

		FlyCameraComponent(Entity&);

		// RotationSpeeedのGetterとSetter
		f32 getRotationSpeed()const { return m_rotationSpeed; }
		void setRotationSpeed(f32 value);

		// TranslationSpeedのGetterとSetter
		f32 getTranslationSpeed()const { return m_translationSpeed; }
		void setTranslationSpeed(f32 value);

		void update();
	private:
		f32 m_rotationSpeed = 1.0f;
		f32 m_translationSpeed = 1.0f;
	};

}