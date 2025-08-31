//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************

#ifdef OS_WINDOWS

#include <Windows.h>
#include <DbgHelp.h>
#include <Framework/Engine/Entry/MainInternal.h>
#include <Framework/Core/Memory/Memory.h>
#include "Framework/Core/Profile/Profile.h"

#include "tracy/TracyC.h"
extern "C"
{
    static HANDLE dbgHelpLock ;
    void DbgHelpInit () { dbgHelpLock = CreateMutex ( nullptr , FALSE , nullptr ) ; }
    void DbgHelpLock () { WaitForSingleObject ( dbgHelpLock , INFINITE ) ; }
    void DbgHelpUnlock () { ReleaseMutex ( dbgHelpLock ) ; }
}

//! @brief  エントリ関数(Windows)
int main(int argc, const char* argv[]) {

    // dbghelpの初期化
    SymInitialize(GetCurrentProcess(), NULL, TRUE);


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