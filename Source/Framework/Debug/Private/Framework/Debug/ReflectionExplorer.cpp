//***********************************************************
//! @file
//! @brief		プロファイラ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Debug/ReflectionExplorer.h>
#include <Plugins/ImGui/ImGui.h>
#include <Framework/Core/Reflection/TypeInfoPrinter.h>

namespace ob::debug {

	ReflectionExplorer::ReflectionExplorer()
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

				TypeInfoManager::Visit([this](const TypeInfo& info) { draw(info); });

				ImGui::EndListBox();
			}

			if (auto info = TypeInfoManager::Find(m_selected)) {
				drawDetail(*info);
			}

			ImGui::End();
		}

	}

}