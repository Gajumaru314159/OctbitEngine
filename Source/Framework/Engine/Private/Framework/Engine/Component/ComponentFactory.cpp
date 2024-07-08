//***********************************************************
//! @file
//! @brief		コンポーネント・ファクトリ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component/ComponentFactory.h>

#include <Framework/Engine/Component/TransformComponent.h>
#include <Framework/Engine/Component/TransformComponentImpl.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ
	//@―---------------------------------------------------------------------------
	ComponentFactory::ComponentFactory() {
		registerCreator(Type::Get<TransformComponent>(), std::make_unique<ComponentCreatorTemplate<TransformComponentImpl>>());
	}

	//@―---------------------------------------------------------------------------
	//! @brief      デストラクタ
	//@―---------------------------------------------------------------------------
	ComponentFactory::~ComponentFactory() {
	}

	//@―---------------------------------------------------------------------------
	//! @brief		ContentCreatorを登録
	//@―---------------------------------------------------------------------------
	void ComponentFactory::registerCreator(Type type, UPtr<ComponentCreator> creator) {
		m_creatorMap[type] = std::move(creator);
	}

	//@―---------------------------------------------------------------------------
	//! @brief		ContentCreatorを検索
	//@―---------------------------------------------------------------------------
	ComponentCreator* ComponentFactory::findCreator(Type type) {
		auto found = m_creatorMap.find(type);
		if (found != m_creatorMap.end()) {
			return found->second.get();
		}
		return nullptr;
	}

}