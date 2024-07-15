//***********************************************************
//! @file
//! @brief		プロファイラ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Debug/ReflectionExplorer.h>
#include <Plugins/ImGui/ImGui.h>
#include <Framework/Core/Reflection/TypeInfoPrinter.h>

namespace ob::debug {

	ReflectionExplorer::ReflectionExplorer(const TypeInfoManager& manager)
		: m_manager(manager)
	{

	}

	void ReflectionExplorer::draw(const TypeInfo& info) {
		m_buffer = info.type.name();
		if (ImGui::Selectable(m_buffer.data())) {
			m_selected = info.type;
		}
	}
	void ReflectionExplorer::drawDetail(const TypeInfo& info) {
		auto text = PrintTypeInfo(info);
		ImGui::Text(text.data());
	}

	void ReflectionExplorer::draw() {

		if (ImGui::Begin("ReflectionExplorer")) {

			if (ImGui::BeginListBox("Types")) {

				m_manager.visit([this](const TypeInfo& info) { draw(info); });

				ImGui::EndListBox();
			}

			if (auto info = m_manager.find(m_selected)) {
				drawDetail(*info);
			}

			ImGui::End();
		}

	}

}