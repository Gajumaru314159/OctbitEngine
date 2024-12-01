//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/FrameGraph/FG.h>
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Platform/Window.h>

#include <Framework/RHI/CommandList.h>
#include <Plugins/ImGui/Library/imgui.h>
#include <Plugins/ImGui/Library/implot.h>

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
	class ImGuiRenderer {
	public:
		OB_RTTI();

		//! @brief		コンストラクタ
		ImGuiRenderer(RenderView& view);

		//! @brief		デストラクタ
		~ImGuiRenderer();

		//! @brief		描画
		bool render(FG& fg, FGTexture& target);

	private:

		//! @brief      コンテキストの初期化
		//! @details	ImGuiとImPlotのコンテキストを生成し、必要なオプションを設定する。
		void initializeContext();

		//! @brief      リソースの初期化
		//! @details	RootSignatureやシェーダはRenderFeatureで共用することも可能。
		//!				実装をシンプルにするためView毎に生成しています。
		void initializeResource();

		//! @brief      フォント画像生成
		void initializeFont();

		//! @brief      マウス更新
		void updateMouse(const platform::Window& window);

		//! @brief      キーボード更新
		void updateKeyboard(platform::Window& window);

		//! @brief      時間更新
		void updateTime();

		//! @brief		バッファ更新
		void updateBuffer();

		//! @brief		バッファ更新
		void updateCommand();

	private:

		RenderView&					m_view;

		struct DrawCommand {
			IntRect					rect;
			ImTextureID				texture;
			rhi::DrawIndexedParam	param;
		};

		ImGuiContext* m_imguiContext;
		ImPlotContext* m_implotContext;
		void* m_fontBlod = nullptr;

		ob::core::DateTime          m_time;

		Vector<DrawCommand>			m_commands;

		size_t						m_vertexCount = 0;
		size_t                      m_indexCount = 0;

		Ref<rhi::PipelineState>		m_pipeline;
		Ref<rhi::Buffer>			m_vertexBuffer;
		Ref<rhi::Buffer>			m_indexBuffer;
		Ref<rhi::Buffer>            m_constantBuffer;
		Ref<rhi::DescriptorTable>   m_constantTable;
		Ref<rhi::Texture>           m_fontTexture;
		Ref<rhi::DescriptorTable>   m_fontTextureTable;
	};

}