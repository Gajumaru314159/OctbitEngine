//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderPass.h>

namespace ob::graphics {

	OB_EVENT_NOTIFIER(ImGui);

	//! @brief		RenderView毎のImGui描画処理
	class ImGuiPass : public RenderPass {
	public:
		struct Input {
			FGResource color;
		};
		struct Output {
			FGResource color;
		};
	public:
		OB_RTTI();

		//! @brief		コンストラクタ
		ImGuiPass();

		//! @brief		デストラクタ
		~ImGuiPass() override;

		//! @brief		描画
		Output render(FG& fg, RenderView& view, const Input& input) const;
	public:
		static void AddTask(RenderView& view, ImGuiHandle& handle, ImGuiDelegate func);
	};

}