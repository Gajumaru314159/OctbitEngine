//***********************************************************
//! @file
//! @brief		エントリ(Windows)
//! @author		Gajumaru
//***********************************************************

#ifdef OS_WINDOWS

#include <Windows.h>
#include <Framework/Engine/Entry/MainInternal.h>

//@―---------------------------------------------------------------------------
//! @brief  エントリ関数(Windows)
//@―---------------------------------------------------------------------------
int main(int argc, const char* argv[]) {

    WCHAR exePath[MAX_PATH];
    ::GetModuleFileNameW(NULL, exePath, std::size(exePath));

    CommonMain();

    return 0;
}

#endif