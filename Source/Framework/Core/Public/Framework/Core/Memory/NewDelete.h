//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <memory>

void operator delete  (void* p, std::align_val_t al) noexcept;
void operator delete[](void* p, std::align_val_t al) noexcept;
void operator delete  (void* p, std::size_t n, std::align_val_t al) noexcept;
void operator delete[](void* p, std::size_t n, std::align_val_t al) noexcept;
void operator delete  (void* p, std::align_val_t al, const std::nothrow_t&) noexcept;
void operator delete[](void* p, std::align_val_t al, const std::nothrow_t&) noexcept;

void* operator new( std::size_t n, std::align_val_t al)   noexcept(false);
void* operator new[]( std::size_t n, std::align_val_t al) noexcept(false);
void* operator new  (std::size_t n, std::align_val_t al, const std::nothrow_t&) noexcept;
void* operator new[](std::size_t n, std::align_val_t al, const std::nothrow_t&) noexcept;
