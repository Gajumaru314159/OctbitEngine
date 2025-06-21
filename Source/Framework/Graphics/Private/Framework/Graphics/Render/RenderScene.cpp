//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Graphics.h>
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Render/RenderView.h>

namespace ob::graphics {

	//! @brief      コンストラクタ
	RenderScene::RenderScene() {
		Graphics::Get()->addScene(this);
	}

	//! @brief      デストラクタ
	RenderScene::~RenderScene() {
		m_releasedNotifier.invoke(*this);
		Graphics::Get()->removeScene(this);
	}

	//! @brief      RenderFeatureを見つける
	RenderFeature* RenderScene::findFeature(Type type)const {
		return m_features.find(type);
	}


	void RenderScene::setPipeline(s32 index, Ref<RenderPipeline> pipeline) {
		if (pipeline) {
			m_pipelines[index] = pipeline;
			pipeline->setup(*this,m_features);
		} else {
			m_pipelines.erase(index);
		}
	}

	//! @brief      解放時イベントを追加
	void RenderScene::addReleasedEvent(RenderSceneEventHandle& handle, RenderSceneEventDelegate func) {
		m_releasedNotifier.add(handle, func);
	}


	//! @brief      ビューを追加
	void RenderScene::addView(RenderView* view) {

		if (view == nullptr) {
			LOG_WARNING("無効なRenderViewは追加できません");
			return;
		}

		if (contains_item(m_views, view)) {
			LOG_WARNING("RenderViewの多重追加はできません");
			return;
		}

		m_views.push_back(view);

	}

	//! @brief      ビューを削除
	void RenderScene::removeView(RenderView* view) {

		if (view == nullptr) {
			LOG_WARNING("無効なRenderViewは削除できません");
			return;
		}

		m_views.erase(std::remove(m_views.begin(), m_views.end(), view), m_views.end());

	}

	void RenderScene::visitView(Func<void(RenderView&)> visitor) {
		if (!visitor)return;
		for (auto& view : m_views) {
			visitor(*view);
		}
	}

	//! @brief      描画
	void RenderScene::render(FG& fg) {

		std::sort(m_views.begin(), m_views.end(), [](auto& a, auto& b) { return a->get<RenderViewData>().priority < b->get<RenderViewData>().priority; });

		for (auto& [type,pipeline] : m_pipelines) {
			pipeline->render(fg, *this);
		}

		for (auto& view : m_views) {
			s32 index = view->get<RenderViewData>().pipeline;
			if (!is_in_range(index, m_pipelines)) continue;

			m_pipelines[index]->render(fg, *view);
		}

	}

}