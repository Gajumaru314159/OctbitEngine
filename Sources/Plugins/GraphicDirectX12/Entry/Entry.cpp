//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "Entry.h"
#include <Framework/Graphic/Interface/IGraphicModule.h>
#include <Plugins/GraphicDirectX12/Device/DeviceImpl.h>

namespace ob::graphic::dx12 {

    class GraphicModule :public ob::graphic::IGraphicModule {
    public:
        GraphicModule() {

        }

        ~GraphicModule() {

        }

        void startup() override {
            // PIX開始
        }
        void shutdown() override {
            // PIX終了
        }

        bool isSupported() override {
            return true;
        };

        IDevice* createDevice(FeatureLevel featureLevel)override {
            return new DeviceImpl(featureLevel);

            //D3D12CreateDevice();
        }
    };
}// namespace ob::graphic::dx12

ob::platform::IModule* GetModule() {
    static ob::graphic::dx12::GraphicModule test;
    return &test;
}
