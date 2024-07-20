//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/File.h>
#include <Framework/Engine/Entity.h>
#include <Framework/Engine/World.h>
#include <Framework/Engine/Component/MeshComponent.h>
//#include <Framework/Model/Model.h>
//#include <Framework/Graphics/Feature/MeshRenderFeature.h>
#include <Framework/Graphics/Material/Material.h>
#include <Framework/Graphics/Mesh/Mesh.h>
#include <Framework/RHI/Texture.h>
#include <Framework/Core/Reflection/TypeBuilder.h>

#include <Framework/Graphics/Graphics.h>
#include <Framework/Graphics/Builtin/RenderFeature/MaterialRenderFeature.h>
#include <Framework/Engine/Component/TransformComponent.h>

OB_DEFINE_CLASS_INFO(ob::engine::MeshComponent) {
	tag("Requirements", "ob::engine::TransformComponent");
	base<ob::engine::Component>();
	constructor<ob::engine::Entity&>("entity").desc("コンストラクタ");

	property("LocalPosition", &T::getModel, &T::setModel).desc("モデルファイル");
}

namespace ob::engine {

	MeshComponent::MeshComponent(Entity& entity)
		: Component(entity)
	{

		using namespace ob::rhi;
		using namespace ob::graphics;

		m_material = [&] {

			auto code = File::ReadAllText("Asset/Shader/GraphicTest.hlsl");
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
	auto MeshComponent::getModel()const -> const String& {
		return m_path;
	}

	void MeshComponent::initialize() {
		if (auto transform = getEntity().findComponent<TransformComponent>()) {
			transform->addTransformChangedEvent(m_hTransformChanged, { *this,&MeshComponent::onTransformChanged });
		}
	}
	void MeshComponent::onTransformChanged(TransformComponent& transform) {
		if (m_material) {
			m_material->setMatrix("Matrix", transform.getWorld());
		}
	}

	void MeshComponent::activate() {
		updateModel();
	}

	void MeshComponent::deactivate() {
		updateModel();
	}

	void MeshComponent::updateModel() {

		graphics::MaterialRenderFeature* feature = nullptr;

		// 仮のアクセス
		if (auto rpi = graphics::Graphics::Get()) {
			if (auto scene = rpi->getScene()) {
				feature = scene->findFeature<graphics::MaterialRenderFeature>();
			}
		}

		if (feature == nullptr) {
			return;
		}
		
		feature->removeRenderable(m_id);

		if (getEntity().isActive() == false) {
			return;
		}

		m_mesh = {};
		if (File::Exists(m_path)) {
			m_mesh = graphics::Mesh::Load(m_path);
		}

		// メッシュの読み込み失敗
		if (!m_mesh) {
			return;
		}

		m_id = feature->addRenderable(m_mesh, m_material);

	}

}