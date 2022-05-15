//***********************************************************
//! @file
//! @brief		デバイス実装(Vulkan)
//! @author		Gajumaru
//***********************************************************
#include "DeviceImpl.h"


namespace ob::graphic::vulkan {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	DeviceImpl::DeviceImpl(FeatureLevel featureLevel)
		:m_featureLevel(featureLevel) {
		initialize();
	}

	DeviceImpl::~DeviceImpl() {
		if(m_instance)vkDestroyInstance(m_instance, nullptr);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  妥当な状態か
	//@―---------------------------------------------------------------------------
	bool DeviceImpl::isValid()const{
		return false;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  コマンドをシステムキューに追加
	//@―---------------------------------------------------------------------------
	void DeviceImpl::entryCommandList(const CommandList& commandList) {
		OB_NOTIMPLEMENTED();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  更新
	//@―---------------------------------------------------------------------------
	void DeviceImpl::update() {
		OB_NOTIMPLEMENTED();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  スワップ・チェーンを生成
	//@―---------------------------------------------------------------------------
	ISwapChain* DeviceImpl::createSwapChain(const SwapchainDesc& desc) {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  コマンドリストを生成
	//@―---------------------------------------------------------------------------
	ICommandList* DeviceImpl::createCommandList(const CommandListDesc& desc) {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ルートシグネチャを生成
	//@―---------------------------------------------------------------------------
	IRootSignature* DeviceImpl::createRootSignature(const RootSignatureDesc& desc) {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  パイプラインステートを生成
	//@―---------------------------------------------------------------------------
	IPipelineState* DeviceImpl::createPipelineState(const PipelineStateDesc& desc) {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  テクスチャを生成
	//@―---------------------------------------------------------------------------
	ob::graphic::ITexture* DeviceImpl::createTexture(const TextureDesc& desc) {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  レンダーテクスチャを生成
	//@―---------------------------------------------------------------------------
	ob::graphic::IRenderTarget* DeviceImpl::createRenderTarget(const RenderTargetDesc& desc) {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  バッファーを生成
	//@―---------------------------------------------------------------------------
	ob::graphic::IBuffer* DeviceImpl::createBuffer(const BufferDesc& desc) {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  頂点シェーダを生成
	//@―---------------------------------------------------------------------------
	ob::graphic::IShader* DeviceImpl::createShader(const String& code, ShaderStage stage) {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  頂点シェーダを生成
	//@―---------------------------------------------------------------------------
	ob::graphic::IShader* DeviceImpl::createShader(const Blob& binary, ShaderStage stage) {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  デスクリプタ・テーブルを生成
	//@―---------------------------------------------------------------------------
	ob::graphic::IDescriptorTable* DeviceImpl::createDescriptorTable(DescriptorHeapType type, s32 elementNum) {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  初期化
	//@―---------------------------------------------------------------------------
	bool DeviceImpl::initialize() {

		VkResult result;

		// インスタンス生成
		{
			VkApplicationInfo appInfo{};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.apiVersion = VK_API_VERSION_1_1;
			appInfo.pApplicationName = "OctbitEngine";

			// レ イ ヤ ー. 検 証 レ イ ヤ ー を 有 効 化
			const char* layers[] = { "VK_LAYER_LUNARG_standard_validation" };
			VkInstanceCreateInfo ci{};
			ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			ci.pApplicationInfo = &appInfo;
			ci.enabledLayerCount = 1;
			ci.ppEnabledLayerNames = layers;
			result = vkCreateInstance(&ci, nullptr, &m_instance);
			if (VK_SUCCESS != result) {
				OB_ASSERT("vkCreateInstance()");
			}
		}

		// 物理デバイス選択
		{
			VkPhysicalDevice gpu = nullptr;

			uint32_t gpu_count = 0;
			vkEnumeratePhysicalDevices(m_instance, &gpu_count, nullptr);

			std::vector<VkPhysicalDevice> physDevs(gpu_count);
			vkEnumeratePhysicalDevices(m_instance, &gpu_count, physDevs.data());


			VkPhysicalDeviceProperties gpu_property = {};
			vkGetPhysicalDeviceProperties(gpu, &gpu_property);



			// 最 初 の デ バ イ ス を 使 ⽤ する.
			m_physicalDevice = physDevs[0];
			// メ モ リ プ ロ パ テ ィ を 取 得 し て おく.
			vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_physMemProps);

			uint32_t propCount=1; // 個 数 は 取 得 済 み と する.
			std::vector<VkQueueFamilyProperties> props(propCount);
			vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &propCount, props.data());
			uint32_t graphicsQueue = ~0u;
			for (uint32_t i = 0; i < propCount; ++i)
			{
				if (props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					graphicsQueue = i; break;
				}
			}
		}

		return false;
	}

}// namespace ob::graphic::vulkan