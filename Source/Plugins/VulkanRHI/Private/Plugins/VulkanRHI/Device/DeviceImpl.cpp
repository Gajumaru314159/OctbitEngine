//***********************************************************
//! @file
//! @brief		デバイス実装(Vulkan)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/Device/DeviceImpl.h>
#include <Plugins/VulkanRHI/Display/DisplayImpl.h>
#include <Plugins/VulkanRHI/Shader/ShaderImpl.h>
#include <Plugins/VulkanRHI/RenderPass/RenderPassImpl.h>

#include <Framework/Platform/Window.h>

namespace ob::rhi::vulkan {

	/*
	//@―---------------------------------------------------------------------------
	//! @brief  レポートフラグからログカテゴリを取得
	//@―---------------------------------------------------------------------------
	static const StringView GetReportBitString(::VkDebugReportFlagsEXT flags)
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
		return TC("OTHER");
	}

	//@―---------------------------------------------------------------------------
	//! @brief  デバッグレイヤのコールバック
	//@―---------------------------------------------------------------------------
	static VKAPI_ATTR VkBool32 VKAPI_CALL
		DebugCallback(
			::VkDebugReportFlagsEXT			flags,
			::VkDebugReportObjectTypeEXT	objectType,
			uint64_t						object,
			size_t							location,
			int32_t							messageCode,
			const char*						pLayerPrefix,
			const char* pMessage,
			void*							pUserData)
	{
		using namespace ob;

		String message;
		StringEncoder::Encode(pMessage, message);

		LOG_ERROR("[{}]: {}", GetReportBitString(flags), message);
		return VK_FALSE;
	}
	*/

	//@―---------------------------------------------------------------------------
	//! @brief  利用可能なレイヤー名のリストを取得
	//@―---------------------------------------------------------------------------
	static Set<std::string> EnumerateInstanceLayerNames() noexcept
	{
		uint32_t propertyCount;
		::vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);
		Array<::VkLayerProperties> properties(propertyCount);
		::vkEnumerateInstanceLayerProperties(&propertyCount, properties.data());

		Set<std::string> names;
		for (auto const& prop : properties)
		{
			names.emplace(prop.layerName);
		}

		return std::move(names);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  利用可能な拡張機能のリストを取得
	//@―---------------------------------------------------------------------------
	static Set<std::string> EnumerateInstanceExtensionNames(Span<const char*> layers) noexcept
	{
		auto enumarate = [](Set<std::string>& names, const char* layerName) {
			uint32_t propertyCount;
			::vkEnumerateInstanceExtensionProperties(layerName, &propertyCount, nullptr);
			Array<::VkExtensionProperties> properties(propertyCount);
			::vkEnumerateInstanceExtensionProperties(layerName, &propertyCount, properties.data());
			for (auto& name : properties)names.emplace(name.extensionName);
		};

		Set<std::string> names;

		enumarate(names, nullptr);

		for (auto& layer : layers) {
			enumarate(names, layer);
		}

		return std::move(names);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  利用可能なGPUのリストを取得
	//@―---------------------------------------------------------------------------
	static Array<VkPhysicalDevice> EnumerateDevices(VkInstance instance) noexcept
	{
		OB_CHECK_ASSERT_EXPR(instance != nullptr);

		uint32_t physicalDeviceCount = 0;
		::vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
		Array<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
		::vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

		return std::move(physicalDevices);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  物理デバイスの性能スコアを計算
	//@―---------------------------------------------------------------------------
	static s32 ComputeScoreDeviceSuitability(const VkPhysicalDevice& physicalDevice) noexcept
	{
		VkPhysicalDeviceProperties props;
		::vkGetPhysicalDeviceProperties(physicalDevice, &props);

		VkPhysicalDeviceFeatures features;
		::vkGetPhysicalDeviceFeatures(physicalDevice, &features);

		// スコア計算
		s32 score = 0;
		{
			// 最大テクスチャサイズ
			score += props.limits.maxImageDimension2D;
			// GPUタイプ
			if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				score += 1000;
			}
		}

		return score;
	}


}

namespace ob::rhi::vulkan {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	DeviceImpl::DeviceImpl(FeatureLevel featureLevel)
		:m_featureLevel(featureLevel) {

		createInstance();
		createPhysicalDevice();
		createLogicalDevice();
		createQueue();

		if (isValid() == false) {
			LOG_ERROR("Vulkanの初期化に失敗");
		}

	}

	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	DeviceImpl::~DeviceImpl() {

		if (m_commandPool) {
			::vkDestroyCommandPool(m_logicalDevice, m_commandPool, nullptr);
			m_commandPool = nullptr;
		}

		if (m_logicalDevice) {
			::vkDestroyDevice(m_logicalDevice, nullptr);
			m_logicalDevice = nullptr;
		}

		if (m_instance) {
			::vkDestroyInstance(m_instance, nullptr);
			m_instance = nullptr;
		}

		m_physicalDevice = nullptr;
		m_queue = nullptr;

	}


	//@―---------------------------------------------------------------------------
	//! @brief  妥当な状態か
	//@―---------------------------------------------------------------------------
	bool DeviceImpl::isValid()const {
		return 
			m_instance != nullptr &&
			m_physicalDevice != nullptr &&
			m_logicalDevice != nullptr &&
			m_queue != nullptr &&
			true;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  VkInstance生成
	//@―---------------------------------------------------------------------------
	void DeviceImpl::createInstance() {

		// レイヤー
		const char* layerNames[] = {
			OB_DEBUG_CONTEXT("VK_LAYER_KHRONOS_validation")
		};
		// 拡張機能名
		const char* extensionNames[] =
		{
			VK_EXT_DEBUG_REPORT_EXTENSION_NAME,							// デバッグレポート
			VK_KHR_SURFACE_EXTENSION_NAME,								// Surface
			OS_WINDOWS_CONTEXT(VK_KHR_WIN32_SURFACE_EXTENSION_NAME)		// Surface(Win32)
		};

		// 利用可能なレイヤーでフィルタ
		Array<const char*> validLayerNames;
		const auto existLayerNames = EnumerateInstanceLayerNames();
		for (const auto& name : layerNames)
		{
			if (existLayerNames.count(name)) {
				validLayerNames.push_back(name);
			}
		}

		// 利用可能な拡張機能でフィルタ
		Array<const char*> validExtensionNames;
		const auto existExtensionNames = EnumerateInstanceExtensionNames(validLayerNames);
		for (const auto& name : extensionNames)
		{
			if (existExtensionNames.count(name)) {
				validExtensionNames.push_back(name);
			}
		}

		// アプリ情報
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.apiVersion = VK_API_VERSION_1_0;
		appInfo.pApplicationName = "OctbitEngine";
		appInfo.pEngineName = "OctbitEngine";

		// インスタンス情報
		VkInstanceCreateInfo instanceInfo{};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pApplicationInfo = &appInfo;
		instanceInfo.enabledLayerCount = (uint32_t)std::size(layerNames);
		instanceInfo.ppEnabledLayerNames = layerNames;
		instanceInfo.enabledExtensionCount = (uint32_t)std::size(extensionNames);
		instanceInfo.ppEnabledExtensionNames = extensionNames;

		// 生成
		Success(::vkCreateInstance(&instanceInfo, nullptr, &m_instance));

	}


	//@―---------------------------------------------------------------------------
	//! @brief  VkPhysicalDevice生成
	//@―---------------------------------------------------------------------------
	void DeviceImpl::createPhysicalDevice() {

		if (m_instance == nullptr)
			return;

		auto devices = EnumerateDevices(m_instance);

		if (devices.empty())LOG_FATAL_EX("RHI", "GPUが接続されていません。");

		// キューファミリーチェック
		for (auto& device : devices) {

			// 拡張機能チェック
			//VkPhysicalDeviceFeatures featuresProperties;
			//::vkGetPhysicalDeviceFeatures(device, &featuresProperties);

			// キューチェック
			uint32_t familyCount = 0;
			::vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);
			Array<VkQueueFamilyProperties> familyPropertyList(familyCount);
			::vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, familyPropertyList.data());

			u32 index = 0;
			for (auto& familyProperty : familyPropertyList) {

				bool ok =
					(familyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
					(familyProperty.queueFlags & VK_QUEUE_COMPUTE_BIT) &&
					(familyProperty.queueFlags & VK_QUEUE_TRANSFER_BIT) &&
					(familyProperty.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT);

				if (ok == false)
					continue;

				m_queueFamilyIndex = index;
				m_queueCount = familyProperty.queueCount;
				index++;
			}

			m_physicalDevice = device;

			// 選択
			break;
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief  VkDevice生成
	//@―---------------------------------------------------------------------------
	void DeviceImpl::createLogicalDevice() {

		if (m_physicalDevice == nullptr)
			return;

		const char* layerNames[] =
		{
			OB_DEBUG_CONTEXT("VK_LAYER_KHRONOS_validation"),
		};

		const char* extensionNames[] =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		};

		// デバイスキューのパラメータ
		Array<float> queuePriorities(m_queueCount, 0.0f);
		::VkDeviceQueueCreateInfo queueInfo{};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = queuePriorities.data();
		queueInfo.queueFamilyIndex = m_queueFamilyIndex;
		queueInfo.queueCount = (uint32_t)queuePriorities.size();

		// 生成情報
		::VkDeviceCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		info.queueCreateInfoCount = 1;
		info.pQueueCreateInfos = &queueInfo;
		info.enabledExtensionCount = (uint32_t)std::size(extensionNames);
		info.ppEnabledExtensionNames = extensionNames;
		info.enabledLayerCount = (uint32_t)std::size(layerNames);
		info.ppEnabledLayerNames = layerNames;
		info.pEnabledFeatures = nullptr;

		Success(::vkCreateDevice(m_physicalDevice, &info, nullptr, &m_logicalDevice));

	}


	//@―---------------------------------------------------------------------------
	//! @brief  VkQueue生成
	//@―---------------------------------------------------------------------------
	void DeviceImpl::createQueue() {

		if (m_logicalDevice == nullptr || m_physicalDevice == nullptr)
			return;

		::vkGetDeviceQueue(m_logicalDevice, m_queueFamilyIndex, 0, &m_queue);

		VkCommandPoolCreateInfo ci{};
		ci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		ci.queueFamilyIndex = m_queueFamilyIndex;
		ci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		vkCreateCommandPool(m_logicalDevice, &ci, nullptr, &m_commandPool);
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
		//OB_NOTIMPLEMENTED();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  レンダーパスを生成
	//@―---------------------------------------------------------------------------
	IRenderPass* DeviceImpl::createRenderPass(const RenderPassDesc& desc) {
		return new RenderPassImpl(m_logicalDevice, desc);
	}

	//@―---------------------------------------------------------------------------
	//! @brief  フレームバッファを生成
	//@―---------------------------------------------------------------------------
	IFrameBuffer* DeviceImpl::createFrameBuffer(const FrameBufferDesc& desc) {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  スワップ・チェーンを生成
	//@―---------------------------------------------------------------------------
	IDisplay* DeviceImpl::createDisplay(const SwapchainDesc& desc) {
		return new DisplayImpl(m_instance, m_physicalDevice, m_logicalDevice, desc);
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
	ob::rhi::ITexture* DeviceImpl::createTexture(const TextureDesc& desc) {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  テクスチャを生成
	//@―---------------------------------------------------------------------------
	ob::rhi::ITexture* DeviceImpl::createTexture(BlobView blob) {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  レンダーテクスチャを生成
	//@―---------------------------------------------------------------------------
	ob::rhi::IRenderTarget* DeviceImpl::createRenderTarget(const RenderTargetDesc& desc) {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  バッファーを生成
	//@―---------------------------------------------------------------------------
	ob::rhi::IBuffer* DeviceImpl::createBuffer(const BufferDesc& desc) {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  頂点シェーダを生成
	//@―---------------------------------------------------------------------------
	ob::rhi::IShader* DeviceImpl::createShader(const String& code, ShaderStage stage) {
		return new ShaderImpl(m_logicalDevice, code, stage);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  頂点シェーダを生成
	//@―---------------------------------------------------------------------------
	ob::rhi::IShader* DeviceImpl::createShader(const Blob& binary, ShaderStage stage) {
		return new ShaderImpl(m_logicalDevice, binary, stage);

	}


	//@―---------------------------------------------------------------------------
	//! @brief  デスクリプタ・テーブルを生成
	//@―---------------------------------------------------------------------------
	ob::rhi::IDescriptorTable* DeviceImpl::createDescriptorTable(DescriptorHeapType type, s32 elementNum) {
		OB_NOTIMPLEMENTED();
		return nullptr;
	}


}// namespace ob::rhi::vulkan