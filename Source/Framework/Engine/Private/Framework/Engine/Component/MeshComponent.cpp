//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Entity.h>
#include <Framework/Engine/World.h>
#include <Framework/Engine/Component/MeshComponent.h>
//#include <Framework/Model/Model.h>
//#include <Framework/Graphics/Feature/MeshRenderFeature.h>
#include <Framework/Graphics/Material/Material.h>
#include <Framework/Graphics/Mesh/Mesh.h>
#include <Framework/RHI/Texture.h>

#include <Framework/Graphics/Graphics.h>
#include <Framework/Graphics/Builtin/RenderFeature/MaterialRenderFeature.h>

namespace ob::engine {

	MeshComponent::MeshComponent(Entity& entity)
		: Component(entity)
	{

		using namespace ob::rhi;
		using namespace ob::graphics;

		m_material = [&] {

			auto code = ReadFile("Asset/Shader/GraphicTest.hlsl");
			OB_ASSERT(code, "ファイル読み込み失敗");

			MaterialDesc desc;
			desc.name = "Default";
			desc.matrixProperties = { "Matrix" };
			desc.textureProperties = { "Main" };

			MaterialPass& opaque = desc.passes["Opaque"];
			opaque.depthStencil.depth.enable = true;
			opaque.colors = { TextureFormat::RGBA8 ,TextureFormat::RGBA8 ,TextureFormat::RGBA8 };	// Shaderに情報を持たせたい
			opaque.depth = TextureFormat::D32;
			opaque.vs = Shader::CompileVS(code.value());
			opaque.ps = Shader::CompilePS(code.value());
			opaque.requiredLayout = {
				{Semantic::Position,ElementType::Float,4},
				{Semantic::Normal,ElementType::Float,4},
				{Semantic::TexCoord,ElementType::Float,2},
			};

			return Material::Create(desc);
		}();

		m_mainTex = Texture::Load("Asset/Model/Ukulele_col.dds");
		m_material->setMatrix("Matrix", Matrix::Identity);
		m_material->setTexture("Main", m_mainTex);
	}

	void MeshComponent::setModel(StringView path) {
		m_path = path;
		updateModel();
	}

	void MeshComponent::activate() {
		updateModel();
	}

	void MeshComponent::deactivate() {
		// m_handle = {};
	}

	void MeshComponent::updateModel() {
		m_mesh = graphics::Mesh::Load(m_path);

		if (auto rpi = graphics::Graphics::Get()) {
			if (auto scene = rpi->getScene()) {
				if (auto feature = scene->findFeature<graphics::MaterialRenderFeature>()) {
					feature->addRenderable(m_mesh,m_material);
				}
			}
		}

		if (auto world = getEntity().getWorld()) {
			//if (rpi = world->findSystem<RPI>()) {
			//
			//}

			//if (auto feature = scene.findFeature<MaterialRenderFeature>()) {
			//	feature->addRenderable(mesh, material);
			//}
		}
	}

}