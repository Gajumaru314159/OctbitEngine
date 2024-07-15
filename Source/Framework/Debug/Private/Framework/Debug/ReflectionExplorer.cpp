//***********************************************************
//! @file
//! @brief		プロファイラ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Debug/ReflectionExplorer.h>
#include <Plugins/ImGui/ImGui.h>

namespace ob::debug {

	ReflectionExplorer::ReflectionExplorer(const TypeInfoManager& manager)
		: m_manager(manager)
	{

	}

	void ReflectionExplorer::draw(const TypeInfo& info) {
		m_buffer = info.type.name();
		ImGui::Text(m_buffer.data());
	}

	void ReflectionExplorer::draw() {

		if (ImGui::Begin("ReflectionExplorer")) {

			if (ImGui::BeginListBox("Types")) {

				m_manager.visit([this](const TypeInfo& info) { draw(info); });

				ImGui::EndListBox();
			}

			ImGui::End();
		}

	}

}