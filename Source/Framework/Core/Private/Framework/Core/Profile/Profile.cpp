//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Profile/Profile.h>
#include <tracy/TracyC.h>

namespace ob::core {

    void BeginProfile() {
#if TRACY_MANUAL_LIFETIME
        ___tracy_startup_profiler();
#endif
    }

    void EndProfile() {
#if TRACY_MANUAL_LIFETIME
        ___tracy_shutdown_profiler();
#endif
    }

}