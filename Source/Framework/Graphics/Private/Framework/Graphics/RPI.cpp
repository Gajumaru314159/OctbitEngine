//***********************************************************
//! @file
//! @brief		システム
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/RPI.h>
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/RHI/RHI.h>

#include <Framework/Graphics/Render/RenderFrameData.h>
#include <Framework/Graphics/System.h>
#include <Framework/Graphics/Material/MaterialManager.h>
#include <Framework/RHI/System.h>
#include <Framework/Core/Utility/DI.h>
#include <Framework/Core/Utility/Name.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      システムをServiceInjectorに登録
	//@―---------------------------------------------------------------------------
	void Register(ServiceInjector& injector) {
		injector.bind<MaterialManager>();
		injector.bind<RPI>();
		rhi::Register(injector);
		Name::Register(injector);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ
	//@―---------------------------------------------------------------------------
	RPI::RPI(rhi::RHI& rhi)
		: m_rhi(rhi)
	{
		m_frameDataList.resize(2);
		for (s32 i = 0; i < m_frameDataList.size(); ++i) {
			m_frameDataList.at(i) = std::make_unique<RenderFrameData>();
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief      デストラクタ
	//@―---------------------------------------------------------------------------
	RPI::~RPI() {
		for (auto& scene : m_scenes) {
			LOG_ERROR("RPIに登録状態のRenderSceneが存在します。[name={}]",scene->getName());
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief      シーンを登録
	//! @details    登録したシーンはRPIの更新対象に含まれるようになります。
	//@―---------------------------------------------------------------------------
	void RPI::addScene(const Ref<RenderScene>& scene) {
		if (!scene)return;

		bool exists = std::find(m_scenes.begin(), m_scenes.end(), scene) != m_scenes.end();
		if (exists) {
			LOG_WARNING("{}は登録済みのRenderSceneです。",scene->getName());
			return;
		}

		m_scenes.push_back(scene);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      シーンの登録を解除
	//@―---------------------------------------------------------------------------
	void RPI::removeScene(const Ref<RenderScene>& scene) {
		std::remove(m_scenes.begin(), m_scenes.end(), scene);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      ゲームループごとの更新を実行する
	//@―---------------------------------------------------------------------------
	void RPI::update() {
		beginForParallel(1);
		updateForParallel(1);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      更新準備
	//@―---------------------------------------------------------------------------
	void RPI::beginForParallel(s32 threadNum) {

		wait();

		// TODO RHI実行スレッド待機
		
		// TODO RHI実行スレッド起動
		
		// TODO 実行準備
		m_frameDataList.next();

	}

	//@―---------------------------------------------------------------------------
	//! @brief      ゲームループごとの更新を実行する
	//@―---------------------------------------------------------------------------
	void RPI::updateForParallel(s32 threadIndex) {
		
		// TODO 前フレームの描画完了を待つ

		auto& storage = m_frameDataList.current()->storage;

		for (auto& scene : m_scenes) {
			scene->render(storage);
		}

		// notify

	}


	//@―---------------------------------------------------------------------------
	//! @brief      ネイティブの描画コマンドを発行して描画処理を開始する
	//@―---------------------------------------------------------------------------
	void RPI::execute() {
		// TODO 前フレームの描画完了を待機
		// TODO ICommandからCommandListを構築
		// TODO CommandList構築を待機
		// TODO RHIThreadで描画開始
	}


	//@―---------------------------------------------------------------------------
	//! @brief      描画コマンドの完了を待機する
	//@―---------------------------------------------------------------------------
	void RPI::wait() {
		// TODO 前フレームの描画完了を待機
		// TODO ICommandからCommandListを構築
		// TODO CommandList構築を待機
		// TODO RHIThreadで描画開始
	}



}