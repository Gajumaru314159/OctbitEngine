//***********************************************************
//! @file
//! @brief		システム
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/System.h>
#include <Framework/Graphics/Graphics.h>
#include <Framework/Graphics/Material/MaterialManager.h>
#include <Framework/RHI/System.h>
#include <Framework/Core/Utility/DI.h>
#include <Framework/Core/Utility/Name.h>

namespace ob::graphics {

    //@―---------------------------------------------------------------------------
    //! @brief      システムをServiceInjectorに登録
    //@―---------------------------------------------------------------------------
    void Register(ServiceInjector& injector) {
        injector.bind<MaterialManager>();
        injector.bind<Graphics>();
        rhi::Register(injector);
        Name::Register(injector);
    }

}