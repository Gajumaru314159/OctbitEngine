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

	enum class MaterialBlockScope {
		Material,
		Global,
		Scene,
		View,
		Draw,
	};


	//!	@brief			コンストラクタ
	MaterialManager::MaterialManager(rhi::RHI&, NameDictionary&) {
		initializeGlobalProperties();
		initializeSceneProperties();
		initializeViewProperties();
	}

	//!	@brief			デストラクタ
	MaterialManager::~MaterialManager() {
	}

	//!	@brief			グローバルプロパティの初期化
	void MaterialManager::initializeGlobalProperties() {

		MaterialBlockDesc desc;
		desc.name = "GlobalMaterialProperties";
		desc.matrices = { "MatrixTest" };
		desc.scalars = { "Time" };
		desc.layout = m_globalLayout = MaterialBlock::CreateLayout(desc,enum_cast(MaterialBlockScope::Global));

		m_block.construct(desc);



	}

	//!	@brief			シーンプロパティの初期化
	void MaterialManager::initializeSceneProperties() {

		MaterialBlockDesc desc;
		desc.name = "SceneMaterialProperties";
		desc.textures = {
			// "ShadowTexture",
		};
		desc.buffers = { 
			// "SpotLightBuffer",
			// "PointLightBuffer",
			// "DirectionalLightBuffer",
		};
		desc.vectors = { 
			"LightDir" 
		};
		desc.layout = m_sceneLayout = MaterialBlock::CreateLayout(desc, enum_cast(MaterialBlockScope::Scene));

		m_block.construct(desc);

	}

	//!	@brief			グローバルビュープロパティの初期化
	void MaterialManager::initializeViewProperties() {

		MaterialBlockDesc desc;
		desc.name = "ViewMaterialProperties";
		desc.matrices = {
			"MatrixV",
			"MatrixP",
			"MatrixVP",
			"MatrixInvV",
			"MatrixInvP",
			"MatrixInvVP",
		};
		desc.vectors = {
			"CameraPos",
			"ScreenSize",
			"CameraUp",
			"CameraRight",
			"CameraFront",
		};
		desc.layout = m_viewLayout = MaterialBlock::CreateLayout(desc, enum_cast(MaterialBlockScope::View));

		m_block.construct(desc);

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
		return m_block->hasProperty(name, type);
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
		m_block->record(cmdList, enum_cast(MaterialBlockScope::Global));
	}


}