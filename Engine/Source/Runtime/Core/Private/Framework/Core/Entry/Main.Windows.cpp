//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/HAL/Platform.h>
#include <Framework/Core/Entry/MainInternal.h>

#ifdef OS_WINDOWS

#include <Windows.h>
#include <DbgHelp.h>

extern "C"
{
    static HANDLE dbgHelpLock ;
    void DbgHelpInit () { dbgHelpLock = CreateMutex ( nullptr , FALSE , nullptr ) ; }
    void DbgHelpLock () { WaitForSingleObject ( dbgHelpLock , INFINITE ) ; }
    void DbgHelpUnlock () { ReleaseMutex ( dbgHelpLock ) ; }
}

//! @brief  エントリ関数(Windows)
int main(int argc, char *argv[]) {

    // dbghelpの初期化
    SymInitialize(GetCurrentProcess(), NULL, TRUE);

    OctbitInternalMain(argc,argv);

    return 0;
}

#endif