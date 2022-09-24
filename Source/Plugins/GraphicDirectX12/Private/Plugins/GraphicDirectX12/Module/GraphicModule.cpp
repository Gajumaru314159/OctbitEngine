//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/Interface/IGraphicModule.h>
#include <Framework/Engine/ModuleFactory.h>
#include <Plugins/GraphicDirectX12/Device/DeviceImpl.h>

namespace ob::graphic::dx12
{

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class DirectXRHIModule :public GraphicModule {
    public:

        DirectXRHIModule(ob::engine::Engine& engine)
            : GraphicModule(engine)
        {

        }

        IDevice* createDevice(FeatureLevel featureLevel) override
        {
            return new DeviceImpl(featureLevel);
        }

    };



}// namespace ob

REGISTER_MODULE_DERIVE(ob::graphic::dx12::DirectXRHIModule, ob::graphic::GraphicModule);