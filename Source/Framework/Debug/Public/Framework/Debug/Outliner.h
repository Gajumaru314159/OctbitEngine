//***********************************************************
//! @file
//! @brief		プロファイラ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/TypeInfo.h>
#include <Framework/Engine/Forward.h>
#include <Framework/Engine/EntityHandle.h>
#include <Framework/Input/Button.h>
#include <Plugins/ImGui/ImGui.h>

namespace ob::debug {

	class Outliner {
	public:
		Outliner();
		void draw(engine::World& world);
	private:
		void draw(engine::Scene&);
		void draw(engine::Entity&);
		void drawComponents();
		void drawContextMenu();

		input::ButtonHandle& allocButtonHandle();

		void deleteEntity();
		void createEmptyEntity();
	private:
		ImGuiTextFilter m_filter;
		engine::Scene* m_selectedScene = nullptr;
		engine::EntityHandle m_selectedEntity;
		bool m_focused = false;

		ImGuiTextFilter m_componentFilter;
		Vector<UPtr<input::ButtonHandle>> m_buttons;
		Vector<const TypeInfo*> m_componentTypes;
	};

}