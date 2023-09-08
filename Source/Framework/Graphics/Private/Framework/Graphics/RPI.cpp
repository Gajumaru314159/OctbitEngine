//***********************************************************
//! @file
//! @brief		システム
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/RPI.h>
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/RHI/RHI.h>

namespace ob::graphics {

	class RPIImpl :public RPI {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief      コンストラクタ
		//@―---------------------------------------------------------------------------
		RPIImpl(rhi::RHI& rhi)
			: m_rhi(rhi)
		{

		}

		//@―---------------------------------------------------------------------------
		//! @brief      シーンを追加する
		//@―---------------------------------------------------------------------------
		void addScene(Ref<RenderScene> scene) override {

			if (!scene) {
				LOG_WARNING("空のシーンが追加されました。");
				return;
			}

			for (auto& s : m_scenes) {
				if (s == scene) {
					LOG_WARNING("{}は既に登録されているシーンです。", scene->getName());
				}
				if (s->getName() == scene->getName()) {
					LOG_ERROR("{}は同じ名前のシーンが既に存在します。", scene->getName());
				}
			}

			m_scenes.push_back(scene);

		}

		//@―---------------------------------------------------------------------------
		//! @brief      シーンを削除する
		//@―---------------------------------------------------------------------------
		void removeScene(Ref<RenderScene> scene) override {
			if (!scene) {
				LOG_WARNING("空のシーンを削除しようとしました。");
				return;
			}
			for (auto itr = m_scenes.begin(); itr != m_scenes.end(); itr++) {
				if (*itr == scene) {
					m_scenes.erase(itr);
					return;
				}
			}
			LOG_ERROR("{}は未登録のシーンです。",scene->getName());
		}

		//@―---------------------------------------------------------------------------
		//! @brief      シーンを探す
		//@―---------------------------------------------------------------------------
		Ref<RenderScene> findScene(StringView name)const override {
			for (auto& scene : m_scenes) {
				if (scene->getName() == name) {
					return scene;
				}
			}
			return {};
		}

		//@―---------------------------------------------------------------------------
		//! @brief      ゲームループごとの更新を実行する
		//@―---------------------------------------------------------------------------
		void simulate() override {
			for (auto& scene : m_scenes) {
				scene->simulate();
			}
		}

		//@―---------------------------------------------------------------------------
		//! @brief      描画ループの更新をする
		//@―---------------------------------------------------------------------------
		void render() override {
			for (auto& scene : m_scenes) {
				scene->render();
			}
			m_rhi.update();
		}

	private:
		rhi::RHI& m_rhi;
		Array<Ref<RenderScene>> m_scenes;

	};

}