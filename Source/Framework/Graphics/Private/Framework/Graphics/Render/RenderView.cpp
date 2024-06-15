//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/RenderPipeline.h>
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/RHI/Display.h>
#include <Framework/RHI/RenderTexture.h>

#include <Framework/Graphics/FrameGraph/FG.h>
#include <Framework/RHI/CommandList.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      コンストラクタ
	//@―---------------------------------------------------------------------------
	RenderView::RenderView(RenderScene& scene, StringView name)
		: m_scene(scene)
		, m_name(name)
	{
		scene.addReleasedEvent(m_hRelease, {*this,&RenderView::onSceneReleased});
		scene.addView(this);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      デストラクタ
	//@―---------------------------------------------------------------------------
	RenderView::~RenderView() {
		m_releasedNotifier.invoke(*this);
		m_scene.removeView(this);
	}


	//@―---------------------------------------------------------------------------
	//! @brief      RenderFeatureを見つける
	//@―---------------------------------------------------------------------------
	RenderFeature* RenderView::findFeature(TypeId typeId)const {
		return m_scene.findFeature(typeId);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画先を設定する
	//@―---------------------------------------------------------------------------
	void RenderView::setDisplay(const Ref<Display>& display) {
		if (m_display == display)return;
		clearTarget();

		m_display = display;
		RenderTextureDesc desc;
		desc.name = m_name;
		desc.size = display->getDesc().size;
		desc.format = TextureFormat::RGBA8;
		desc.clear.color = Color::Black;
		desc.display = display;
		m_renderTexture = RenderTexture::Create(desc);
	}
	
	//@―---------------------------------------------------------------------------
	//! @brief      ディスプレイを取得する
	//@―---------------------------------------------------------------------------
	auto RenderView::getDisplay()const->const Ref<rhi::Display> {
		return m_display;
	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画先を設定する
	//@―---------------------------------------------------------------------------
	void RenderView::setRenderTexture(const Ref<RenderTexture>& renderTexture) {
		if (m_renderTexture == renderTexture)return;
		clearTarget();

		m_renderTexture = renderTexture;
	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画先を取得する
	//@―---------------------------------------------------------------------------
	auto RenderView::getRenderTexture()const->const Ref<rhi::RenderTexture> {
		return m_renderTexture;
	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画
	//@―---------------------------------------------------------------------------
	void RenderView::render(FG& fg) {
		if (m_pipeline)m_pipeline->render(fg);

		// TODO 登録順序に依存を持たせないと最初に描画される？
		if (m_display && m_renderTexture) {
			fg.addCallbackPass<FG::NoData>(
				"AppluDisplay",
				[](FrameGraph::Builder& builder, FG::NoData& data) {
					builder.setSideEffect();
				},
				[this](const FG::NoData& data, FrameGraphPassResources& resources, rhi::CommandList& cmdList) {
					cmdList.applyDisplay(m_display, m_renderTexture);
				}
			);
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画先をクリア
	//@―---------------------------------------------------------------------------
	void RenderView::clearTarget() {
		m_pipeline.reset();
		m_display = {};
		m_renderTexture = {};
	}
	//@―---------------------------------------------------------------------------
	//! @brief      シーンの開放チェック
	//@―---------------------------------------------------------------------------
	void RenderView::onSceneReleased(RenderScene& scene) {
		LOG_ERROR("シーンの開放前にビューを解放してください");
		clearTarget();
	}

}