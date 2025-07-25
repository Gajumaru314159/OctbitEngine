//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Material/MaterialManager.h>
#include <Framework/Graphics/Material/MaterialSystem.h>
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/Sampler.h>
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/RHI/CommandList.h>
#include <Framework/RHI/RootSignature.h>

namespace ob::graphics {

	enum class MaterialBlockScope {
		Material,
		Global,
		Scene,
		View,
		Draw,
	};


	//!	@brief			コンストラクタ
	MaterialManager::MaterialManager(rhi::Device&, NameDictionary&) {
	}

	//!	@brief			デストラクタ
	MaterialManager::~MaterialManager() {
	}

	//!	@brief			レイアウトID取得
	VertexLayoutId MaterialManager::getVertexLayoutId(const rhi::VertexLayout& layout) {
		ScopeLock lock(m_lock);
		auto found = m_vertexLayoutCache.find(layout);
		if (found == m_vertexLayoutCache.end()) {
			return m_vertexLayoutCache[layout] = static_cast<VertexLayoutId>(m_vertexLayoutCache.size());
		} else {
			return found->second;
		}
	}

	//! @brief  
	void MaterialManager::recordGlobalShaderProperties(Ref<rhi::CommandList>& cmdList) {
		auto& m_block = MaterialSystem::Instance().getGlobalBlock();
		return m_block.record(cmdList, enum_cast(MaterialBlockScope::Global));
	}


}