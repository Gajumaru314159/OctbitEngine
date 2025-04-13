//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Material/MaterialManager.h>
#include <Framework/RHI/Texture.h>
#include <Framework/RHI/Buffer.h>
#include <Framework/RHI/Sampler.h>
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/RHI/CommandList.h>
#include <Framework/RHI/RootSignature.h>

namespace ob::graphics {

	//!	@brief			コンストラクタ
	MaterialManager::MaterialManager(rhi::RHI&, NameDictionary&) {
		initializeGlobalProperties();
		initializeRootSignature();
	}

	//!	@brief			デストラクタ
	MaterialManager::~MaterialManager() {
	}

	//!	@brief			グローバルプロパティの初期化
	void MaterialManager::initializeGlobalProperties() {

		MaterialBlockDesc desc;
		desc.name = "GlobalMaterialParamter";
		desc.vectors = { "LightDir" };
		desc.matrices = { "Matrix" };

		m_block.construct(desc);

	}

	//!	@brief			共通ルートシグネチャを生成
	void MaterialManager::initializeRootSignature() {
		using namespace ob::rhi;

		// TODO テクスチャの複数枚対応
		RootSignatureDesc desc;
		desc.constants.set(16*2, 0);
		desc.samplers = { StaticSamplerDesc(SamplerDesc(TextureFillter::Linear), 0) };	// グローバルプロパティ(サンプラー)
		desc.flags &= RootSignatureFlag::EnableBindless;
		desc.name = "Common";

		m_signature = RootSignature::Create(desc);
		OB_ASSERT_EXPR(m_signature);
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
	bool MaterialManager::hasProprty(StringView name, MaterialPropertyType type) const {
		return m_block->hasProprty(name, type);
	}

	//! @brief  
	void MaterialManager::setFloat(StringView name, f32 value) {
		return m_block->setScalar(name, value);
	}

	//! @brief  
	void MaterialManager::setColor(StringView name, Color value) {
		return m_block->setVector(name, value);
	}

	//! @brief  
	void MaterialManager::setMatrix(StringView name, const Matrix& value) {
		return m_block->setMatrix(name, value);
	}

	//! @brief  
	void MaterialManager::setTexture(StringView name, const Ref<rhi::Texture>& value) {
		return m_block->setTexture(name, value,rhi::Sampler::Default());
	}

	//! @brief  
	void MaterialManager::recordGlobalShaderProperties(Ref<rhi::CommandList>& cmdList) {
		m_block->record(cmdList, 0);
	}


}