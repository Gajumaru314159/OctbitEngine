//***********************************************************
//! @file
//! @brief		システム
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Graphics.h>
#include <Framework/Graphics/Render/RenderScene.h>

#include <Framework/Graphics/Material/MaterialManager.h>
#include <Framework/RHI/System.h>
#include <Framework/RHI/CommandList.h>


#include <Framework/Graphics/FrameGraph/FG.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      システムをServiceInjectorに登録
	//@―---------------------------------------------------------------------------
	void Register(ServiceInjector& injector) {
		injector.bind<MaterialManager>();
		injector.bind<Graphics>();
		rhi::Register(injector);
		Name::Register(injector);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ
	//@―---------------------------------------------------------------------------
	Graphics::Graphics(rhi::RHI& rhi) 
		: m_rhi(rhi)
		, m_fgResourcePool(rhi)
	{

		for (s32 i = 0; i < 3; ++i) {
			rhi::CommandListDesc desc;
			desc.name = Format(TC("SystemCommanList_{}"), i);
			desc.type = rhi::CommandListType::Graphic;
			m_commandLists.emplace_back(rhi::CommandList::Create(desc));
		}

	}

	//Ref<RenderScene> Graphics::createScene() {
	//	return nullptr;
	//}



	//@―---------------------------------------------------------------------------
	//! @brief      ゲームループごとの更新を実行する
	//@―---------------------------------------------------------------------------
	void Graphics::update() {
		beginForParallel(1);
		updateForParallel(0);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      更新準備
	//@―---------------------------------------------------------------------------
	void Graphics::beginForParallel(s32 threadNum) {

		wait();

		// TODO RHI実行スレッド待機

		// TODO RHI実行スレッド起動

		// TODO 実行準備

		m_fgResourcePool.update();

		m_fg = std::make_unique<FG>();

		for (auto& scene : m_scenes) {
			OB_ASSERT_EXPR(scene);
			scene->render(*m_fg);
		}

		m_fg->compile();

	}

	//@―---------------------------------------------------------------------------
	//! @brief      ゲームループごとの更新を実行する
	//@―---------------------------------------------------------------------------
	void Graphics::updateForParallel(s32 threadIndex) {

		if (threadIndex != 0)return;
		
		auto commandList = m_commandLists.current();

		m_fg->execute(*commandList,m_fgResourcePool);

	}


	//@―---------------------------------------------------------------------------
	//! @brief      ネイティブの描画コマンドを発行して描画処理を開始する
	//@―---------------------------------------------------------------------------
	void Graphics::execute() {
		// TODO 前フレームの描画完了を待機
		// TODO ICommandからCommandListを構築
		// TODO CommandList構築を待機
		// TODO RHIThreadで描画開始
	}


	//@―---------------------------------------------------------------------------
	//! @brief      描画コマンドの完了を待機する
	//@―---------------------------------------------------------------------------
	void Graphics::wait() {
		// TODO 前フレームの描画完了を待機
		// TODO ICommandからCommandListを構築
		// TODO CommandList構築を待機
		// TODO RHIThreadで描画開始
	}

	//@―---------------------------------------------------------------------------
	//! @brief      シーンを追加
	//@―---------------------------------------------------------------------------
	void Graphics::addScene(Ref<RenderScene>& scene) {

		if (!scene) {
			LOG_WARNING("空のRenderSceneは追加できません");
			return;
		}

		if (scene->m_graphics != nullptr) {
			LOG_WARNING("{}は他のGraphicsで追加済みのRenderSceneです", scene->getName());
			return;
		}

		for (auto& item : m_scenes) {
			if (item == scene) {
				LOG_WARNING("{}は追加済みのRenderSceneです", scene->getName());
				return;
			}
		}

		scene->m_graphics = this;

		m_scenes.push_back(scene);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      シーンを削除
	//@―---------------------------------------------------------------------------
	void Graphics::removeScene(Ref<RenderScene>& scene) {

		if (!scene) {
			LOG_WARNING("空のRenderSceneは削除できません");
			return;
		}

		auto found = std::find(m_scenes.begin(), m_scenes.end(), [&](Ref<RenderScene>& item) {return scene == item; });

		if (found == m_scenes.end()) {
			LOG_WARNING("{}はこのGraphicsに追加されていないシーンです", scene->getName());
		}

		OB_ASSERT_EXPR(found->get() != nullptr);

		found->get()->m_graphics = nullptr;

	}

}