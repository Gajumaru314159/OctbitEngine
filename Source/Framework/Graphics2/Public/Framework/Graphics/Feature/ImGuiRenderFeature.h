//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderFeature.h>

#include <Framework/Graphics/FrameGraph/FG.h>
#include <Framework/Graphics/Render/RenderStep.h>

#include <Framework/RHI/CommandList.h>
#include <Plugins/ImGui/Library/imgui.h>
#include <Plugins/ImGui/Library/implot.h>

namespace ob::graphics {

	OB_EVENT_NOTIFIER(ImGui);

	//@―---------------------------------------------------------------------------
	//! @brief      ImGui描画機能
	//@―---------------------------------------------------------------------------
	class ImGuiRenderFeature : public RenderFeature{
	public:

		OB_RTTI();

		ImGuiRenderFeature(RenderScene& scene);
		virtual ~ImGuiRenderFeature();

		//@―---------------------------------------------------------------------------
		//! @brief      RenderViewごとに必要な描画ステップを追加する
		//@―---------------------------------------------------------------------------
		void createSteps(RenderStepInjector& injector) override;


		//@―---------------------------------------------------------------------------
		//! @brief		描画タスクを追加
		//@―---------------------------------------------------------------------------
		void addTask(ImGuiHandle& handle,ImGuiDelegate func);

	private:
		ImGuiNotifier m_notifier;
	};



	//@―---------------------------------------------------------------------------
	//! @brief		RenderView毎のImGui描画処理
	//@―---------------------------------------------------------------------------
	class ImGuiStep : public RenderStep {
	public:
		OB_RTTI();

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		ImGuiStep(RenderView& view,Action draw);

		//@―---------------------------------------------------------------------------
		//! @brief		デストラクタ
		//@―---------------------------------------------------------------------------
		~ImGuiStep();

		//@―---------------------------------------------------------------------------
		//! @brief		描画
		//@―---------------------------------------------------------------------------
		void render(FG& fg, FrameGraphResource target);

	private:

		//@―---------------------------------------------------------------------------
		//! @brief      コンテキストの初期化
		//! @details	ImGuiとImPlotのコンテキストを生成し、必要なオプションを設定する。
		//@―---------------------------------------------------------------------------
		void initializeContext();

		//@―---------------------------------------------------------------------------
		//! @brief      リソースの初期化
		//! @details	RootSignatureやシェーダはRenderFeatureで共用することも可能。
		//!				実装をシンプルにするためView毎に生成しています。
		//@―---------------------------------------------------------------------------
		void initializeResource();

		//@―---------------------------------------------------------------------------
		//! @brief      フォント画像生成
		//@―---------------------------------------------------------------------------
		void initializeFont();

		//@―---------------------------------------------------------------------------
		//! @brief      マウス更新
		//@―---------------------------------------------------------------------------
		void updateMouse();

		//@―---------------------------------------------------------------------------
		//! @brief      キーボード更新
		//@―---------------------------------------------------------------------------
		void updateKeyboard();

		//@―---------------------------------------------------------------------------
		//! @brief      時間更新
		//@―---------------------------------------------------------------------------
		void updateTime();

		//@―---------------------------------------------------------------------------
		//! @brief		バッファ更新
		//@―---------------------------------------------------------------------------
		void updateBuffer();

		//@―---------------------------------------------------------------------------
		//! @brief		バッファ更新
		//@―---------------------------------------------------------------------------
		void updateCommand();

	private:

		Action m_draw;

		struct DrawCommand {
			IntRect					rect;
			ImTextureID				texture;
			rhi::DrawIndexedParam	param;
		};

		ImGuiContext* m_imguiContext;
		ImPlotContext* m_implotContext;
		void* m_fontBlod = nullptr;

		ob::platform::Window        m_window;
		ob::core::DateTime          m_time;

		Array<DrawCommand>			m_commands;

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