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

    CommonMain();

    return 0;
}

#endif