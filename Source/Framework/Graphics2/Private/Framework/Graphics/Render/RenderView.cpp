//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/Graphics/Render/RenderStep.h>
#include <Framework/Graphics/FrameGraph/FG.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/CommandList.h>
#include <Framework/Platform/Window.h>
#include <Framework/RHI/Display.h>

namespace ob::graphics {

	static std::atomic<s32> s_renderViewId{ 1 };

	RenderView::RenderView(const RenderViewDesc& desc,RenderScene& scene,RenderStepInjector& injector) 
		: m_scene(scene)
	{
		auto steps = injector.create(*this);
		for (auto& step : steps) {
			m_steps[step->getTypeId()] = std::move(step);
		}

		m_priority = 0;
	}

	void RenderView::setPriority(s32 priority) {
		m_priority = priority;
	}
	s32 RenderView::getPriority()const {
		return m_priority;
	}


	void RenderView::setRenderTarget(const Ref<rhi::Display>& display){
		
		m_display = display;
		m_texture = {};

		if (!display)return;

		rhi::RenderTextureDesc desc;
		desc.name = Format("{}_RenderView",m_desc.name);
		desc.size = display->getDesc().size;
		desc.format = rhi::TextureFormat::RGBA8;
		desc.clear.color = Color::Yellow;
		desc.display = display;
		m_texture = rhi::RenderTexture::Create(desc);
	}

	auto RenderView::getRenderTarget()const->const Ref<rhi::RenderTexture> {
		return m_texture;
	}

	void RenderView::applyDisplay(FG& fg) {
		fg.addCallbackPass<FG::NoData>(
			"AppluDisplay",
			[](FrameGraph::Builder& builder, FG::NoData& data) {
				builder.setSideEffect();
			},
			[this](const FG::NoData& data, FrameGraphPassResources& resources, rhi::CommandList& cmdList) {
				cmdList.applyDisplay(m_display, m_texture);
			}
		);
	}

	//@―---------------------------------------------------------------------------
	//! @brief      RenderFeatureを見つける
	//@―---------------------------------------------------------------------------
	RenderStep* RenderView::findStep(TypeId typId)const {
		auto found = m_steps.find(typId);
		if (found == m_steps.end())return nullptr;
		return found->second.get();
	}

}