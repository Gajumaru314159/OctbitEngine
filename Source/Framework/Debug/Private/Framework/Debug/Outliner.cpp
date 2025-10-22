//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Debug/Outliner.h>
#include <Framework/Engine/World.h>
#include <Framework/Engine/Scene.h>
#include <Framework/Engine/Entity.h>
#include <Framework/Engine/Component.h>
#include <Framework/Engine/Component/TransformComponent.h>
#include <Plugins/ImGui/ImGui.h>
#include <Plugins/ImGui/ImGuiRAII.h>
#include <Framework/Debug/Inspector.h>
#include <Framework/Core/Reflection/TypeInfoManager.h>

#include <Framework/Input/Keyboard.h>

namespace ob::debug {

	using namespace ob::engine;
	using namespace ob::input;

	Outliner::Outliner() {

		// TODO 修飾キーの対応
		Keyboard::X.bindDown(allocButtonHandle(), [&]() { if (m_focused && Keyboard::LeftCtrl.pressed())LOG_TRACE("Cut"); });
		Keyboard::C.bindDown(allocButtonHandle(), [&]() { if (m_focused && Keyboard::LeftCtrl.pressed())LOG_TRACE("Copy"); });
		Keyboard::V.bindDown(allocButtonHandle(), [&]() { if (m_focused && Keyboard::LeftCtrl.pressed() && !Keyboard::LeftShift.pressed())LOG_TRACE("Paste"); });
		Keyboard::V.bindDown(allocButtonHandle(), [&]() { if (m_focused && Keyboard::LeftCtrl.pressed() && Keyboard::LeftShift.pressed())LOG_TRACE("Paste As Child"); });

		Keyboard::F2.bindDown(allocButtonHandle(), [&]() { if (m_focused)LOG_TRACE("Rename"); });
		Keyboard::D.bindDown(allocButtonHandle(), [&]() { if (m_focused && Keyboard::LeftCtrl.pressed())LOG_TRACE("Deplicate");; });
		Keyboard::Delete.bindDown(allocButtonHandle(), [&]() { if (m_focused)deleteEntity(); });

		Keyboard::N.bindDown(allocButtonHandle(), [&]() { if (m_focused && Keyboard::LeftCtrl.pressed()) createEmptyEntity(); });

		// Component一覧を登録
		TypeInfoManager::Visit(
			[&](const TypeInfo& info) {
				if (!info.isSuperClassOf<Component>())return;
				if (!info.findConstructor<Entity&>()) return;
				m_componentTypes.push_back(&info);
			}
		);

	}
	input::ButtonHandle& Outliner::allocButtonHandle() {
		m_buttons.emplace_back() = std::make_unique<ButtonHandle>();
		return *m_buttons.back();
	}

	void Outliner::draw(World& world) {

		ImGui::ScopedStyleVar style(ImGuiStyleVar_IndentSpacing, 10);

		if (ImGui::Begin("Outliner")) {
			m_filter.Draw();
			for (auto& child : world.getRootScene().getChildren()) {
				draw(*child);
			}
		}
		m_focused = ImGui::IsWindowFocused();
		ImGui::End();

		drawComponents();
	}

	void Outliner::draw(Scene& scene) {

		ImGui::ScopedID sid(&scene);

		bool empty = scene.getChildren().empty() && scene.getEntities().empty();
		auto flag = empty ? ImGuiTreeNodeFlags_Leaf : 0;
		if (m_selectedScene == &scene) flag |= ImGuiTreeNodeFlags_Selected;
		flag |= ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;


		ImGui::PushStyleColor(ImGuiCol_Text,Color::Yellow.toVec4());
		bool opend = ImGui::TreeNodeEx(scene.getName().c_str(), flag);
		ImGui::PopStyleColor();

		if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1)) {
			m_selectedEntity = {};
			m_selectedScene = &scene;
		}

		drawContextMenu();

		if (opend) {
			for (auto& child : scene.getChildren()) {
				draw(*child);
			}

			for (auto& handle : scene.getEntities()) {
				if (auto child = handle.get()) {
					draw(*child);
				}
			}

			ImGui::TreePop();
		}


	}

	void Outliner::draw(Entity& entity) {

		ImGui::ScopedID sid(&entity);

		auto name = entity.getName().c_str();
		bool empty = entity.getChildren().empty();

		auto flag = empty ? ImGuiTreeNodeFlags_Leaf : 0;
		if (m_selectedEntity == entity.getHandle())flag |= ImGuiTreeNodeFlags_Selected;
		flag |= ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;

		bool opend = ImGui::TreeNodeEx(name, flag, name);

		if (ImGui::IsItemClicked(0) || ImGui::IsItemClicked(1)) {
			m_selectedEntity = entity.getHandle();
			m_selectedScene = nullptr;
		}

		drawContextMenu();

		if (opend) {

			for (auto child : entity.getChildren()) {
				if (child) {
					draw(*child);
				}
			}

			ImGui::TreePop();

		}

	}

	void Outliner::drawComponents() {

		if (ImGui::Begin("Inspector")) {

			debug::Inspector inspector;

			if (auto pEntity = m_selectedEntity.get()) {
				auto& entity = *pEntity;

				if (ImGui::CollapsingHeader("Entity")) {

					ImGui::ScopedIndent indent;

					Any obj(entity);

					inspector.draw(obj, Type::Get(entity));
				}

				HashSet<Type> requiredTypes = {Type::Get<TransformComponent>()};
				Vector<Component*> removeComponents;

				for (auto& component : entity.componets()) {
					if (auto info = TypeInfoManager::Find(component->getType())) {
						if (auto tag = info->findTag("Requirements")) {
							requiredTypes.emplace(Type(tag.value()));
						}
					}
				}

				for (auto& component : entity.componets()) {

					auto cmpname = String(component->getType().shortName());

					bool removable = requiredTypes.count(component->getType()) == 0;

					bool visible = true;
					bool* pVisible = removable?&visible:nullptr;

					if (ImGui::CollapsingHeader(cmpname.c_str(), pVisible)) {
						ImGui::ScopedIndent indent;
						Any obj(*component);
						inspector.draw(obj, obj.type());
					}

					if (visible == false) {
						removeComponents.push_back(component.get());
					}
				}
				for (auto component : removeComponents) {
					entity.removeComponent(component);
				}

				if (ImGui::BeginPopupContextItem("Add Component")) {

					HashSet<Type> existComponents;
					for(auto& component : entity.componets()) {
						existComponents.emplace(component->getType());
					};

					m_componentFilter.Draw();
					for (auto info : m_componentTypes) {
						String shortName(info->type.shortName());
						String name(info->type.name());
						String desc = name;

						if (existComponents.count(info->type) && !info->hasTag("Multiple")) {
							continue;
						}

						if (m_componentFilter.PassFilter(name.c_str())) {
							bool clicked = ImGui::MenuItem(shortName.c_str());
							if (ImGui::IsItemHovered()) {
								if (ImGui::BeginTooltip()) {
									ImGui::Text(name.c_str());
									ImGui::Separator();
									if (auto tag = info->findTag("Description")) {
										ImGui::Text("[Description]");
										ImGui::TextUnformatted(tag->data());
									}
									if (auto tag = info->findTag("Requirements")) {
										ImGui::Text("[Requirements]");
										ImGui::TextUnformatted(tag->data());
									}
									ImGui::EndTooltip();
								}
							}
							if (clicked) {
								pEntity->addComponent(info->type);
							}
						}
					}
					ImGui::EndPopup();
				}

				if (ImGui::Button("Add Component", { -1,0 })) {
					ImGui::OpenPopup("Add Component");
				}
			}

		}
		ImGui::End();
	}

	void Outliner::drawContextMenu() {

		if (ImGui::BeginPopupContextItem()) {

			if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
			if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
			if (ImGui::MenuItem("Paste", "Ctrl+V", nullptr, false)) {}
			if (ImGui::MenuItem("Paste As Child", "Ctrl+Shift+V", nullptr, false)) {}
			ImGui::Separator();

			if (ImGui::MenuItem("Rename", "F2")) {}
			if (ImGui::MenuItem("Duplicate", "Ctrl+D")) {}
			if (ImGui::MenuItem("Delete", "Delete")) {}
			ImGui::Separator();

			if (ImGui::MenuItem("Create Empty", "Ctrl+N")) { createEmptyEntity(); }
			if (ImGui::MenuItem("Create Empty Parent")) {}

			if (ImGui::BeginMenu("3D Object"))
			{
				if (ImGui::MenuItem("Model")) {}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Light"))
			{
				if (ImGui::MenuItem("Directional Light")) {}
				if (ImGui::MenuItem("Point Light")) {}
				if (ImGui::MenuItem("Spot Light")) {}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

	}

	void Outliner::deleteEntity() {
		if (auto entity = m_selectedEntity.get()) {
			entity->requestRelease();
			m_selectedEntity = {};
		}
	}
	void Outliner::createEmptyEntity() {
		auto parent = m_selectedEntity.get();
		Entity* entity = nullptr;
		if (parent || m_selectedEntity) {
			entity = Entity::Create("New Entity");
			entity->addComponent<TransformComponent>();
			entity->setActive(true);
		}

		if (parent) {
			parent->addChild(entity);
		} else if (m_selectedScene) {
			m_selectedScene->addEntity(entity);
		}
	}

};