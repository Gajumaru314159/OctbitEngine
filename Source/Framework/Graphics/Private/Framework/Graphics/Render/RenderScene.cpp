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
	Ref<RenderScene> RenderScene::Create(const RenderSceneDesc& desc, Graphics* graphics) {
		if (graphics == nullptr) graphics = Graphics::Get();
		if (graphics == nullptr) return nullptr;
		return graphics->createScene(desc);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ
	//@―---------------------------------------------------------------------------
	RenderScene::RenderScene(const RenderSceneDesc& desc, Graphics& graphics)
		: m_graphics(graphics)
		, m_name(desc.name)
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
	}

	//@―---------------------------------------------------------------------------
	//! @brief      破棄予約状態か
	//@―---------------------------------------------------------------------------
	bool RenderScene::isDisposeRequested()const {
		return m_disposeRequested;
	}

	void RenderScene::requestDispose() {
		m_disposeRequested = true;
	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画
	//@―---------------------------------------------------------------------------
	void RenderScene::render(FG& fg) {
		for (auto& [typeId, pipeline] : m_pipelines) {
			pipeline->render(fg, m_views);
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief      RenderView を生成する
	//@―---------------------------------------------------------------------------
	auto RenderScene::createView(const RenderViewDesc& desc) -> Ref<RenderView> {
		return new RenderView(desc,*this);
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
		for (auto& [typeId, feature] : m_features) {
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