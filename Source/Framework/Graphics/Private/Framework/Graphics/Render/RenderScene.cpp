//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Graphics.h>
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Render/RenderSceneDesc.h>
#include <Framework/Graphics/Render/RenderView.h>

namespace ob::graphics {

	//! @brief      コンストラクタ
	RenderScene::RenderScene(const RenderSceneDesc& desc) {

		// 作成可能なPassを列挙
		for (auto& feature : desc.features.features) {
			if (!feature) continue;

			m_features.emplace_back(feature(*this));

			// Typeで辞書引きできるように登録
			auto& f = m_features.back();
			m_featuresByType[f->getType()] = f.get();
			
		}

		// 必要なPassを列挙
		for (auto& pdesc : desc.pipelines) {

			auto& pipeline = m_pipelines[pdesc.name];

			// TODO グローバルパス対応

			// RenderPassを登録
			for (auto& feature : m_features) {
				feature->setupPasses(pipeline.builer);
			}

			// RenderPassを接続
			for (auto& connection : pdesc.connections) {
				pipeline.builer.connect(connection);
			}

			pipeline.builer.flush();

		}


		Graphics::Get()->addScene(this);
	}

	//! @brief      デストラクタ
	RenderScene::~RenderScene() {
		m_releasedNotifier.invoke(*this);

		for (auto& [name,pipeline] : m_pipelines) {
			OB_ASSERT(pipeline.views.empty(), "削除されていないRenderViewが存在します");
		}

		Graphics::Get()->removeScene(this);
	}

	//! @brief      RenderFeatureを見つける
	RenderFeature* RenderScene::findFeature(Type type)const {
		auto found = m_featuresByType.find(type);
		if (found == m_featuresByType.end())return nullptr;
		return found->second;
	}

	//! @brief      解放時イベントを追加
	void RenderScene::addReleasedEvent(RenderSceneEventHandle& handle, RenderSceneEventDelegate func) {
		m_releasedNotifier.add(handle, func);
	}


	//! @brief      ビューを追加
	void RenderScene::addView(RenderView* view,StringView pipelineName) {

		if (view == nullptr) {
			LOG_WARNING("無効なRenderViewは追加できません");
			return;
		}

		auto itr = m_pipelines.find(pipelineName);
		if (itr == m_pipelines.end()) {
			LOG_ERROR("未登録のRenderPipelineです [name={}]",pipelineName);
			return;
		}

		auto& pipeline = itr->second;

		if (contains_item(pipeline.views, view)) {
			LOG_WARNING("RenderViewの多重追加はできません");
			return;
		}

		pipeline.views.push_back(view);

	}

	//! @brief      ビューを削除
	void RenderScene::removeView(RenderView* view) {
		if (view == nullptr) {
			LOG_WARNING("無効なRenderViewは削除できません");
			return;
		}

		for (auto& [name,pipeline] : m_pipelines) {
			pipeline.views.erase(std::remove(pipeline.views.begin(), pipeline.views.end(), view), pipeline.views.end());
		}
	}

	//! @brief      描画
	void RenderScene::render(FG& fg) {

		// TODO Passに移行
		for (auto& feature : m_features) {
			// feature->render(fg, m_blackboard);
		}


		// Viewの優先度でソート
		struct ViewInfo { s32 priority; RenderPipeline& pipeline; RenderView& view; };
		Vector<ViewInfo> views;
		for (auto& [pipelineName,pipeline] : m_pipelines) {
			for (auto& view : pipeline.views) {
				// TODO Priority指定
				views.emplace_back(/*view.priority*/ 0 , pipeline, *view);
			}
		}
		//std::sort(views.begin(), views.end());

		// Viewごとにパスを実行
		for (auto& viewInfo : views) {
			viewInfo.pipeline.builer.render(fg, viewInfo.view);
		}

	}

}