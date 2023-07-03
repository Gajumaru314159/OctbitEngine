//***********************************************************
//! @file
//! @brief		プロファイラ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Debug/Profiler.h>
#include <Plugins/ImGui/ImGui.h>

namespace ob::debug {

	Profiler::Profiler() {
        m_time = 0.0f;
        m_history = 5.0f;

	}

	void Profiler::update() {

		if (ImGui::Begin("Profiler")) {

            ImVec2 mouse = ImGui::GetMousePos();
            static float t = 0;
            auto dt = ImGui::GetIO().DeltaTime;
            auto fps = 1.0f / dt;
            t += dt;
            m_timeBuffer.add(t, fps);

            static float history = 5.0f;
            ImGui::SliderFloat("History", &history, 1, 10, "%.1f s");

            if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1, 150))) {
                ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_Lock);
                ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 100);
                ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
                ImPlot::PlotShaded("fps", &m_timeBuffer.data[0].x, &m_timeBuffer.data[0].y, m_timeBuffer.data.size(), -INFINITY, 0, m_timeBuffer.offset, sizeof(Vec2));
                ImPlot::EndPlot();
            }

		}
		ImGui::End();

	}

}