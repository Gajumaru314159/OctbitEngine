//
// pch.cpp
// Include the standard header and generate the precompiled header.
//

#include "OctbitCore-test.PCH.h"

//void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
//{
//    return malloc(size);
//}
//void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
//{
//    // this allocator doesn't support alignment
//    EASTL_ASSERT(alignment <= 8);
//    return malloc(size);
//}
//
//int Vsnprintf8(char8_t* pDestination, size_t n, const char8_t* pFormat, va_list arguments)
//{
//#ifdef _MSC_VER
//    return vsnprintf(pDestination, n, pFormat, arguments);
//#else
//    return vsnprintf(pDestination, n, pFormat, arguments);
//#endif
//}