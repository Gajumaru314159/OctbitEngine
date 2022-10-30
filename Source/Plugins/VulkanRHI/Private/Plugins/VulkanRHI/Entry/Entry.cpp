//***********************************************************
//! @file
//! @brief		グラフィック・モジュール(Vulkan)
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/IGraphicModule.h>
#include <Framework/Engine/ModuleFactory.h>
#include <Plugins/VulkanRHI/Device/DeviceImpl.h>

namespace ob::rhi::vulkan {

	class VulkanRHIModule :public ob::rhi::GraphicModule {
	public:
		VulkanRHIModule()
			: m_objectManager(2)
			, m_device(FeatureLevel::Default)
		{

		}

		IDevice* getDevice()override {
			return &m_device;
		}

		GraphicObjectManager& getObjectManager() override {
			return m_objectManager;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  ゲーム更新イベント
		//@―---------------------------------------------------------------------------
		void update()override {
			m_device.update();
			m_objectManager.update();
		}

	private:
		DeviceImpl m_device;// DeviceImpl内にGraphicObjectはないので後から解放
		GraphicObjectManager m_objectManager;
	};

}// namespace ob::rhi::dx12

REGISTER_MODULE_DERIVE(ob::rhi::vulkan::VulkanRHIModule, ob::rhi::GraphicModule);

void Link_Vulkan() {

}