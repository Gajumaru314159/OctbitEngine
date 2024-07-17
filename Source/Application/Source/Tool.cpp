#include <Framework/Core/Core.h>
#include <Framework/Engine/All.h>
#include <Plugins/ImGui/ImGui.h>
#include <Framework/Core/Reflection/TypeInfoManager.h>
#include <Framework/Debug/Inspector.h>

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

	debug::Inspector inspector;

	if (pEntity == nullptr) {
		pEntity = s_selectedEntity;
	}

	if (ImGui::Begin("Inspector")) {
		if (pEntity) {
			auto& entity = *pEntity;

			if (ImGui::CollapsingHeader("Entity")) {

				ImGui::ScopedIndent indent;

				AnyReference obj(entity);

				inspector.draw(obj, Type::Get(entity));
			}

			for (auto& component : entity.componets()) {

				auto cmpname = String(component->getType().shortName());
				if (ImGui::CollapsingHeader(cmpname.c_str())) {

					ImGui::ScopedIndent indent;

					AnyReference obj(*component);

					inspector.draw(obj,component->getType());

				}
			}
		}

	}
	ImGui::End();
}