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

		//ScopeLock lock(m_lock);

		f32 maxScale = 1.0f;
		m_model->visitParts([&](model::ModelParts& parts) {
			maxScale = Math::Max(maxScale, parts.bounds.box.size.length()*0.5f);
		});

		m_model->visitParts([&](model::ModelParts& parts) {
			parts.material->setMatrix("Matrix", transform.getWorld().toMatrix() * Matrix::Scale(Vec3(10 / maxScale)));
		});
	}

	void MeshComponent::activate() {
		updateModel();
	}

	void MeshComponent::deactivate() {
		updateModel();
	}

	void MeshComponent::updateModel() {

		ScopeLock lock(m_lock);

		m_model = {};

		if (getEntity().isActive() == false) return;
		
		if (auto pool = ThreadPool::Get()) {
			pool->enqueue([this] {
				if (File::Exists(m_path)) {
					auto model = model::Model::Load(m_path);
					ScopeLock lock(m_lock);
					m_model = model;
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

		if (m_model) {
			// 仮のアクセス
			if (auto rpi = graphics::Graphics::Get()) {
				if (auto scene = rpi->getScene()) {
					m_model->join(*scene);

					if (auto transform = getEntity().findComponent<TransformComponent>()) {
						onTransformChanged(*transform);
					}
				}
			}
		}
	}

}