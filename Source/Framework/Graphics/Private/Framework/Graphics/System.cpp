//***********************************************************
//! @file
//! @brief		システム
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/System.h>
#include <Framework/Graphics/GraphicModule.h>
#include <Framework/Graphics/Material/MaterialManager.h>
#include <Framework/RHI/System.h>
#include <Framework/Core/Utility/DI.h>
#include <Framework/Core/Utility/Name.h>

namespace ob::graphics {

    //@―---------------------------------------------------------------------------
    //! @brief      システムをServiceInjectorに登録
    //@―---------------------------------------------------------------------------
    void Register(ServiceInjector& injector) {
        injector.bind<GraphicModule>();
        injector.bind<MaterialManager>();
        rhi::Register(injector);
        Name::Register(injector);
    }

}