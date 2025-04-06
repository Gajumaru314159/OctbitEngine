//***********************************************************
//! @file
//! @brief		デバイス実装(Vulkan)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RHI.h>
#include <Framework/RHI/Config.h>
#include <Framework/RHI/Types/DescriptorDesc.h>

namespace ob::platform {
	class WindowManager;
}

namespace ob::rhi::vulkan {

	struct VulkanRHIConfig {
		bool enableDebugLayer = true;
	};

	class VulkanRHI :public RHI {
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		VulkanRHI(ob::platform::WindowManager&, GraphicObjectManager&, ob::rhi::RHIConfig*,VulkanRHIConfig*);
		~VulkanRHI();

		//! @brief  ゲーム更新イベント
		void update();


		//! @brief  妥当な状態か
		bool isValid()const;


		//===============================================================
		// 更新
		//===============================================================

		void entryCommandList(const CommandList&);


		//===============================================================
		// 生成
		//===============================================================

		//! @brief  スワップ・チェーンを生成
		Ref<Display> createDisplay(const DisplayDesc& desc);


		//! @brief  コマンドリスト生成
		Ref<CommandList> createCommandList(const CommandListDesc& desc);


		//! @brief  ルートシグネチャを生成
		Ref<RootSignature> createRootSignature(const RootSignatureDesc& desc);


		//! @brief  パイプラインステートを生成
		Ref<PipelineState> createPipelineState(const PipelineStateDesc& desc);


		//! @brief  テクスチャを生成
		Ref<Texture> createTexture(const TextureDesc& desc){ return {}; }


		Ref<Texture> createTexture(StringView name, Size size, Span<const IntColor> colors) { return {}; }


		//! @brief  テクスチャを生成
		Ref<Texture> createTexture(StringView name, BlobView blob){ return {}; }


		//! @brief  レンダーテクスチャを生成
		Ref<RenderTexture> createRenderTexture(const RenderTextureDesc& desc){ return {}; }


		//! @brief  サンプラーを生成
		Ref<Sampler> createSampler(const SamplerDesc& desc){ return {}; }


		//! @brief  バッファーを生成
		Ref<Buffer> createBuffer(const BufferDesc& desc) { return {}; }


		//! @brief  シェーダをコンパイル
		Ref<Shader> compileShader(const ShaderCompileDesc& desc) { return {}; }


		//! @brief  シェーダをロード
		Ref<Shader> loadShader(BlobView, ShaderStage) { return {}; }


		//! @brief  デスクリプタ・テーブルを生成
		Ref<DescriptorTable> createDescriptorTable(DescriptorRangeType type, s32 elementNum){ return {}; }



		//! @brief  GraphicFileHandleを生成
		Ref<GraphicFileHandle>  createGraphicFileHandle(StringView path) { return {}; }

		//! @brief  GraphicFileEventを生成
		Ref<GraphicFileEvent>   createGraphicFileEvent() { return {}; }

		//! @brief  GraphicFileQueueを生成
		Ref<GraphicFileQueue>   createGraphicFileQueue(const GraphicFileQueueDesc&) { return {}; }

		//! @brief  GraphicFileQueueを生成
		bool                    generateGraphicFile(StringView input, StringView output, s32 compressionLevel) { return {}; }

		//! @brief  プラットフォームごとのGraphicFileから事前情報を取得
		Vector<GraphicFileMipInfo> prepareGraphicFile(StringView path) override { return {}; }

	private:

		void createInstance();
		void createPhysicalDevice();
		void createLogicalDevice();
		void createQueue();

	private:

		RHIConfig								m_config;
		VulkanRHIConfig							m_vconfig;

		VkInstance			m_instance			= nullptr;
		VkPhysicalDevice	m_physicalDevice	= nullptr;
		VkDevice			m_logicalDevice		= nullptr;
		VkQueue				m_queue				= nullptr;
		VkCommandPool		m_commandPool		= nullptr;

		u32					m_queueFamilyIndex;
		u32					m_queueCount;

	};
}






//===============================================================
// インライン
//===============================================================
namespace ob::rhi::dx12 {

}