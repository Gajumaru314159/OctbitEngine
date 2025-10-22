//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include "Framework/Core/Memory/NewDelete.h"
#include "Framework/Core/Memory/Memory.h"

#pragma warning(push)
#pragma warning(disable : 28251)

// グローバルnew/delete演算子の実装
#if 0
void* operator new(size_t size) {
    return ob::core::Alloc(size, alignof(std::max_align_t));
}

void* operator new[](size_t size) {
    return ob::core::Alloc(size, alignof(std::max_align_t));
}

void* operator new(size_t size, std::align_val_t alignment) {
    return ob::core::Alloc(size, static_cast<size_t>(alignment));
}

void* operator new[](size_t size, std::align_val_t alignment) {
    return ob::core::Alloc(size, static_cast<size_t>(alignment));
}

void operator delete(void* ptr) {
    ob::core::Free(ptr);
}

void operator delete[](void* ptr) {
    ob::core::Free(ptr);
}

void operator delete(void* ptr, [[maybe_unused]]std::align_val_t alignment) {
    ob::core::Free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]]std::align_val_t alignment) {
    ob::core::Free(ptr);
}
#endif

#pragma warning(pop)