//***********************************************************
//! @file
//! @brief		コンポーネント
//! @author		Gajumaru
//***********************************************************
#pragma once
#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#include "Library/imgui.h"
#include <Framework/Core/Core.h>
#include <Framework/Core/Utility/Ref.h>
#include <Framework/Platform/Window.h>
#include <Framework/RHI/Forward.h>
#include <Plugins/ImGui/ImGuiWidgets.h>
#include <Plugins/ImGui/ImGuiRAII.h>

namespace ob::imgui {

	bool Startup(const ob::platform::Window& window, const Ref<rhi::RenderPass>& renderPass, s32 subpass = 0);
	void Shutdown();
	void BeginFrame();
	void EndFrame(const Ref<rhi::CommandList>& commandList);

}// namespcae ob
