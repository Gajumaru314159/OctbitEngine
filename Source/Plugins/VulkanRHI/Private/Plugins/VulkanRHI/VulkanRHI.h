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
#include <Plugins/VulkanRHI/Texture/TextureUploader.h>

namespace ob::platform {
	class WindowManager;
}

namespace ob::rhi::vulkan {

	class BufferUploader;

	struct VulkanRHIConfig {
		bool enableDebugLayer = true;
	};


	struct VulkanFeatureInfo {

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

		void entryCommandList(const CommandList&) override;


		//===============================================================
		// 生成
		//===============================================================

		//! @brief  スワップ・チェーンを生成
		Ref<Display> createDisplay(const DisplayDesc& desc) override;


		//! @brief  コマンドリスト生成
		Ref<CommandList> createCommandList(const CommandListDesc& desc) override;


		//! @brief  ルートシグネチャを生成
		Ref<RootSignature> createRootSignature(const RootSignatureDesc& desc) override;


		//! @brief  パイプラインステートを生成
		Ref<PipelineState> createPipelineState(const PipelineStateDesc& desc) override;


		//! @brief  テクスチャを生成
		Ref<Texture> createTexture(const TextureDesc& desc) override;


		Ref<Texture> createTexture(StringView name, TextureType type, Size size, Span<const IntColor> colors) override;


		//! @brief  テクスチャを生成
		Ref<Texture> createTexture(StringView name, BlobView blob) override;


		//! @brief  レンダーテクスチャを生成
		Ref<RenderTexture> createRenderTexture(const RenderTextureDesc& desc) override;


		//! @brief  サンプラーを生成
		Ref<Sampler> createSampler(const SamplerDesc& desc) override;


		//! @brief  バッファーを生成
		Ref<Buffer> createBuffer(const BufferDesc& desc) override;


		//! @brief  シェーダをコンパイル
		Ref<Shader> compileShader(const ShaderCompileDesc& desc) override;


		//! @brief  シェーダをロード
		Ref<Shader> loadShader(BlobView, ShaderStage) override;


		//! @brief  デスクリプタ・テーブルを生成
		Ref<DescriptorTable>createDescriptorTable(const BindingSlot& desc) override;
		Ref<DescriptorTable> createDescriptorTable(const Ref<RootSignature>& signature, s32 slot)override;



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

		//! @brief サポートしているテクスチャフォーマットか 
		bool supports(TextureFormat format)const override;

		//! @brief サポートしているシェーダーステージか 
		bool supports(ShaderStage format)const override;

	public:

		const vk::PhysicalDeviceLimits& getLimits() const { return m_limits; }
		const vk::PhysicalDeviceFeatures& getFeatures() const { return m_features; }

		BufferUploader& getBufferUploader() { return *m_bufferUploader; }
		TextureUploader& getTextureUploader() { return *m_textureUploader; }

		VkMemoryAllocateInfo getAllocationInfo(vk::MemoryRequirements requirements, vk::MemoryPropertyFlags requestProps) {
			uint32_t memoryTypeIndex = (uint32_t)(-1);
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

			if (memoryTypeIndex == (uint32_t)(-1)) {
				throw Exception("Invalid Memory Requirements");
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

#ifdef OS_WINDOWS
		//! @brief  シェーダーコンパイラ―を取得
		ComPtr<IDxcCompiler3>& getShaderCompiler() { return m_shaderCompiler; }

		//! @brief  シェーダーインクルードハンドラーを取得
		ComPtr<IDxcIncludeHandler>& getIncludeHandler() { return m_shaderIncludeHandler; }
#endif

	private:

		void createInstance();
		void createPhysicalDevice();
		void createDevice();
		void createQueue();
		void createUploaders();
		void createShaderCompiler();

	private:

		RHIConfig								m_config;
		VulkanRHIConfig							m_vconfig;

		vk::Optional<const vk::AllocationCallbacks>	m_allocationCallbacks = nullptr;

		vk::raii::Context							m_context;
		vk::raii::Instance							m_instance			= nullptr;
		vk::raii::PhysicalDevice					m_physicalDevice	= nullptr;
		vk::raii::Device							m_device		= nullptr;
		vk::raii::Queue								m_queue				= nullptr;

#ifdef OS_WINDOWS
		ComPtr<IDxcCompiler3>				m_shaderCompiler;
		ComPtr<IDxcUtils>					m_shaderUtils;
		ComPtr<IDxcIncludeHandler>			m_shaderIncludeHandler;
#endif

		vk::PhysicalDeviceMemoryProperties			m_memoryProperties;

		u32											m_queueFamilyIndex;
		u32											m_queueCount;

		UPtr<BufferUploader> m_bufferUploader;
		UPtr<TextureUploader> m_textureUploader;

		vk::PhysicalDeviceFeatures				m_features;
		vk::PhysicalDeviceLimits				m_limits;

	};
}






//===============================================================
// インライン
//===============================================================
namespace ob::rhi::dx12 {

}