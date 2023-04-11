//***********************************************************
//! @file
//! @brief		RHI・モジュール(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RHI.h>
#include <Framework/RHI/Config.h>
#include <Framework/Core/Utility/Pimpl.h>

namespace ob::platform {
	class WindowManager;
}

namespace ob::rhi::dx12 {

	class DeviceImpl;

	class DirectX12RHI : public RHI{
	public:
		DirectX12RHI(ob::rhi::Config*,ob::platform::WindowManager&);
		~DirectX12RHI();

		Device* getDevice()override;
		GraphicObjectManager& getObjectManager() override {
			return *m_objectManager.get();
		}

		//@―---------------------------------------------------------------------------
		//! @brief  ゲーム更新イベント
		//@―---------------------------------------------------------------------------
		void update();

	private:
		ob::rhi::Config m_config;
		Pimpl<DeviceImpl> m_device;// DeviceImpl内にGraphicObjectはないので後から解放
		Pimpl<GraphicObjectManager> m_objectManager;
	};

}