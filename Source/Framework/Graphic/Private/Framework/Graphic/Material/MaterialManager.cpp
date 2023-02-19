//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/Material/MaterialManager.h>
#include <Framework/Graphic/Module/GraphicModule.h>
#include <Framework/Engine/Engine.h>

namespace ob::graphic {

	//@―---------------------------------------------------------------------------
	//!	@brief			取得
	//@―---------------------------------------------------------------------------
	MaterialManager& MaterialManager::Get() {
		static auto module = GEngine->get<GraphicModule>();
		OB_ASSERT_EXPR(module);
		return module->getMaterialManager();
	}


	MaterialManager::MaterialManager() = default;
	MaterialManager::~MaterialManager() {
	}

	//@―---------------------------------------------------------------------------
	//!	@brief			描画タグにRenderPassを登録
	//@―---------------------------------------------------------------------------
	void MaterialManager::registerRenderPass(engine::Name name, const Ref<rhi::RenderPass>& renderPass, s32 subpass) {
		m_renderPassMap.emplace(name, rhi::SubPass{ renderPass, subpass });
	}

	//@―---------------------------------------------------------------------------
	//!	@brief			描画タグからRenderPassを登録
	//@―---------------------------------------------------------------------------
	rhi::SubPass MaterialManager::FindRenderPass(engine::Name renderTag) {
		auto found = m_renderPassMap.find(renderTag);
		if (found == m_renderPassMap.end())
			return {nullptr,0};
		return found->second;
	}


	//@―---------------------------------------------------------------------------
	//!	@brief			レイアウトID取得
	//@―---------------------------------------------------------------------------
	VertexLayoutId MaterialManager::getVertexLayoutId(const rhi::VertexLayout& layout) {
		ScopeLock lock(m_lock);
		auto found = m_map.find(layout);
		if (found == m_map.end()) {
			return m_map[layout] = static_cast<VertexLayoutId>(m_map.size());
		} else {
			return found->second;
		}
	}


}// namespace ob