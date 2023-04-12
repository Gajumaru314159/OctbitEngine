//***********************************************************
//! @file
//! @brief		コンポーネント・ファクトリ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Component.h>
#include <Framework/Engine/ComponentCreator.h>
#include <Framework/Engine/Entity.h>
#include <Framework/Engine/Reflection/TypeBuilder.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief  コンポーネント・ファクトリ
	//@―---------------------------------------------------------------------------
	class ComponentFactory: public Singleton<ComponentFactory> {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		ComponentFactory();

		//@―---------------------------------------------------------------------------
		//! @brief  デストラクタ
		//@―---------------------------------------------------------------------------
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