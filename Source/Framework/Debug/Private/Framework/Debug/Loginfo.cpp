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
		m_levelColors[LogLevel::Trace] = Color(0.7f);

		m_levelNames[LogLevel::Fatal] = TC("Fatal");
		m_levelNames[LogLevel::Error] = TC("Error");
		m_levelNames[LogLevel::Warning] = TC("Warning");
		m_levelNames[LogLevel::Info] = TC("Info");
		m_levelNames[LogLevel::Trace] = TC("Trace");

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

				if (!m_logs.empty()) {
					auto& last = m_logs.back();
					if (last.level == log.level && last.message == log.message) {
						last.count++;
						return;
					}
				}

				auto& cache = m_logs.emplace_back();
				cache.datetime = DateTime::Now();
				cache.level = log.level;
				cache.message = log.message;
				cache.file = Format(TC("{}({})"), Path(log.sourceLocation.filePath).fileName(), log.sourceLocation.line);
				cache.line = Format(TC("{}({})"),log.sourceLocation.filePath, log.sourceLocation.line);
				cache.count = 1;
				
			}
		);
	}

	void LogInfo::update() {

		String buffer;

		if (ImGui::Begin("LogInfo")) {

			ImGui::Checkbox("AutoWrap", &m_bAutoWrap);

			if (ImGui::Button("Clear")) {
				m_logs.clear();
			}

			ImGui::SameLine();
			m_filter.Draw();
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("format: aaa[,bbb][,-ccc]\nex: rhi,graphic,-mesh");
			}

			auto drawLevelFilter = [this](LogLevel level) {
				Color color = m_levelColors[level];
				Color hovered = Color::Lerp(color, Color::White, 0.1);
				if (m_levelFilter[level] == false) {
					color = Color::Lerp(color, Color(0.2), 0.8);
				}
				ImGui::ScopedTextColor stc(Color::Black);
				ImGui::ScopedStyleVar ssv(ImGuiStyleVar_FrameRounding,12);
				ImGui::ScopedButtonColor sbc(color);
				ImGui::ScopedButtonHoveredColor sbhc(hovered);

				if (ImGui::Button(ImGui::ToImChars(m_levelNames[level]))) {
					m_levelFilter[level] = !m_levelFilter[level];
				}

				ImGui::SameLine();
			};

			drawLevelFilter(LogLevel::Fatal);
			drawLevelFilter(LogLevel::Error);
			drawLevelFilter(LogLevel::Warning);
			drawLevelFilter(LogLevel::Info);
			drawLevelFilter(LogLevel::Trace);

			ImGui::NewLine();

			ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable | ImGuiTableFlags_ScrollY;

			if (ImGui::BeginTable("LogTable",5,flags)) {

				// ヘッダ
				ImGui::TableSetupColumn("Level", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize,60);
				ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("Count", ImGuiTableColumnFlags_WidthFixed, 20);
				ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed,100);
				ImGui::TableSetupColumn("Location", ImGuiTableColumnFlags_WidthFixed, 100);
				ImGui::TableSetupScrollFreeze(0, 1);
				ImGui::TableHeadersRow();


				for (auto& [index, log] : ReverseIndexed(m_logs)) {

					if (m_levelFilter[log.level] == false)
						continue;

					if(!m_filter.PassFilter(log.message.c_str()))
						continue;

					auto rowHeight = ImGui::GetTextLineHeight();

					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					{
						ImGui::ScopedTextColor stc(m_levelColors[log.level]);
						ImGui::Text(ImGui::ToImChars(m_levelNames[log.level]));
					}

					ImGui::TableNextColumn();
					if (m_bAutoWrap) {
						ImGui::PushTextWrapPos(0.0f);
						ImGui::TextUnformatted(ImGui::ToImChars(log.message));
						auto width = ImGui::GetContentRegionAvail().x;
						update_max(rowHeight,ImGui::CalcTextSize(ImGui::ToImChars(log.message),0,false, width).y);
						ImGui::PopTextWrapPos();
					} else {
						ImGui::TextUnformatted(ImGui::ToImChars(log.message));
					}

					ImGui::TableNextColumn();
					ImGui::Text("%d", log.count);

					ImGui::TableNextColumn();
					ImGui::TextUnformatted(ImGui::ToImChars(Format(TC("{}"), log.datetime.toString(TC("HH:mm:ss.ff")))));

					ImGui::TableNextColumn();
					ImGui::TextUnformatted(ImGui::ToImChars(log.file));

					{
						ImGui::ScopedID sid(index);
						ImGui::SameLine();
						ImGui::Selectable("##Row", false, ImGuiSelectableFlags_SpanAllColumns, { 0,rowHeight });
						if (ImGui::BeginPopupContextItem("##Popup"))
						{
							if (ImGui::Selectable("Copy")) {
								ImGui::SetClipboardText(ImGui::ToImChars(Format(TC("[{}]\n{}\n{}\n{}"), m_levelNames[log.level],log.message, log.line, log.datetime.toString(TC("HH:mm:ss.ff")))));
							}
							ImGui::EndPopup();
						}
					}
				}

				ImGui::EndTable();
			}

		}
		ImGui::End();

	}

}