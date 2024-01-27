//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Entity.h>
#include <Framework/Engine/World.h>
#include <Framework/Engine/Component/MeshComponent.h>
//#include <Framework/Model/Model.h>
//#include <Framework/Graphics/Feature/MeshRenderFeature.h>

namespace ob::engine {

	MeshComponent::MeshComponent() {

	}

	void MeshComponent::setModel() {

		// active時のみでいいかも？
		updateModel();
	}

	void MeshComponent::activate() {
		updateModel();
	}

	void MeshComponent::deactivate() {
		// m_handle = {};
	}

	void MeshComponent::updateModel() {
		if (auto world = getEntity().getWorld()) {
			//if (rpi = world->findSystem<RPI>()) {
			//
			//}
		}
	}

}