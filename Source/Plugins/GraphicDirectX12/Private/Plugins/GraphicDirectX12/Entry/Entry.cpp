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

        void startup(ob::core::Allocator* pAllocator) override {
            MemorySystem::SetHeapAllocator(HeapUsage::VirtualMemory, pAllocator);
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
        }
    };


    class TestGraphicModule :public ob::graphic::IGraphicModule2 {
    public:
        TestGraphicModule(ob::engine::Engine& engine)
            : ob::graphic::IGraphicModule2(engine)
        {
            LOG_INFO("Module Created.");
        }

    };

}// namespace ob::graphic::dx12

ob::platform::IModule* GetModule() {
    static ob::graphic::dx12::GraphicModule module;
    static ob::Logger logger;
    return &module;
}



void RegisterModule(ob::engine::ModuleManager& manager) {
    manager.add<ob::graphic::dx12::TestGraphicModule, ob::graphic::IGraphicModule2>();
}
