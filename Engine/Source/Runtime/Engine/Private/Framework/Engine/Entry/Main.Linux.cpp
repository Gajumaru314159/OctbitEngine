//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************

#ifdef OS_LINUX

#include <Framework/Engine/Entry/MainInternal.h>
#include <Framework/Core/Memory/Memory.h>
#include "Framework/Core/Profile/Profile.h"

#include "tracy/TracyC.h"

//! @brief  エントリ関数(Windows)
int main(int argc, const char* argv[]) {

    ob::core::BeginProfile();
    {
        ob::core::Logger log;

        ob::internal::InitMemory();

        CommonMain();

        ob::internal::FinalizeMemory();
    }

    ob::core::EndProfile();

    return 0;
}

#endif