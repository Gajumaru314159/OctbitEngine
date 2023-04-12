//***********************************************************
//! @file
//! @brief		システム
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/System.h>
#include <Framework/Graphics/GraphicModule.h>
#include <Framework/RHI/System.h>
#include <Framework/Core/Utility/DI.h>
#include <Framework/Core/Utility/Name.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      システムをServiceInjectorに登録
    //@―---------------------------------------------------------------------------
    void Register(ServiceInjector& injector) {
        injector.bind<ob::graphics::GraphicModule>();
        rhi::Register(injector);
        Name::Register(injector);
    }

}