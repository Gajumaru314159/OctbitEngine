//***********************************************************
//! @file
//! @brief		ImGui
//! @author		Gajumaru
//***********************************************************
#pragma once
#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#define IMGUI_DISABLE_OBSOLETE_KEYIO
#include "Library/imgui.h"
#include <Framework/Core/Core.h>
#include <Framework/Core/Utility/Ref.h>
#include <Framework/Platform/Window.h>
#include <Framework/RHI/Forward.h>
#include <Plugins/ImGui/ImGuiWidgets.h>
#include <Plugins/ImGui/ImGuiRAII.h>
#include <Plugins/ImGui/ImGuiUtil.h>

namespace ImGui {

	//@―---------------------------------------------------------------------------
	//! @brief  ImGuiで使用する形式のString
	//@―---------------------------------------------------------------------------
	using ImString = ob::StringBase<char>;
	using ImStringView = ob::StringViewBase<char>;

	//@―---------------------------------------------------------------------------
	//! @brief  起動
	//@―---------------------------------------------------------------------------
	bool StartupImGui(const ob::platform::Window& window, const ob::Ref<rhi::RenderPass>& renderPass, s32 subpass = 0);

	//@―---------------------------------------------------------------------------
	//! @brief  終了
	//@―---------------------------------------------------------------------------
	void ShutdownImGui();


	//@―---------------------------------------------------------------------------
	//! @brief  フレーム開始
	//@―---------------------------------------------------------------------------
	void BeginFrame();

	//@―---------------------------------------------------------------------------
	//! @brief  フレーム終了
	//@―---------------------------------------------------------------------------
	void EndFrame(const ob::Ref<rhi::CommandList>& commandList);


	//@―---------------------------------------------------------------------------
	//! @brief		StringViewをImStringに変換
	//! @details	ImGuiはシングルスレッドで使用されることを前提にしています。
	//@―---------------------------------------------------------------------------
	const char* ToImChars(ob::StringView text,s32* out_len = nullptr);

}
