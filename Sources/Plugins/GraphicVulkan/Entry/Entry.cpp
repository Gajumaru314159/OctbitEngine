﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "Entry.h"
#include <Framework/Graphic/Interface/IGraphicModule.h>
#include <Plugins/GraphicVulkan/Device/DeviceImpl.h>

namespace ob::graphic::vulkan {

    class GraphicModule :public ob::graphic::IGraphicModule {
    public:
        GraphicModule() {

        }

        ~GraphicModule() {

        }

        void startup(ob::foundation::Allocator* pAllocator) override {
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
}// namespace ob::graphic::dx12

ob::platform::IModule* GetModule() {
    static ob::graphic::vulkan::GraphicModule module;
    static ob::Logger logger;
    return &module;
}
