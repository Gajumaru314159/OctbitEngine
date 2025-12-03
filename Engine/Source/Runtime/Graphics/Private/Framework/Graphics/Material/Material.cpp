//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Material/Material.h>
#include <Framework/Graphics/Material/MaterialShader.h>
#include <Framework/Graphics/Material/MaterialSystem.h>
#include <Framework/Graphics/Mesh/Mesh.h>
#include <Framework/Graphics/Mesh/MeshImpl.h>
#include <Framework/RHI/CommandList.h>
#include <Framework/RHI/Sampler.h>

namespace ob::graphics {

	//! @brief  生成
	Ref<Material> Material::Create(const Ref<MaterialShader>& shader) {
		return new Material(shader);
	}

	//! @brief  生成
	Ref<Material> Material::Create(const MaterialDesc& desc) {
		return Create(MaterialShader::Create(desc));
	}

	Material::Material(const Ref<MaterialShader>& shader) :m_shader(shader) {
		OB_ASSERT_EXPR(m_shader);
		m_block = MaterialBlock(shader->getBlockDesc());
	}

	const MaterialDesc& Material::getDesc() const {
		return m_shader->getDesc();
	}

	bool Material::hasProperty(StringView name, MaterialPropertyType type) const {
		return m_block.hasProperty(name, type);
	}

	void Material::setTexture(StringView name, const Ref<Texture>& value) {
		m_block.setTexture(name, value, rhi::Sampler::Default());
	}

	void Material::setBuffer(StringView name, const Ref<Buffer>& value) {
		m_block.setBuffer(name, value);
	}

	void Material::setMatrix(StringView name, const Matrix& value) {
		m_block.setMatrix(name, value);
	}

	void Material::setVector(StringView name, Color value) {
		m_block.setVector(name, value);
	}

	void Material::setScalar(StringView name, f32 value) {
		m_block.setScalar(name, value);
	}

	void Material::setInteger(StringView name, f32 value) {
		m_block.setInteger(name, value);
	}

	s32 Material::calcQualityIndex(StringView pass, s32 quality) const {
		return m_shader->calcQualityIndex(pass, quality);
	}

	bool Material::prepare(const Ref<Mesh>& mesh) {
		return m_shader->prepare(mesh);
	}

	void Material::record(Ref<rhi::CommandList>& commandList, MaterialBlockSet& blocks, const Ref<Mesh>& mesh, s32 submesh, StringView passName, s32 quality) {

		auto pMesh = mesh.cast<MeshImpl>();
		if (!pMesh) return;

		Ref<rhi::PipelineState> pipeline = m_shader->getPipeline(mesh, passName, quality);
		if (!pipeline) return;

		commandList->setPipelineState(pipeline);

		m_block.record(commandList, 0);
		if (blocks.global)blocks.global->record(commandList, 1);
		if (blocks.scene)blocks.scene->record(commandList, 2);
		if (blocks.view)blocks.view->record(commandList, 3);

		pMesh->record(commandList, submesh);
	}


	//! @brief  グローバルマテリアルパラメータを設定
	void Material::SetGlobalTexture(StringView name, const Ref<Texture>& value) {
		if (auto system = MaterialSystem::Get()) {
			system->getGlobalBlock().setTexture(name, value, rhi::Sampler::Default());
		}
	}

	//! @brief  グローバルマテリアルパラメータを設定
	void Material::SetGlobalBuffer(StringView name, const Ref<Buffer>& value) {
		if (auto system = MaterialSystem::Get()) {
			system->getGlobalBlock().setBuffer(name, value);
		}
	}

	//! @brief  グローバルマテリアルパラメータを設定
	void Material::SetGlobalMatrix(StringView name, const Matrix& value) {
		if (auto system = MaterialSystem::Get()) {
			system->getGlobalBlock().setMatrix(name, value);
		}
	}

	//! @brief  グローバルマテリアルパラメータを設定
	void Material::SetGlobalVector(StringView name, Color value) {
		if (auto system = MaterialSystem::Get()) {
			system->getGlobalBlock().setVector(name, value);
		}
	}

	//! @brief  グローバルマテリアルパラメータを設定
	void Material::SetGlobalScalar(StringView name, f32 value) {
		if (auto system = MaterialSystem::Get()) {
			system->getGlobalBlock().setScalar(name, value);
		}
	}

	//! @brief  グローバルマテリアルパラメータを設定
	void Material::SetGlobalInteger(StringView name, s32 value) {
		if (auto system = MaterialSystem::Get()) {
			system->getGlobalBlock().setInteger(name, value);
		}
	}

}
