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

#include <Framework/Model/Model.h>

#include <Framework/Core/Thread/ThreadPool.h>


OB_DEFINE_CLASS_INFO(ob::engine::MeshComponent) {
	desc("メッシュの描画を行うクラス");
	tag("Requirements", "ob::engine::TransformComponent");
	base<ob::engine::Component>();
	constructor<ob::engine::Entity&>("entity").desc("コンストラクタ");

	property("LocalPosition", &T::getModel, &T::setModel).desc("モデルファイル");
}

namespace ob::engine {

	MeshComponent::MeshComponent(Entity& entity)
		: Component(entity)
	{
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
		if (!m_model)return;

		// テスト
		f32 maxScale = 1.0f;
		for (auto& submesh : m_model->getMesh()->getSubMeshes()) {
			maxScale = Math::Max(maxScale, submesh.bounds.size.maxAbsComponent());
		}

		for (auto& material : m_model->getMaterials()) {
			material->setMatrix("Matrix", transform.getWorld().toMatrix() * Matrix::Scale(Vec3(10/maxScale)));
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

		for (auto& id : m_materialIds) feature->removeRenderable(id);
		m_materialIds.clear();

		if (getEntity().isActive() == false) return;

		ScopeLock lock(m_lock);

		m_model = {};

		if (auto pool = ThreadPool::Get()) {
			pool->enqueue([this] {
				{
					ScopeLock lock(m_lock);
					if (File::Exists(m_path)) {
						m_model = model::Model::Load(m_path);
					}
				}
				onModelLoaded();
			});
		} else {
			if (File::Exists(m_path)) {
				m_model = model::Model::Load(m_path);
			}

			onModelLoaded();
		}


	}

	void MeshComponent::onModelLoaded() {

		ScopeLock lock(m_lock);

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

		// メッシュの読み込み失敗
		if (!m_model) return;

		if (auto transform = getEntity().findComponent<TransformComponent>()) {
			onTransformChanged(*transform);
		}

		for (auto& [index, submesh] : Indexed(m_model->getMesh()->getSubMeshes())) {
			auto id = feature->addRenderable(m_model->getMesh(), m_model->getMaterials().at(index));
			m_materialIds.push_back(id);
		}

	}

}