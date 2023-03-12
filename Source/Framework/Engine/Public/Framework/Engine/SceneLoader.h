//***********************************************************
//! @file
//! @brief		シーン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Scene.h>

namespace ob::engine {

	// TODO SceneLoaderはAssetに依存するのでEngine層から除外する必要がある
	//		残す場合は内部処理を外部から登録できるようにする必要がある

	//@―---------------------------------------------------------------------------
	//! @brief  シーン読み込みクラス
	//@―---------------------------------------------------------------------------
	class SceneLoader{
	public:

		SceneLoader(Path path,SceneEventDelegate loaded) {
			m_path = std::move(path);
			m_loaded = std::move(loaded);

			std::function<void(SceneLoader&)> f = &SceneLoader::load;

			Thread loadThread(Format(TC("SceneLoader [{}]"), path), std::bind(f, *this));

		}

		~SceneLoader() {
			m_thread.join();
		}

	private:

		void load() {

			Ref<Scene> scene;

			// 非同期読み込み処理

			if (m_loaded) {
				m_loaded(scene);
			}
		}

	private:
		Thread	m_thread;
		Path	m_path;
		SceneEventDelegate m_loaded;
	};

}// namespcae ob
