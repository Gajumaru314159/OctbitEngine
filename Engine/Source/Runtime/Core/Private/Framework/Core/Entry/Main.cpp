//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Profile/Profile.h>
#include <Framework/Core/Entry/Main.h>
#include <Framework/Core/Memory/Memory.h>

int OctbitInternalMain(int argc, char *argv[])
{
    ob::internal::InitMemory();

    int result = OctbitMain(argc,argv);

    ob::internal::FinalizeMemory();

    return result;
}