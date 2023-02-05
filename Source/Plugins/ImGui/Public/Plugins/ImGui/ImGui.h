//***********************************************************
//! @file
//! @brief		コンポーネント
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Library/imgui.h"
#include <Framework/Core/Core.h>
#include <Framework/Core/Utility/Ref.h>
#include <Framework/Platform/Window.h>
#include <Framework/RHI/Forward.h>

namespace ob::imgui {

	bool ImGui_Init(const ob::platform::Window& window);
	void ImGui_Shutdown();
	void ImGui_NewFrame();

	//void     ImGui_EnableDpiAwareness();
	//float    ImGui_GetDpiScaleForHwnd(void* hwnd);       // HWND hwnd
	//float    ImGui_GetDpiScaleForMonitor(void* monitor); // HMONITOR monitor
	//
	//void     ImGui_EnableAlphaCompositing(void* hwnd);   // HWND hwnd



	bool ImGui_DrawInit(const Ref<rhi::RenderPass>& renderPass,s32 subpass=0);
	void ImGui_DrawShutdown();
	void ImGui_DrawNewFrame();
	void ImGui_RenderDrawData(const Ref<rhi::CommandList>& commandList);


}// namespcae ob
