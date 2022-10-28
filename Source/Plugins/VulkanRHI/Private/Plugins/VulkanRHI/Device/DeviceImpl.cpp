//***********************************************************
//! @file
//! @brief		デバイス実装(Vulkan)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Device/DeviceImpl.h>
#include <Plugins/VulkanRHI/Device/Instance.h>
#include <Plugins/VulkanRHI/Device/DebugReportCallBack.h>


namespace {

	static const ob::StringView GetReportBitString(::VkDebugReportFlagsEXT flags)
	{
		if ((flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) != 0)
		{
			return TC("INFO");
		}
		if ((flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) != 0)
		{
			return TC("WARNING");
		}
		if ((flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) != 0)
		{
			return TC("PERFORMANCE_WARNING");
		}
		if ((flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) != 0)
		{
			return TC("ERROR");
		}
		if ((flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) != 0)
		{
			return TC("DEBUG");
		}
		return TC("");
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL
		DebugCallback(
			::VkDebugReportFlagsEXT      flags,
			::VkDebugReportObjectTypeEXT /*objectType*/,
			std::uint64_t              /*object*/,
			std::size_t                /*location*/,
			std::int32_t               /*messageCode*/,
			const char*                /*pLayerPrefix*/,
			const char* pMessage,
			void*                      /*pUserData*/)
	{
		using namespace ob;

		String message;
		StringEncoder::Encode(pMessage, message);

		LOG_ERROR("[{}]: {}", GetReportBitString(flags), message);
		return VK_FALSE;
	}

}

namespace ob::graphic::vulkan {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	DeviceImpl::DeviceImpl(FeatureLevel featureLevel)
		:m_featureLevel(featureLevel) {
		initialize();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	DeviceImpl::~DeviceImpl() {
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
	//! @brief  テクスチャを生成
	//@―---------------------------------------------------------------------------
	ob::graphic::ITexture* DeviceImpl::createTexture(BlobView blob) {
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

		const char* layerNames[] = {
#ifdef OB_DEBUG
			"VK_LAYER_LUNARG_standard_validation"
#endif	
		};

		const char* extensionNames [] =
		{
			VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
			VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef OS_WINDOWS
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif OS_ANDROID
			VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,
#elif OS_MAC
			VK_EXT_METAL_SURFACE_EXTENSION_NAME,
#endif
		};

		m_instance = std::make_unique<Instance>(TC("OctbitEngine"), layerNames,extensionNames);

		// RebugReportCallback
		//m_callback = std::make_unique<DebugReportCallback>(
		//	m_instance->Get(),
		//	VK_DEBUG_REPORT_ERROR_BIT_EXT |
		//	VK_DEBUG_REPORT_WARNING_BIT_EXT |
		//	VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
		//	VK_DEBUG_REPORT_INFORMATION_BIT_EXT,
		//	&DebugCallback);


		// 物理デバイス

		// サーフェイス

		// コマンドプール

		// コマンドバッファ

		// デスクリプタプール

		// キュー

		// スワップチェーン





		// 物理デバイス選択
		VkPhysicalDevice gpu = nullptr;

		uint32_t gpu_count = 0;
		vkEnumeratePhysicalDevices(m_instance->Get(), &gpu_count, nullptr);

		Array<VkPhysicalDevice> physDevs(gpu_count);
		auto result = vkEnumeratePhysicalDevices(m_instance->Get(), &gpu_count, physDevs.data());

		// 最 初 の デ バ イ ス を 使 ⽤ する.
		m_physicalDevice = physDevs[0];

		// メ モ リ プ ロ パ テ ィ を 取 得 し て おく.
		vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_physMemProps);





		std::vector<const char*> const device_layer_names =
		{
#if defined(_DEBUG)
			"VK_LAYER_KHRONOS_validation",
#endif
		};

		std::vector<const char*> const device_extension_names =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		};



		//VkPhysicalDeviceProperties gpu_property = {};
		//vkGetPhysicalDeviceProperties(gpu, &gpu_property);

		uint32_t propCount=1; // 個 数 は 取 得 済 み と する.
		Array<VkQueueFamilyProperties> props(propCount);
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &propCount, props.data());
		uint32_t graphicsQueue = ~0u;
		for (uint32_t i = 0; i < propCount; ++i)
		{
			if (props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				graphicsQueue = i; break;
			}
		}


		// 論理デバイス生成
		const float defaultQueuePriority(1.0f); 
		VkDeviceQueueCreateInfo devQueueCI{}; 
		devQueueCI.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; 
		devQueueCI.queueFamilyIndex = graphicsQueue; 
		devQueueCI.queueCount = 1;
		devQueueCI.pQueuePriorities = &defaultQueuePriority;

		// vkEnumerateDeviceExtensionPropertiesで拡張取得済とする.

		std::vector<VkExtensionProperties> devExtProps;
		std::vector<const char*> extensions; 
		for (const auto& v : devExtProps) { 
			extensions.push_back(v.extensionName);
		}
		VkDeviceCreateInfo ci{};
		ci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO; 
		ci.ppEnabledExtensionNames = extensions.data(); 
		ci.enabledExtensionCount = uint32_t(extensions.size()); 
		ci.pQueueCreateInfos = &devQueueCI; 
		ci.queueCreateInfoCount = 1; 
		vkCreateDevice(m_physicalDevice, &ci, nullptr, &m_device);


		// コマンドプール生成
		VkCommandPoolCreateInfo ci{}; 
		ci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO; 
		ci.queueFamilyIndex = m_graphicsQueueIndex; 
		ci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; 
		vkCreateCommandPool(m_device, &ci, nullptr, &m_commandPool);


		return false;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	Instance::~Instance()
	{
		if (m_instance) {
			::vkDestroyInstance(m_instance, nullptr);
		}
	}

	DebugReportCallback::~DebugReportCallback()
	{
		if (m_instance) {
			//::vkDestroyDebugReportCallbackEXT(m_instance, m_callback, nullptr);
		}
	}
}// namespace ob::graphic::vulkan