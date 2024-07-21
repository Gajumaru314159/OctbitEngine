//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/TypeBuilder.h>
#include <Framework/Engine/Component/FlyCameraComponent.h>
#include <Framework/Engine/Component/TransformComponent.h>
#include <Framework/Engine/Entity.h>
#include <Framework/Input/Mouse.h>
#include <Framework/Input/Keyboard.h>

#include <Framework/Graphics/Material/Material.h>
#include <Framework/Platform/Window.h>

OB_DEFINE_CLASS_INFO(ob::engine::FlyCameraComponent) {
	tag("Requirements", "ob::engine::TransformComponent");
	base<ob::engine::Component>();
	constructor<ob::engine::Entity&>("entity").desc("コンストラクタ");

	property("TranslationSpeed", &T::getTranslationSpeed, &T::setTranslationSpeed).desc("移動速度").tag("Min","0").tag("Max","100");
	property("RotationSpeed", &T::getRotationSpeed, &T::setRotationSpeed).desc("回転速度").tag("Min", "0").tag("Max", "100");
}

namespace ob::engine {

	FlyCameraComponent::FlyCameraComponent(Entity& entity)
		: Component(entity)
	{

	}
	// RotationSpeeedのGetterとSetter
	void FlyCameraComponent::setRotationSpeed(f32 value) {
		setProperty(m_rotationSpeed, value, "RotationSpeed");
	}

	// TranslationSpeedのGetterとSetter
	void FlyCameraComponent::setTranslationSpeed(f32 value) {
		setProperty(m_translationSpeed, value, "TranslationSpeed");
	}

	void FlyCameraComponent::update() {
		using namespace ob::input;
		if (auto transform = getEntity().findComponent<TransformComponent>()) {

			auto deltaTime = 0.05f;

			if (Mouse::Right.pressed()) {
				// カメラ回転
				Rot rotation = transform->getLocalRotation();
				rotation.y += Mouse::DeltaX.value() * m_rotationSpeed * deltaTime;
				rotation.x += Mouse::DeltaY.value() * m_rotationSpeed * deltaTime;
				rotation.y = Math::Mod(rotation.y, 360.f);
				rotation.x = Math::Clamp(rotation.x, -85.f, 85.f);
				rotation.z = 0.0f;

				
				// マウスのホイール変更量を移動速度に掛ける
				m_translationSpeed += Mouse::Wheel.value()*0.01f;
				m_translationSpeed = Math::Max(m_translationSpeed, 0.01f);

				// 移動
				Vec3 speed(0, 0, 0);
				Vec3 updown(0, 0, 0);
				if (Keyboard::W.pressed())speed.z += 1;
				if (Keyboard::S.pressed())speed.z -= 1;
				if (Keyboard::A.pressed())speed.x -= 1;
				if (Keyboard::D.pressed())speed.x += 1;
				if (Keyboard::E.pressed())updown.y += 1;
				if (Keyboard::Q.pressed())updown.y -= 1;

				Vec3 position = transform->getLocalPosition();
				position += (rotation.toQuat() * speed + updown) * m_translationSpeed * deltaTime;


				// 更新
				Transform trs;
				trs.position = position;
				trs.scale = Vec3(1);
				trs.rotation = rotation.toQuat();
				transform->setLocal(trs);

				auto viewSize = platform::Window::Main().getSize();
				auto viewMtx =
					Matrix::Perspective(60, viewSize.x, viewSize.y, 0.01f, 10000.0f) *
					trs.inverse();
				graphics::Material::SetGlobalMatrix("Matrix", viewMtx);
			}

			
		}
	}

}