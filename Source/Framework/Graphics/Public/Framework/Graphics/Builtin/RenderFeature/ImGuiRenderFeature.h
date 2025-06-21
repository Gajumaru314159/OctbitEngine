//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/FrameGraph/FG.h>
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/Render/RenderPass.h>

namespace ob::graphics {

	OB_EVENT_NOTIFIER(ImGui);

	//! @brief      ImGui描画機能
	class ImGuiRenderFeature : public RenderFeature {
	public:

		OB_RTTI();

		ImGuiRenderFeature(RenderScene& scene);
		virtual ~ImGuiRenderFeature();

		//! @brief		タスクを追加
		void addTask(ImGuiHandle& handle, ImGuiDelegate func);

		//! @brief		タスクを実行
		void executeTasks();

	public:

		//! @brief		タスクを追加
		static void AddTask(RenderScene& scene, ImGuiHandle& handle, ImGuiDelegate func);
		static void AddTask(RenderView& view,ImGuiHandle& handle, ImGuiDelegate func);

	private:
		ImGuiNotifier m_notifier;
	};



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
		~ImGuiPass();

		//! @brief		描画
		Output render(FG& fg, RenderView& view, Input input) const;
	};

}