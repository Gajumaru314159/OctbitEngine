//***********************************************************
//! @file
//! @brief		デバイス実装(Vulkan)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RHI.h>
#include <Framework/RHI/Config.h>
#include <Framework/RHI/Types/DescriptorDesc.h>
#include <Plugins/VulkanRHI/Buffer/BufferUploader.h>

namespace ob::platform {
	class WindowManager;
}

namespace ob::rhi::vulkan {

	class BufferUploader;

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

	public:

		BufferUploader& getBufferUploader() { return *m_bufferUploader; }

		VkMemoryAllocateInfo getAllocationInfo(vk::MemoryRequirements requirements, vk::MemoryPropertyFlags requestProps) {
			uint32_t memoryTypeIndex;
			auto requestBits = requirements.memoryTypeBits;
			for (uint32_t i = 0; i < m_memoryProperties.memoryTypeCount; ++i)
			{
				if (requestBits & 1)
				{
					const auto& types = m_memoryProperties.memoryTypes[i];
					if ((types.propertyFlags & requestProps) == requestProps)
					{
						memoryTypeIndex = i; break;
					}
				}
				requestBits >>= 1;
			}

			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = requirements.size;
			allocInfo.memoryTypeIndex = memoryTypeIndex;

			return allocInfo;
		}

		auto getQueryFamilyIndex() const { return m_queueFamilyIndex; }

		vk::raii::Instance&			getInstance() { return m_instance; }
		vk::raii::Device&			getDevice() { return m_device; }
		vk::raii::PhysicalDevice&	getPhysicalDevice() { return m_physicalDevice; }
		vk::raii::Queue&			getQueue() { return m_queue; }


		vk::Optional<const vk::AllocationCallbacks>&	getAllocationCallbacks() { return m_allocationCallbacks; }

	private:

		void createInstance();
		void createPhysicalDevice();
		void createDevice();
		void createQueue();

	private:

		RHIConfig								m_config;
		VulkanRHIConfig							m_vconfig;

		vk::Optional<const vk::AllocationCallbacks>	m_allocationCallbacks = nullptr;

		vk::raii::Context							m_context;
		vk::raii::Instance							m_instance			= nullptr;
		vk::raii::PhysicalDevice					m_physicalDevice	= nullptr;
		vk::raii::Device							m_device		= nullptr;
		vk::raii::Queue								m_queue				= nullptr;

		vk::PhysicalDeviceMemoryProperties			m_memoryProperties;

		u32											m_queueFamilyIndex;
		u32											m_queueCount;

		UPtr<BufferUploader> m_bufferUploader;

	};
}






//===============================================================
// インライン
//===============================================================
namespace ob::rhi::dx12 {

}