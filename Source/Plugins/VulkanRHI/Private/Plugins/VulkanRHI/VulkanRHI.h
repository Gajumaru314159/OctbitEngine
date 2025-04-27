//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RHI.h>
#include <Framework/RHI/Config.h>
#include <Plugins/VulkanRHI/Buffer/VulkanBufferUploader.h>
#include <Plugins/VulkanRHI/Texture/VulkanTextureUploader.h>
#include <Plugins/VulkanRHI/Command/VulkanCommandQueue.h>
#include <Plugins/VulkanRHI/VulkanRHIConfig.h>

namespace ob::platform {
	class WindowManager;
}

namespace ob::rhi {

	struct VulkanFeatureInfo {
		bool debugMarkerEnabled = false;
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


		//! @brief	API名を取得
		String getAPIName()const { return "Vulkan"; };

		//===============================================================
		// 更新
		//===============================================================

		void entryCommandList(const Ref<CommandList>&) override;


		//===============================================================
		// 生成
		//===============================================================

		//! @brief  スワップ・チェーンを生成
		Ref<SwapChain> createSwapChain(const SwapChainDesc& desc) override;


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
		Ref<DescriptorLayout> createDescriptorLayout(const DescriptorLayoutDesc& desc) override;
		Ref<DescriptorTable> createDescriptorTable(const DescriptorTableDesc& desc)override;


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
		bool supports(TextureFormat format, TextureType type)const override;

		//! @brief サポートしているレンダーテクスチャーフォーマットか 
		bool supportsForRenderTexture(TextureFormat format)const override;

		//! @brief サポートしているシェーダーステージか 
		bool supports(ShaderStage format)const override;

	public:

		template<typename T>
		void setName(const T& object, StringView name) {
#if OB_DEBUG
			if (m_featuresEx.debugMarkerEnabled) {
			    vk::DebugUtilsObjectNameInfoEXT info;
				info.objectType = T::objectType;
				info.objectHandle = (uint64_t)((typename T::CType) * object);
			    info.pObjectName = name.data();			
				m_device.setDebugUtilsObjectNameEXT(info);
			}
#endif
		}

		const vk::PhysicalDeviceLimits& getLimits() const { return m_limits; }
		const vk::PhysicalDeviceFeatures& getFeatures() const { return m_features; }
		const VulkanFeatureInfo& getFeaturesEx() const { return m_featuresEx; }

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
		vk::Queue				getQueue() { return m_commandQueue->getQueue(); }

		vk::Optional<const vk::AllocationCallbacks>&	getAllocationCallbacks() { return m_allocationCallbacks; }

		vk::DescriptorSetLayout getBindlessDescriptorSetLayout() const {
			return m_bindlessDescriptorSetLayout;
		}
		vk::DescriptorSet getBindlessDescriptorSet() const {
			return m_bindlessDescriptorSet;
		}

#ifdef OS_WINDOWS
		//! @brief  シェーダーコンパイラ―を取得
		ComPtr<IDxcCompiler3>& getShaderCompiler() { return m_shaderCompiler; }

		//! @brief  シェーダーインクルードハンドラーを取得
		ComPtr<IDxcIncludeHandler>& getIncludeHandler() { return m_shaderIncludeHandler; }
#endif

		bool supportsDebugMarker() const {
			return m_vkDebugMarkerSetObjectNameEXT;
		}

	private:

		void createInstance();
		void createPhysicalDevice();
		void createDevice();
		void createQueue();
		void createUploaders();
		void createShaderCompiler();
		void initializeBindless();

	private:

		RHIConfig									m_config;
		VulkanRHIConfig								m_vconfig;

		vk::Optional<const vk::AllocationCallbacks>	m_allocationCallbacks = nullptr;

		vk::raii::Context							m_context;
		vk::raii::Instance							m_instance			= nullptr;
		vk::raii::PhysicalDevice					m_physicalDevice	= nullptr;
		vk::raii::Device							m_device		= nullptr;

#ifdef OS_WINDOWS
		ComPtr<IDxcCompiler3>						m_shaderCompiler;
		ComPtr<IDxcUtils>							m_shaderUtils;
		ComPtr<IDxcIncludeHandler>					m_shaderIncludeHandler;
#endif

		vk::PhysicalDeviceMemoryProperties			m_memoryProperties;

		u32											m_queueFamilyIndex;
		u32											m_queueCount;

		UPtr<VulkanCommandQueue>					m_commandQueue;

		UPtr<BufferUploader>						m_bufferUploader;
		UPtr<TextureUploader>						m_textureUploader;

		Ref<CommandList>							m_copyCommandList;

		vk::PhysicalDeviceFeatures					m_features;
		vk::PhysicalDeviceLimits					m_limits;
		VulkanFeatureInfo							m_featuresEx;

		vk::raii::DescriptorSetLayout				m_bindlessDescriptorSetLayout = nullptr;
		vk::raii::DescriptorPool					m_bindlessDescriptorPool = nullptr;
		vk::raii::DescriptorSet						m_bindlessDescriptorSet = nullptr;

#if OB_DEBUG
		PFN_vkDebugMarkerSetObjectNameEXT			m_vkDebugMarkerSetObjectNameEXT;
		PFN_vkCmdDebugMarkerBeginEXT 				m_vkCmdDebugMarkerBeginEXT;
		PFN_vkCmdDebugMarkerEndEXT 					m_vkCmdDebugMarkerEndEXT;
		PFN_vkCmdDebugMarkerInsertEXT				m_vkCmdDebugMarkerInsertEXT;
#endif
	};
}






//===============================================================
// インライン
//===============================================================
namespace ob::rhi {

}