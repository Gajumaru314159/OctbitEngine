//***********************************************************
//! @file
//! @brief		ImGui
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

namespace ImGui {
	inline namespace ob {

		using namespace ::ob;

		//@―---------------------------------------------------------------------------
		//! @brief  ImGuiで使用する形式のString
		//@―---------------------------------------------------------------------------
		using ImString = StringBase<char>;
		using ImStringView = StringViewBase<char>;

		//@―---------------------------------------------------------------------------
		//! @brief  起動
		//@―---------------------------------------------------------------------------
		bool Startup(const ob::platform::Window& window, const Ref<rhi::RenderPass>& renderPass, s32 subpass = 0);

		//@―---------------------------------------------------------------------------
		//! @brief  終了
		//@―---------------------------------------------------------------------------
		void Shutdown();


		//@―---------------------------------------------------------------------------
		//! @brief  フレーム開始
		//@―---------------------------------------------------------------------------
		void BeginFrame();

		//@―---------------------------------------------------------------------------
		//! @brief  フレーム終了
		//@―---------------------------------------------------------------------------
		void EndFrame(const Ref<rhi::CommandList>& commandList);


		//@―---------------------------------------------------------------------------
		//! @brief		StringViewをImStringに変換
		//! @details	ImGuiはシングルスレッドで使用されることを前提にしています。
		//@―---------------------------------------------------------------------------
		const char* ToImChars(StringView text,s32* out_len = nullptr);

	}
}
