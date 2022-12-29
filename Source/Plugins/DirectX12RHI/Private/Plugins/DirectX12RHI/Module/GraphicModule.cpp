//***********************************************************
//! @file
//! @brief		グラフィック・モジュール(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/IGraphicModule.h>
#include <Framework/Engine/ModuleFactory.h>
#include <Plugins/DirectX12RHI/Device/DeviceImpl.h>

namespace ob::rhi::dx12 {

	class DirectX12GraphicModule :public ob::rhi::GraphicModule {
	public:
		DirectX12GraphicModule()
			: m_objectManager(2)
			, m_device(FeatureLevel::Default)
		{

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

REGISTER_MODULE_DERIVE(ob::rhi::dx12::DirectX12GraphicModule, ob::rhi::GraphicModule);

void Link_DirectX12() {

}