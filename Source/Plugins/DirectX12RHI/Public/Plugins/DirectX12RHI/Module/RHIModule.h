//***********************************************************
//! @file
//! @brief		RHI・モジュール(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RHIModule.h>
#include <Framework/Core/Utility/Pimpl.h>

namespace ob::platform {
	class WindowManager;
}

namespace ob::rhi::dx12 {

	class DeviceImpl;

	class DirectX12RHIModule : public RHIModule{
	public:
		DirectX12RHIModule(ob::platform::WindowManager&);
		~DirectX12RHIModule();

		Device* getDevice()override;
		GraphicObjectManager& getObjectManager() override {
			return *m_objectManager.get();
		}

		//@―---------------------------------------------------------------------------
		//! @brief  ゲーム更新イベント
		//@―---------------------------------------------------------------------------
		void update();

	private:
		Pimpl<DeviceImpl> m_device;// DeviceImpl内にGraphicObjectはないので後から解放
		Pimpl<GraphicObjectManager> m_objectManager;
	};

}