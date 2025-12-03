//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include "Framework/Core/Memory/NewDelete.h"
#include "Framework/Core/Memory/Memory.h"

#pragma warning(push)
#pragma warning(disable : 28251)

void operator delete  (void* p, [[maybe_unused]]std::align_val_t al) noexcept {
    ob::core::Free(p);
}
void operator delete[](void* p, [[maybe_unused]]std::align_val_t al) noexcept {
    ob::core::Free(p);
}
void operator delete  (void* p, [[maybe_unused]]std::size_t n, [[maybe_unused]]std::align_val_t al) noexcept {
    ob::core::Free(p);
}
void operator delete[](void* p, [[maybe_unused]]std::size_t n, [[maybe_unused]]std::align_val_t al) noexcept {
    ob::core::Free(p);
}
void operator delete  (void* p, [[maybe_unused]]std::align_val_t al, [[maybe_unused]]const std::nothrow_t&) noexcept {
    ob::core::Free(p);
}
void operator delete[](void* p, [[maybe_unused]]std::align_val_t al, [[maybe_unused]]const std::nothrow_t&) noexcept {
    ob::core::Free(p);
}

void* operator new( std::size_t n, std::align_val_t al)   noexcept(false) {
    return ob::core::Alloc(n, static_cast<size_t>(al));
}
void* operator new[]( std::size_t n, std::align_val_t al) noexcept(false) {
    return ob::core::Alloc(n, static_cast<size_t>(al));
}
void* operator new  (std::size_t n, std::align_val_t al, const std::nothrow_t&) noexcept {
    return ob::core::Alloc(n, static_cast<size_t>(al));
}
void* operator new[](std::size_t n, std::align_val_t al, const std::nothrow_t&) noexcept {
    return ob::core::Alloc(n, static_cast<size_t>(al));
}

#pragma warning(pop)