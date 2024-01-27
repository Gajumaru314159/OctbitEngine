//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Render/RenderView.h>

namespace ob::graphics {

	static std::atomic<s32> s_renderViewId{ 1 };

	RenderView::RenderView(RenderViewDesc& desc) {
		m_id = static_cast<RenderViewId>(s_renderViewId.fetch_add(1));
		m_priority = 0;
	}


	auto RenderView::getId()const->RenderViewId {
		return m_id;
	}

	void RenderView::setPriority(s32 priority) {
		m_priority = priority;
	}
	s32 RenderView::getPriority()const {
		return m_priority;
	}



}