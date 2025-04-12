//***********************************************************
//! @file
//! @brief		デバイス実装(Vulkan)
//! @author		Gajumaru
//***********************************************************
#include <Plugins/VulkanRHI/VulkanRHI.h>
#include <Plugins/VulkanRHI/Utility/Utility.h>
#include <Plugins/VulkanRHI/Buffer/BufferImpl.h>
#include <Plugins/VulkanRHI/Texture/TextureImpl.h>
#include <Plugins/VulkanRHI/Display/DisplayImpl.h>
#include <Plugins/VulkanRHI/Command/CommandListImpl.h>
#include <Plugins/VulkanRHI/Shader/ShaderImpl.h>
#include <Plugins/VulkanRHI/RootSignature/RootSignatureImpl.h>
#include <Plugins/VulkanRHI/PipelineState/PipelineStateImpl.h>
#include <Plugins/VulkanRHI/Buffer/BufferUploader.h>

#include <Framework/Platform/Window.h>

#define SAFE_CREATE(type,type_impl,...)			\
		try {\
			return new type_impl(__VA_ARGS__);\
		} catch (const vk::Error& error) {\
			LOG_ERROR("[VulkanRHI] {}の構築に失敗 {}", #type,error.what());\
			return nullptr;\
		}

namespace ob::rhi::vulkan {

	//@―---------------------------------------------------------------------------
	//! @brief  利用可能なレイヤー名のリストを取得
	//@―---------------------------------------------------------------------------
	static Set<std::string> EnumerateInstanceLayerNames() noexcept
	{
		uint32_t propertyCount;
		::vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);
		Vector<::VkLayerProperties> properties(propertyCount);
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
			Vector<::VkExtensionProperties> properties(propertyCount);
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
	static Vector<VkPhysicalDevice> EnumerateDevices(VkInstance instance) noexcept
	{
		OB_ASSERT_EXPR(instance != nullptr);

		uint32_t physicalDeviceCount = 0;
		::vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
		Vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
		::vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

		return std::move(physicalDevices);
	}

	/*
	//@―---------------------------------------------------------------------------
	//! @brief  レポートフラグからログカテゴリを取得
	//@―---------------------------------------------------------------------------
	static const StringView GetReportBitString(::VkDebugReportFlagsEXT flags)
	{
		if ((flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) != 0)
		{
			return "INFO";
		}
		if ((flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) != 0)
		{
			return "WARNING";
		}
		if ((flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) != 0)
		{
			return "PERFORMANCE_WARNING";
		}
		if ((flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) != 0)
		{
			return "ERROR";
		}
		if ((flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) != 0)
		{
			return "DEBUG";
		}
		return "OTHER";
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

}

namespace ob::rhi::vulkan {

	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	VulkanRHI::VulkanRHI(platform::WindowManager&, GraphicObjectManager& objectManager, ob::rhi::RHIConfig* config, VulkanRHIConfig* vconfig)
		: RHI(objectManager, config)
		, m_config(config ? *config : ob::rhi::RHIConfig{})
		, m_vconfig(vconfig ? *vconfig : ob::rhi::vulkan::VulkanRHIConfig{})
	{
		createInstance();
		createPhysicalDevice();
		createDevice();
		createQueue();
		createUploaders();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	VulkanRHI::~VulkanRHI() {
		finalize();
	}


	//@―---------------------------------------------------------------------------
	//! @brief  妥当な状態か
	//@―---------------------------------------------------------------------------
	bool VulkanRHI::isValid()const {
		return 
			m_instance != nullptr &&
			m_physicalDevice != nullptr &&
			m_device != nullptr &&
			m_queue != nullptr &&
			true;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  VkInstance生成
	//@―---------------------------------------------------------------------------
	void VulkanRHI::createInstance() {

		// レイヤー / 拡張機能名
		Vector<const char*> layerNames;
		Vector<const char*> extensionNames;
		if (m_vconfig.enableDebugLayer) {
			OB_DEBUG_CONTEXT(layerNames.push_back("VK_LAYER_KHRONOS_validation"));
			OB_DEBUG_CONTEXT(extensionNames.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME));
		}
		
		extensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
		OS_WINDOWS_CONTEXT(extensionNames.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME));


		// 利用可能なレイヤーでフィルタ
		Vector<const char*> validLayerNames;
		Vector<const char*> invalidLayerNames;
		const auto existLayerNames = EnumerateInstanceLayerNames();
		for (const auto& name : layerNames)
		{
			if (existLayerNames.count(name)) {
				validLayerNames.push_back(name);
			} else {
				invalidLayerNames.push_back(name);
			}
		}

		// 利用可能な拡張機能でフィルタ
		Vector<const char*> validExtensionNames;
		Vector<const char*> invalidExtensionNames;
		const auto existExtensionNames = EnumerateInstanceExtensionNames(validLayerNames);
		for (const auto& name : extensionNames)
		{
			if (existExtensionNames.count(name)) {
				validExtensionNames.push_back(name);
			} else {
				invalidExtensionNames.push_back(name);
			}
		}

		// アプリ情報
		vk::ApplicationInfo appInfo;
		appInfo.apiVersion = VK_API_VERSION_1_0;
		appInfo.pApplicationName = "OctbitEngine";
		appInfo.pEngineName = "OctbitEngine";

		// インスタンス情報
		vk::InstanceCreateInfo instanceInfo;
		instanceInfo.pApplicationInfo = &appInfo;
		instanceInfo.enabledLayerCount = (uint32_t)validLayerNames.size();
		instanceInfo.ppEnabledLayerNames = validLayerNames.data();
		instanceInfo.enabledExtensionCount = (uint32_t)validExtensionNames.size();
		instanceInfo.ppEnabledExtensionNames = validExtensionNames.data();

#if OB_DEBUG
		{
			// 初期化情報を出力
			String message;
			message += Format("\n[ Vulkan ver.{} ]\n", m_context.enumerateInstanceVersion());
			message += Format("Validation Layers\n");
			for (auto& name : validLayerNames) {
				message += Format("+ {}\n", name);
			}
			for (auto& name : invalidLayerNames) {
				message += Format("- {}\n", name);
			}
			message += Format("Extensions\n");
			for (auto& name : validExtensionNames) {
				message += Format("+ {}\n", name);
			}
			for (auto& name : invalidExtensionNames) {
				message += Format("- {}\n", name);
			}
			message.pop_back();
			LOG_INFO("{}", message);
		}
#endif

		m_instance = m_context.createInstance(instanceInfo, m_allocationCallbacks);

	}


	//@―---------------------------------------------------------------------------
	//! @brief  VkPhysicalDevice生成
	//@―---------------------------------------------------------------------------
	void VulkanRHI::createPhysicalDevice() {

		auto devices = m_instance.enumeratePhysicalDevices();

		if (devices.empty()) {
			throw Exception("GPUが接続されていません。");
		}

		// キューファミリーチェック
		for (auto& device : devices) {

			// 拡張機能チェック
			auto featuresProperties = device.getFeatures();
			auto familyPropertyList = device.getQueueFamilyProperties();

			u32 index = 0;
			for (auto& familyProperty : familyPropertyList) {

				bool ok =
					(familyProperty.queueFlags & vk::QueueFlagBits::eGraphics) &&
					(familyProperty.queueFlags & vk::QueueFlagBits::eCompute) &&
					(familyProperty.queueFlags & vk::QueueFlagBits::eTransfer) &&
					(familyProperty.queueFlags & vk::QueueFlagBits::eSparseBinding);

				if (ok == false)
					continue;

				m_queueFamilyIndex = index;
				m_queueCount = familyProperty.queueCount;
				index++;
			}

			// 選択
			m_physicalDevice = device;

			break;
		}

		if (m_physicalDevice == nullptr) {
			LOG_ERROR("GPUが見つかりません。");
			return;
		}

		m_memoryProperties = m_physicalDevice.getMemoryProperties();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  VkDevice生成
	//@―---------------------------------------------------------------------------
	void VulkanRHI::createDevice() {

		if (m_physicalDevice == nullptr)
			return;

		Vector<const char*> layerNames;
		Vector<const char*> extensionNames;


		OB_DEBUG_CONTEXT(layerNames.push_back("VK_LAYER_KHRONOS_validation"));

		extensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		//OB_DEBUG_CONTEXT(layerNames.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME));

		// TODO

		//// 利用可能なレイヤーでフィルタ
		//Vector<const char*> validLayerNames;
		//const auto existLayerNames = EnumerateInstanceLayerNames();
		//for (const auto& name : layerNames)
		//{
		//	if (existLayerNames.count(name)) {
		//		validLayerNames.push_back(name);
		//	}
		//}
		//
		//// 利用可能な拡張機能でフィルタ
		//Vector<const char*> validExtensionNames;
		//const auto existExtensionNames = EnumerateInstanceExtensionNames(validLayerNames);
		//for (const auto& name : extensionNames)
		//{
		//	if (existExtensionNames.count(name)) {
		//		validExtensionNames.push_back(name);
		//	}
		//}

		// デバイスキューのパラメータ
		Vector<float> queuePriorities(m_queueCount, 0.0f);
		vk::DeviceQueueCreateInfo queueInfo;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = queuePriorities.data();
		queueInfo.queueFamilyIndex = m_queueFamilyIndex;
		queueInfo.queueCount = (uint32_t)queuePriorities.size();

		// 生成情報
		vk::DeviceCreateInfo info;
		info.queueCreateInfoCount = 1;
		info.pQueueCreateInfos = &queueInfo;
		info.enabledExtensionCount = (uint32_t)extensionNames.size();
		info.ppEnabledExtensionNames = extensionNames.data();
		info.enabledLayerCount = (uint32_t)layerNames.size();
		info.ppEnabledLayerNames = layerNames.data();
		info.pEnabledFeatures = nullptr;

		m_device = m_physicalDevice.createDevice(info, m_allocationCallbacks);

	}


	//@―---------------------------------------------------------------------------
	//! @brief  VkQueue生成
	//@―---------------------------------------------------------------------------
	void VulkanRHI::createQueue() {

		if (m_device == nullptr)
			return;

		m_queue = m_device.getQueue(m_queueFamilyIndex, 0);

	}


	//@―---------------------------------------------------------------------------
	//! @brief  VkQueue生成
	//@―---------------------------------------------------------------------------
	void VulkanRHI::createUploaders() {
		m_bufferUploader = std::make_unique<BufferUploader>(*this,16*1024);
		m_textureUploader = std::make_unique<TextureUploader>(*this);
	}


	//@―---------------------------------------------------------------------------
	//! @brief  コマンドをシステムキューに追加
	//@―---------------------------------------------------------------------------
	void VulkanRHI::entryCommandList(const CommandList& commandList) {
		OB_NOTIMPLEMENTED();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  更新
	//@―---------------------------------------------------------------------------
	void VulkanRHI::update() {
		//OB_NOTIMPLEMENTED();
	}



	//! @brief  スワップ・チェーンを生成
	Ref<Display> VulkanRHI::createDisplay(const DisplayDesc& desc) {
		SAFE_CREATE(Display, DisplayImpl,*this, desc);
	}


	//! @brief  コマンドリスト生成
	Ref<CommandList> VulkanRHI::createCommandList(const CommandListDesc& desc) {
		SAFE_CREATE(CommandList, CommandListImpl, *this, desc);
	}



	//! @brief  ルートシグネチャを生成
	Ref<RootSignature> VulkanRHI::createRootSignature(const BindingLayoutDesc& desc) {
		SAFE_CREATE(RootSignature, RootSignatureImpl, *this,desc);
	}


	//! @brief  パイプラインステートを生成
	Ref<PipelineState> VulkanRHI::createPipelineState(const PipelineStateDesc& desc) {
		SAFE_CREATE(PipelineState, PipelineStateImpl, desc);
	}


	//! @brief  テクスチャを生成
	Ref<Texture> VulkanRHI::createTexture(const TextureDesc& desc) { return {}; }


	Ref<Texture> VulkanRHI::createTexture(StringView name, TextureType type, Size size, Span<const IntColor> colors) { 
		SAFE_CREATE(Texture, TextureImpl, *this,name,type,size,colors);
	}


	//! @brief  テクスチャを生成
	Ref<Texture> VulkanRHI::createTexture(StringView name, BlobView blob) { return {}; }


	//! @brief  レンダーテクスチャを生成
	Ref<RenderTexture> VulkanRHI::createRenderTexture(const RenderTextureDesc& desc) { return {}; }


	//! @brief  サンプラーを生成
	Ref<Sampler> VulkanRHI::createSampler(const SamplerDesc& desc) { return {}; }


	//! @brief  バッファーを生成
	Ref<Buffer> VulkanRHI::createBuffer(const BufferDesc& desc) { 
		SAFE_CREATE(Buffer, BufferImpl, *this, desc);
	}


	//! @brief  シェーダをコンパイル
	Ref<Shader> VulkanRHI::compileShader(const ShaderCompileDesc& desc) { return {}; }


	//! @brief  シェーダをロード
	Ref<Shader> VulkanRHI::loadShader(BlobView, ShaderStage) { return {}; }


	//! @brief  デスクリプタ・テーブルを生成
	Ref<DescriptorTable> VulkanRHI::createDescriptorTable(const BindingSlot& desc) { return {}; }
	Ref<DescriptorTable> VulkanRHI::createDescriptorTable(const Ref<RootSignature>& signature, s32 slot) { return {}; }

}