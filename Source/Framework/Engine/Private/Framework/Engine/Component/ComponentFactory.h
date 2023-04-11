//***********************************************************
//! @file
//! @brief		TransformComponentImpl
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component.h>
#include <Framework/Engine/ComponentCreator.h>
#include <Framework/Engine/Entity.h>
#include <Framework/Engine/Reflection/TypeBuilder.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	class ComponentFactory {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief      取得
		//@―---------------------------------------------------------------------------
		static ComponentFactory* Get();

	public:

		ComponentFactory();
		~ComponentFactory();


		//@―---------------------------------------------------------------------------
		//! @brief  ContentCreatorを登録
		//@―---------------------------------------------------------------------------
		void registerCreator(TypeId typeId, UPtr<ComponentCreator> creator);

		//@―---------------------------------------------------------------------------
		//! @brief  ContentCreatorを検索
		//@―---------------------------------------------------------------------------
		ComponentCreator* findCreator(TypeId typeId);

	private:

		HashMap<TypeId, UPtr<ComponentCreator>> m_creatorMap;

	};
}