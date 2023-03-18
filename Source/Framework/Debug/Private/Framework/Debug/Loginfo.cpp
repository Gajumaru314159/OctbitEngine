//***********************************************************
//! @file
//! @brief		マウス
//! @author		Gajumaru
//***********************************************************
#include <Framework/Debug/LogInfo.h>
#include <Plugins/ImGui/ImGui.h>

namespace ob::debug {

	LogInfo::LogInfo() {

		m_maxLogCount = 100;

		m_levelColors[LogLevel::Fatal] = Color::Magenta;
		m_levelColors[LogLevel::Error] = Color::Red;
		m_levelColors[LogLevel::Warning] = Color(1,0.8,0);
		m_levelColors[LogLevel::Info] = Color::Cyan;
		m_levelColors[LogLevel::Trace] = Color::Gray;

		m_levelFilter[LogLevel::Fatal] = true;
		m_levelFilter[LogLevel::Error] = true;
		m_levelFilter[LogLevel::Warning] = true;
		m_levelFilter[LogLevel::Info] = true;
		m_levelFilter[LogLevel::Trace] = true;

		Logger::Get().addEvent(m_hLogged,
			[this](const Log& log) {
				if (m_maxLogCount < m_logs.size()) {
					m_logs.pop_front();
				}
				auto& cache = m_logs.emplace_back();
				cache.datetime = DateTime::Now();
				cache.level = log.level;
				cache.message = log.message;
				cache.file = Format(TC("{}({})"), Path(log.sourceLocation.filePath).filename().u8string().c_str(), log.sourceLocation.line);
				cache.line = Format(TC("{}({})"),log.sourceLocation.filePath, log.sourceLocation.line);
				m_logs.emplace_back(std::move(cache));
			}
		);
	}

	void LogInfo::update() {

		String buffer;

		if (ImGui::Begin("LogInfo")) {

			ImGui::Checkbox("AutoWrap", &m_bAutoWrap);

			if (ImGui::Button("Clear")) {
				FormatTo(m_filter.InputBuf, "mesh");
				m_filter.Build();
				//m_logs.clear();
			}

			ImGui::SameLine();
			m_filter.Draw();
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("format: aaa[,bbb][,-ccc]\nex: rhi,graphic,-mesh");
			}

			auto drawLevelFilter = [this](LogLevel level, const char* label) {
				Color color = m_levelColors[level];
				Color hovered = Color::Lerp(color, Color::White, 0.1);
				if (m_levelFilter[level] == false) {
					color = Color::Lerp(color, Color(0.2), 0.8);
				}
				ImGui::ScopedTextColor stc(Color::Black);
				ImGui::ScopedButtonColor sbc(color);
				ImGui::ScopedButtonHoveredColor sbhc(hovered);

				if (ImGui::Button(label)) {
					m_levelFilter[level] = !m_levelFilter[level];
				}

				ImGui::SameLine();
			};

			drawLevelFilter(LogLevel::Fatal, "Fatal");
			drawLevelFilter(LogLevel::Error, "Error");
			drawLevelFilter(LogLevel::Warning, "Warning");
			drawLevelFilter(LogLevel::Info, "Info");
			drawLevelFilter(LogLevel::Trace, "Trace");

			ImGui::NewLine();

			ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable | ImGuiTableFlags_ScrollY;

			if (ImGui::BeginTable("LogTable", 4,flags)) {

				// ヘッダ
				ImGui::TableSetupColumn("Level", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize,60);
				ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed,100);
				ImGui::TableSetupColumn("Location", ImGuiTableColumnFlags_WidthFixed,100);
				ImGui::TableSetupScrollFreeze(0, 1);
				ImGui::TableHeadersRow();

				for (auto& [index, log] : ReverseIndexed(m_logs)) {

					if (m_levelFilter[log.level] == false)
						continue;

					if(!m_filter.PassFilter(log.message.c_str()))
						continue;


					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					{
						ImGui::ScopedTextColor stc(m_levelColors[log.level]);
						switch (log.level) {
						case LogLevel::Fatal:   ImGui::Text("Fatal"); break;
						case LogLevel::Error:   ImGui::Text("Error"); break;
						case LogLevel::Warning: ImGui::Text("Warning"); break;
						case LogLevel::Info:    ImGui::Text("Info"); break;
						case LogLevel::Trace:   ImGui::Text("Trace"); break;
						default:                ImGui::Text("Unknown"); break;
						}
					}

					ImGui::TableNextColumn();
					if (m_bAutoWrap) {
						ImGui::PushTextWrapPos(0.0f);
						ImGui::TextUnformatted(ImGui::ToImChars(log.message));
						ImGui::PopTextWrapPos();
					} else {
						ImGui::TextUnformatted(ImGui::ToImChars(log.message));
					}

					ImGui::TableNextColumn();
					ImGui::TextUnformatted(ImGui::ToImChars(Format(TC("{}"), log.datetime.toString("HH:mm:ss.ff"))));

					ImGui::TableNextColumn();
					ImGui::TextUnformatted(ImGui::ToImChars(log.file));

				}

				//s32 hovered_column = -1;
				//for (auto& [index, log] : Indexed(m_logs)) {
				//
				//	ImGui::ScopedID sid(index);
				//	if (ImGui::TableGetColumnFlags(index) & ImGuiTableColumnFlags_IsHovered)
				//		hovered_column = index;
				//	if (hovered_column == index && !ImGui::IsAnyItemHovered() && ImGui::IsMouseReleased(1))
				//		ImGui::OpenPopup("MyPopup");
				//	if (ImGui::BeginPopup("MyPopup"))
				//	{
				//		// TODO ログコピー機能
				//		if (index < m_logs.size())
				//			ImGui::Text("Copy");
				//
				//		if (ImGui::Button("Close"))
				//			ImGui::CloseCurrentPopup();
				//
				//		ImGui::EndPopup();
				//	}
				//}
				ImGui::EndTable();
			}

		}
		ImGui::End();

	}

}