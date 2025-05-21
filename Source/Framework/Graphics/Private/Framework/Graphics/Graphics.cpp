//***********************************************************
//! @file
//! @brief		システム
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Utility/DI.h>
#include <Framework/Graphics/FrameGraph/FG.h>
#include <Framework/Graphics/Graphics.h>
#include <Framework/Graphics/Material/MaterialManager.h>
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/RHI/CommandList.h>
#include <Framework/RHI/System.h>
#include <Framework/Graphics/FrameGraph/FGResourcePool.h>

namespace ob::graphics {

	//! @brief      システムをServiceInjectorに登録
	void RegisterGraphicsService(ServiceInjector& injector) {
		injector.bind<MaterialManager>();
		injector.bind<Graphics>();
		rhi::RegisterRHIService(injector);
		Name::Register(injector);
	}

	//! @brief      コンストラクタ
	Graphics::Graphics(rhi::RHI& rhi,MaterialManager&)
		: m_rhi(rhi)
		, m_fgResourcePool(rhi)
	{

		for (s32 i = 0; i < 3; ++i) {
			rhi::CommandListDesc desc;
			desc.name = Format("SystemCommanList_{}", i);
			desc.type = rhi::CommandListType::Graphic;
			m_commandLists.emplace_back(rhi::CommandList::Create(desc));
		}

	}

	//! @brief      デストラクタ
	Graphics::~Graphics() {
		OB_ASSERT(m_scenes.empty(), "削除されていないRenderSceneが存在します");
	}

	//! @brief      ゲームループごとの更新を実行する
	void Graphics::update() {

		beginForParallel(1);
		updateForParallel(0);
	}

	//! @brief      更新準備
	void Graphics::beginForParallel(s32 threadNum) {

		wait();

		// TODO RHI実行スレッド待機

		// TODO RHI実行スレッド起動

		// TODO 実行準備

		// NOTE ENQUEUE_RENDER_COMMANDのようなカスタムコマンド実行を仕込む？そもそものFrameGraphもENQUEUE_RENDER_COMMANDで追加しても良いかも

		m_fgResourcePool->update();

		m_fg = std::make_unique<FG>();

		//
		for (auto& scene : m_scenes) {
			scene->render(*m_fg);
		}

		m_fg->compile();


	}

	//! @brief      ゲームループごとの更新を実行する
	void Graphics::updateForParallel(s32 threadIndex) {

		if (threadIndex != 0)return;

		auto commandList = m_commandLists.current();

		commandList->begin();
		m_fg->execute(commandList, *m_fgResourcePool);
		commandList->end();
		commandList->flush();

	}


	//! @brief      ネイティブの描画コマンドを発行して描画処理を開始する
	void Graphics::execute() {
		// TODO 前フレームの描画完了を待機
		// TODO ICommandからCommandListを構築
		// TODO CommandList構築を待機
		// TODO RHIThreadで描画開始
	}


	//! @brief      描画コマンドの完了を待機する
	void Graphics::wait() {
		// TODO 前フレームの描画完了を待機
		// TODO ICommandからCommandListを構築
		// TODO CommandList構築を待機
		// TODO RHIThreadで描画開始
	}

	//! @brief      シーンを追加
	//! @note       追加したシーンはGraphicsの終了までに removeScene で削除される必要があります。
	void Graphics::addScene(RenderScene* scene) {
		if (scene == nullptr) {
			LOG_WARNING("無効なRenderSceneは追加できません");
			return;
		}
		if (contains_item(m_scenes, scene)) {
			LOG_WARNING("RenderSceneの多重追加はできません");
			return;
		}
		m_scenes.push_back(scene);
	}

	//! @brief      シーンを削除
	void Graphics::removeScene(RenderScene* scene) {
		if (scene == nullptr) {
			LOG_WARNING("無効なRenderSceneは削除できません");
			return;
		}
		m_scenes.erase(std::remove(m_scenes.begin(), m_scenes.end(), scene), m_scenes.end());
		//if (!erase_all_item(m_scenes, scene)) {
		//	LOG_WARNING("追加されていないRenderSceneを削除しようとしました");
		//	return;
		//}
	}

	FGData Graphics::getFGData()const {
		FGData data;
		if (m_fg) {
			FGDataWriter writer(data);
			m_fg->debugOutput(writer);
		}
		return data;
	}

}