//***********************************************************
//! @file
//! @brief		ComponentFactory
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component/ComponentFactory.h>
#include <Framework/Engine/Engine.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ
	//@―---------------------------------------------------------------------------
	ComponentFactory::ComponentFactory() {
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