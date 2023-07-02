//***********************************************************
//! @file
//! @brief		コンポーネント・ファクトリ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component/ComponentFactory.h>

#include <Framework/Engine/Component/TransformComponent.h>
#include <Framework/Engine/Component/TransformComponentImpl.h>
#include <Framework/Engine/Component/CameraComponent.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ
	//@―---------------------------------------------------------------------------
	ComponentFactory::ComponentFactory() {

		registerCreator(TypeId::Get<TransformComponent>(), std::make_unique<ComponentCreatorTemplate<TransformComponentImpl>>());
		registerCreator(TypeId::Get<CameraComponent>(), std::make_unique<ComponentCreatorTemplate<CameraComponent>>());
	}

	//@―---------------------------------------------------------------------------
	//! @brief      デストラクタ
	//@―---------------------------------------------------------------------------
	ComponentFactory::~ComponentFactory() {
	}

	//@―---------------------------------------------------------------------------
	//! @brief		ContentCreatorを登録
	//@―---------------------------------------------------------------------------
	void ComponentFactory::registerCreator(TypeId typeId, UPtr<ComponentCreator> creator) {
		m_creatorMap[typeId] = std::move(creator);
	}

	//@―---------------------------------------------------------------------------
	//! @brief		ContentCreatorを検索
	//@―---------------------------------------------------------------------------
	ComponentCreator* ComponentFactory::findCreator(TypeId typeId) {
		auto found = m_creatorMap.find(typeId);
		if (found != m_creatorMap.end()) {
			return found->second.get();
		}
		return nullptr;
	}

}