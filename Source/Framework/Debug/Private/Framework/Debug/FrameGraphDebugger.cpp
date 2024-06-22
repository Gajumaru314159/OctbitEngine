//***********************************************************
//! @file
//! @brief		マウス
//! @author		Gajumaru
//***********************************************************
#include <Framework/Debug/FrameGraphDebugger.h>
#include <Plugins/ImGui/ImGui.h>
#include <Plugins/ImGui/ImGuiRAII.h>

namespace ob::debug {

	FrameGraphDebugger::FrameGraphDebugger() {

	}

	void FrameGraphDebugger::update() {
		if (auto graphics = graphics::Graphics::Get()) {
			if (m_captureRequested) {
				m_fgdata = graphics->getFGData();
				m_captureRequested = false;
			}
		}
	}

	void FrameGraphDebugger::draw() {
		if (ImGui::Begin("Graphisc")) {
			if (ImGui::Button("Capture")) {
				m_captureRequested = true;
			}
			static ImGuiTableFlags table_flags =
				ImGuiTableFlags_SizingFixedFit |
				ImGuiTableFlags_ScrollX |
				ImGuiTableFlags_ScrollY |
				ImGuiTableFlags_BordersOuter |
				ImGuiTableFlags_BordersInner |
				ImGuiTableFlags_HighlightHoveredColumn;

			if (!m_fgdata.passes.empty()) {
				if (ImGui::BeginTable("FrameGraph", m_fgdata.passes.size() + 1, table_flags))
				{
					ImGui::TableSetupColumn("##Resources", ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder);

					for (auto [index, pass] : Indexed(m_fgdata.passes)) {
						if (pass.culled) ImGui::PushStyleColor(ImGuiCol_Text, Color::Gray.toVec4());
						ImGui::TableSetupColumn(pass.name.c_str(), ImGuiTableColumnFlags_AngledHeader | ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize);
						if (pass.culled) ImGui::PopStyleColor();
						// if (ImGui::IsItemHovered()) {
						// 	ImGui::SetTooltip(
						// 		Format(
						// 			"{}\n"
						// 			"Culled:{}",
						// 			pass.name,
						// 			pass.culled?"〇":"×"
						// 		).c_str()
						// 	);
						// }
					}

					ImGui::TableSetupScrollFreeze(1, 1);
					ImGui::TableAngledHeadersRow();

					for (auto& [id, resource] : m_fgdata.resources)
					{
						HashMap<u32, u32> rw;
						for (auto& writer : resource.writers) {
							rw[writer]++;
						}
						for (auto& reader : resource.readers) {
							rw[reader]++;
						}

						ImColor bgColor = ImColor(ImGui::GetStyleColorVec4(ImGuiCol_TableHeaderBg));
						if (resource.transient) bgColor.Value.x *= 0.5f;

						ImGui::PushID(id);
						ImGui::TableNextRow();
						ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, bgColor, 0);
						ImGui::TableSetColumnIndex(0);
						ImGui::Text(resource.name.c_str());

						if (ImGui::IsItemHovered()) {
							ImGui::SetTooltip(
								Format(
									"{}\n"
									"Transient:{}",
									resource.desc,
									resource.transient ? "〇" : "×"
								).c_str()
							);
						}

						for (auto& writer : resource.writers) {
							if (rw[writer] == 2)continue;
							if (ImGui::TableSetColumnIndex(writer + 1))
							{
								ImGui::Text("W");
							}
						}
						for (auto& reader : resource.readers) {
							if (rw[reader] == 2)continue;
							if (ImGui::TableSetColumnIndex(reader + 1))
							{
								ImGui::Text("R");
							}
						}
						for (auto [id, count] : rw) {
							if (count != 2)continue;
							if (ImGui::TableSetColumnIndex(id + 1))
							{
								ImGui::Text("M");
							}
						}
						ImGui::PopID();
					}
					ImGui::EndTable();
				}
			}

		}
		ImGui::End();
	}

}