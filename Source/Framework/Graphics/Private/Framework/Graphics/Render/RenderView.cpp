//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/RenderPipeline.h>
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/RHI/SwapChain.h>
#include <Framework/RHI/RenderTexture.h>

#include <Framework/Graphics/FrameGraph/FG.h>
#include <Framework/RHI/CommandList.h>

namespace ob::graphics {

	//! @brief      コンストラクタ
	RenderView::RenderView(RenderScene& scene, StringView name)
		: m_scene(scene)
		, m_name(name)
	{
		setRect({ 0,0,1,1 });
		scene.addReleasedEvent(m_hRelease, {*this,&RenderView::onSceneReleased});
		scene.addView(this);
	}

	//! @brief      デストラクタ
	RenderView::~RenderView() {
		m_releasedNotifier.invoke(*this);
		m_scene.removeView(this);
	}

	//! @brief      描画矩形を設定
	void RenderView::setRect(const Rect& rect) {
		m_rect = rect;
	}

	//! @brief      描画矩形を取得
	auto RenderView::getRect()const->const Rect& {
		return m_rect;
	}

	//! @brief      描画サイズを取得
	auto RenderView::getRenderSize()const->Size {
		if (!m_renderTexture) return { 0,0 };
		return m_renderTexture->size();
	}

	//! @brief      描画矩形を取得
	auto RenderView::getScaledRect()const->IntRect {
		auto size = getRenderSize();

		IntRect rect{
			(s32)(m_rect.left* size.width),
			(s32)(m_rect.top* size.height),
			(s32)(m_rect.right* size.width),
			(s32)(m_rect.bottom* size.height),
		};

		return rect;
	}

	//! @brief      ビューポートを取得
	auto RenderView::getViewport()const->Viewport {
		IntRect rect = getScaledRect();
		Viewport viewport(rect.left, rect.top, rect.right, rect.bottom);
		return viewport;
	}

	//! @brief      RenderFeatureを見つける
	RenderFeature* RenderView::findFeature(Type type)const {
		return m_scene.findFeature(type);
	}

	//! @brief      描画先を設定する
	void RenderView::setDisplay(const Ref<SwapChain>& swapChain) {
		if (m_swapChain == swapChain)return;
		clearTarget();

		m_swapChain = swapChain;

		m_swapChain->addEventListener(m_hDisplayUpdated, { *this,&RenderView::onDisplayUpdated });

		onDisplayUpdated();

	}
	
	//! @brief      ディスプレイを取得する
	auto RenderView::getDisplay()const->const Ref<rhi::SwapChain> {
		return m_swapChain;
	}

	//! @brief		ディスプレイ更新イベント
	void RenderView::onDisplayUpdated() {

		RenderTextureDesc desc;
		desc.name = Format("Display_{}", m_name);
		desc.size = m_swapChain->getDesc().size;
		desc.format = TextureFormat::RGBA8;
		desc.clear.color = Color::Black;
		m_renderTexture = RenderTexture::Create(desc);

	}

	//! @brief      描画先を設定する
	void RenderView::setRenderTexture(const Ref<RenderTexture>& renderTexture) {
		if (m_renderTexture == renderTexture)return;
		clearTarget();

		m_renderTexture = renderTexture;
	}

	//! @brief      描画先を取得する
	auto RenderView::getRenderTexture()const->const Ref<rhi::RenderTexture> {
		return m_renderTexture;
	}

	//! @brief      描画
	void RenderView::render(FG& fg) {
		if (!m_pipeline) return;
		if (!m_swapChain || !m_renderTexture)return;
		
		FGTexture target = m_pipeline->render(fg);

		struct Data {
			Ref<SwapChain> swapChain;
			FGTexture target;
		};

		fg.addPass<Data>(
			"ApplyDisplay",
			[&](FGBuilder& builder, Data& data) {
				data.swapChain = m_swapChain;
				data.target = builder.read(target);
				builder.setSideEffect();
			},
			[](const Data& data, FGResources& resources, rhi::CommandList& cmdList) {
				auto texture = resources.get(data.target);
				cmdList.applySwapChain(data.swapChain, texture);
			}
		);
	}

	//! @brief      描画先をクリア
	void RenderView::clearTarget() {
		m_pipeline.reset();
		m_swapChain = {};
		m_renderTexture = {};
	}
	//! @brief      シーンの開放チェック
	void RenderView::onSceneReleased(RenderScene& scene) {
		LOG_ERROR("シーンの開放前にビューを解放してください");
		clearTarget();
	}

}