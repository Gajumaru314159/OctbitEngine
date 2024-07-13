#include <Framework/Core/Core.h>
#include <Framework/Engine/All.h>
#include <Plugins/ImGui/ImGui.h>

using namespace ob;

engine::Entity* s_selectedEntity = nullptr;
engine::Scene* s_selectedScene = nullptr;

void drawOutliner_Entity(engine::Entity* entity) {
	if (!entity)return;
	auto name = entity->getName().c_str();
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
void drawOutliner_Scene(engine::Scene* scene) {
	if (!scene)return;
	auto name = scene->getName().c_str();
	bool empty = scene->getChildren().empty() && scene->getEntities().empty();

	auto flag = empty ? ImGuiTreeNodeFlags_Leaf : 0;
	if (s_selectedScene == scene)flag |= ImGuiTreeNodeFlags_Selected;
	flag |= ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;

	bool opend = ImGui::TreeNodeEx(name, flag, name);

	if (ImGui::IsItemClicked()) {
		s_selectedScene = scene;
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
void drawOutliner(engine::Scene* scene) {

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
			ImGui::TextUnformatted(entity.getName().c_str());

			for (auto& component : entity.componets()) {
				auto cmpname = String(component->getComponentType().shortName());
				if (ImGui::CollapsingHeader(cmpname.c_str())) {

					ImGui::ScopedIndent indent;

					if (component->getType() == Type::Get<engine::TransformComponent>()) {
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

				}
			}
		}

	}
	ImGui::End();
}