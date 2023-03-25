﻿//***********************************************************
//! @file
//! @brief		TransformComponentImpl
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component/ComponentFactory.h>
#include <Framework/Engine/Engine.h>
#include <Framework/Engine/ModuleFactory.h>

REGISTER_MODULE(ob::engine::ComponentFactory);

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief      取得
	//@―---------------------------------------------------------------------------
	ComponentFactory& ComponentFactory::Get() {
		static std::atomic<ComponentFactory*> m = nullptr;
		if (m == nullptr) {
			m = GEngine->get<ComponentFactory>();
		}
		OB_ASSERT_EXPR(m);
		return *m;
	}

	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ
	//@―---------------------------------------------------------------------------
	ComponentFactory::ComponentFactory() {

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