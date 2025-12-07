//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/HAL/Platform.h>
#include <Framework/Core/Entry/MainInternal.h>

#ifdef OS_LINUX

//! @brief  エントリ関数(Windows)
int main(int argc, char *argv[]) {
    OctbitInternalMain(argc,argv);
    return 0;
}

#endif