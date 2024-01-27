//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/Graphics/Render/RenderPipeline.h>
#include <Framework/Graphics/Graphics.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      描画シーンを生成
	//@―---------------------------------------------------------------------------
	Ref<RenderScene> RenderScene::Create(const RenderSceneDesc& desc) {
		return new RenderScene(desc);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ
	//@―---------------------------------------------------------------------------
	RenderScene::RenderScene(const RenderSceneDesc& desc)
		: m_name(desc.name)
	{
		// RenderFeature生成
		auto features = desc.features.create(*this);
		for (auto& feature : features) {
			m_features[feature->getTypeId()] = std::move(feature);
		}
		// RenderPipeline生成
		auto pipelines = desc.pipelines.create(*this);
		for (auto& pipeline : pipelines) {
			m_pipelines[pipeline->getTypeId()] = std::move(pipeline);
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief      デストラクタ
	//@―---------------------------------------------------------------------------
	RenderScene::~RenderScene() {
		OB_ASSERT_EXPR(m_graphics == nullptr);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画
	//@―---------------------------------------------------------------------------
	void RenderScene::render(FG& fg) {
		for (auto& [typeId,pipeline] : m_pipelines) {
			pipeline->render(fg,m_views);
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief      シーンを追加
	//@―---------------------------------------------------------------------------
	void RenderScene::addView(Ref<RenderView>& view) {

		if (!view) {
			LOG_WARNING("空のRenderViewは追加できません");
			return;
		}

		if (view->m_scene != nullptr) {
			LOG_WARNING("{}は他のRenderSceneで追加済みのRenderViewです", view->getName());
			return;
		}

		for (auto& item : m_views) {
			if (item == view) {
				LOG_WARNING("{}は追加済みのRenderViewです", view->getName());
				return;
			}
		}

		view->m_scene = this;

		m_views.push_back(view);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      シーンを削除
	//@―---------------------------------------------------------------------------
	void RenderScene::removeView(Ref<RenderView>& view) {

		if (!view) {
			LOG_WARNING("空のRenderViewは削除できません");
			return;
		}

		auto found = std::find(m_views.begin(), m_views.end(), [&](Ref<RenderView>& item) {return view == item; });

		if (found == m_views.end()) {
			LOG_WARNING("{}はこのRenderSceneに追加されていないRenderViewです", view->getName());
		}

		OB_ASSERT_EXPR(found->get() != nullptr);

		found->get()->m_scene = nullptr;

	}


	//@―---------------------------------------------------------------------------
	//! @brief      RenderFeatureを見つける
	//@―---------------------------------------------------------------------------
	RenderFeature* RenderScene::findFeature(TypeId typId)const {
		auto found = m_features.find(typId);
		if (found == m_features.end())return nullptr;
		return found->second.get();
	}

	//@―---------------------------------------------------------------------------
	//! @brief      RenderFeatureを走査する
	//@―---------------------------------------------------------------------------
	void RenderScene::visitFeatures(Func<void(RenderFeature&)>&& visitor) {
		if (!visitor)return;
		for (auto& [typeId,feature] : m_features) {
			visitor(*feature);
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief      全てのRenderFeatureを有効にする
	//@―---------------------------------------------------------------------------
	void RenderScene::activateAllFeature() {
		for (auto& [typeId, feature] : m_features) {
			feature->activate();
		}
	}

}