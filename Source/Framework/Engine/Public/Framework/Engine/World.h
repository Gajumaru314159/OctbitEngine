﻿//***********************************************************
//! @file
//! @brief		ワールド
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Forward.h>
#include <Framework/Engine/Scene.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief  ワールド
	//! @see	https://docs.unrealengine.com/4.27/en-US/API/Runtime/Engine/Engine/UWorld/
	//@―---------------------------------------------------------------------------
	class World : public RefObject {
	public:

		static Ref<World> Create(StringView name) {
			return new World(name);
		}

	public:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		~World();

		//@―---------------------------------------------------------------------------
		//! @brief		ルートシーンを取得
		//@―---------------------------------------------------------------------------
		const Ref<Scene>& getRootScene()const;

	private:

		World(StringView name);

		void createRootScene();

	private:

		String m_name;

		Ref<Scene> m_scene;

	};

}
