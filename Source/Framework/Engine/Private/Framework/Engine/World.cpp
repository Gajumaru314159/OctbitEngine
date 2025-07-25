//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Engine/World.h>

namespace ob::engine {

	//! @brief		コンストラクタ
	World::World(StringView name) 
		: m_name(name)
	{
		createRootScene();
	}
	//! @brief		コンストラクタ
	World::~World() {

	}

	//! @brief		ルートシーン取得
	Scene& World::getRootScene()const {
		OB_ASSERT(m_scene!=nullptr,"ルートシーンが解放済みです。");
		return *m_scene;
	}


	//! @brief		ルートシーン生成
	void World::createRootScene() {
		if (m_scene)return;
		m_scene = Scene::Create(Format("Root({})",m_name));
	}

}
