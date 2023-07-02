#include <Framework/Core/Core.h>
#include <Framework/Engine/All.h>
#include <Plugins/ImGui/ImGui.h>
#include <Framework/Engine/Component/CameraComponent.h>

using namespace ob;

engine::Entity* s_selectedEntity = nullptr;
engine::Scene* s_selectedScene = nullptr;

void drawOutliner_Entity(engine::Entity* entity) {
	if (!entity)return;
	auto name = ImGui::ToImChars(entity->getName());
	bool empty = entity->getChildren().empty();

	auto flag = empty ? ImGuiTreeNodeFlags_Leaf : 0;
	if (s_selectedEntity == entity)flag |= ImGuiTreeNodeFlags_Selected;
	flag |= ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;

	bool opend = ImGui::TreeNodeEx(name, flag, name);

	if (ImGui::IsItemClicked()) {
		s_selectedScene = nullptr;
		s_selectedEntity = entity;
	}

	if (opend) {

		for (auto& child : entity->getChildren()) {

			drawOutliner_Entity(child);

		}

		ImGui::TreePop();

	}
}
void drawOutliner_Scene(const Ref<engine::Scene>& scene) {
	if (!scene)return;
	auto name = ImGui::ToImChars(scene->getName());
	bool empty = scene->getChildren().empty() && scene->getEntities().empty();

	auto flag = empty ? ImGuiTreeNodeFlags_Leaf : 0;
	if (s_selectedScene == scene.get())flag |= ImGuiTreeNodeFlags_Selected;
	flag |= ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;

	bool opend = ImGui::TreeNodeEx(name, flag, name);

	if (ImGui::IsItemClicked()) {
		s_selectedScene = scene.get();
		s_selectedEntity = nullptr;
	}

	if (opend) {

		for (auto& child : scene->getChildren()) {

			drawOutliner_Scene(child);

		}

		ImGui::ScopedTextColor textColor(Color::Yellow);

		for (auto& entity : scene->getEntities()) {

			drawOutliner_Entity(entity.get());

		}

		ImGui::TreePop();

	}
}
void drawOutliner(const Ref<engine::Scene>& scene) {

	if (ImGui::Begin("Outliner")) {
		drawOutliner_Scene(scene);
	}
	ImGui::End();

}
void drawComponents(engine::Entity* pEntity) {

	if (pEntity == nullptr) {
		pEntity = s_selectedEntity;
	}

	if (ImGui::Begin("Inspector")) {
		if (pEntity) {
			auto& entity = *pEntity;
			ImGui::Text(ImGui::ToImChars(entity.getName()));

			for (auto& component : entity.componets()) {
				auto cmpname = ImGui::ToImChars(component->getComponentTypeId().name());
				if (ImGui::CollapsingHeader(cmpname)) {

					ImGui::ScopedIndent indent;

					if (component->getTypeId() == TypeId::Get<engine::TransformComponent>()) {
						auto c = reinterpret_cast<engine::TransformComponent*>(component.get());
						{
							Vec3 value = c->getLocal().position;
							if (ImGui::DragFloat3("Position", value)) {
								c->setLocalPosition(value);
							}
						}
						{
							auto eulerAngles = c->getLocal().rotation.toRot();
							f32 xyz[] = { eulerAngles.x,eulerAngles.y,eulerAngles.z };
							if (ImGui::DragFloat3("Rotation", xyz)) {
								c->setLocalRotation({ xyz[0] ,xyz[1] ,xyz[2] });
							}
						}
						{
							Vec3 value = c->getLocal().scale;
							if (ImGui::DragFloat3("Scale", value)) {
								c->setLocalScale(value);
							}
						}
					}
					if (component->getTypeId() == TypeId::Get<engine::CameraComponent>()) {
						auto c = reinterpret_cast<engine::CameraComponent*>(component.get());
						{
							f32 value[] = { c->getFov() };
							if (ImGui::SliderFloat("FovY", value, 0, 180)) {
								c->setFov(value[0]);
							}
						}
						{
							f32 value[] = { c->getClipRange().min,c->getClipRange().max };
							if (ImGui::SliderFloat2("ClipRange", value, 0, 10000)) {
								c->setClipRange({ value[0],value[1] });
							}
						}
						{
							auto rect = c->getVieportRect();
							f32 values[] = { rect.left,rect.top,rect.right,rect.bottom };
							if (ImGui::DragFloat4("ViewportRect", values, 0.01f, 0.0f, 1.0f)) {
								c->setVieportRect({ values[0] ,values[1],values[2] ,values[3] });
							}
						}
						{
							auto value = c->getClearColor();
							if (ImGui::ColorEdit3("ClearColor", value)) {
								c->setClearColor(value);
							}
						}
					}

				}
			}
		}

	}
	ImGui::End();
}