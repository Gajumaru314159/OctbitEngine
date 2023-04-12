//***********************************************************
//! @file
//! @brief		システム
//! @author		Gajumaru
//***********************************************************
#include <Framework/Input/System.h>
#include <Framework/Input/InputManager.h>
#include <Framework/Platform/System.h>
#include <Framework/Core/Utility/DI.h>

namespace ob::input {

    //@―---------------------------------------------------------------------------
    //! @brief      システムをServiceInjectorに登録
    //@―---------------------------------------------------------------------------
    void Register(ServiceInjector& injector) {
        injector.bind<InputModule>();
        platform::Register(injector);
    }

}// namespace ob