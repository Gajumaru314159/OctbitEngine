//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************

#ifdef OS_WINDOWS

#include <Windows.h>
#include <Framework/Engine/Entry/MainInternal.h>
#include <Framework/Core/Memory/Memory.h>

//! @brief  エントリ関数(Windows)
int main(int argc, const char* argv[]) {

    ob::core::Logger log;

    ob::internal::InitMemory();

    CommonMain();

    ob::internal::FinalizeMemory();

    return 0;
}

#endif