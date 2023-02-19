//***********************************************************
//! @file
//! @brief		RHI・モジュール(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RHIModule.h>
#include <Framework/Engine/ModuleFactory.h>
#include <Plugins/DirectX12RHI/Device/DeviceImpl.h>

namespace ob::rhi::dx12 {

	class DirectX12RHIModule :public ob::rhi::RHIModule {
	public:
		DirectX12RHIModule()
			: m_objectManager(2)
			, m_device()
		{

		}

		~DirectX12RHIModule() {
			m_device.releaseSystemResource();
		}

		Device* getDevice()override {
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

REGISTER_MODULE_DERIVE(ob::rhi::dx12::DirectX12RHIModule, ob::rhi::RHIModule);

void Link_DirectX12() {

}