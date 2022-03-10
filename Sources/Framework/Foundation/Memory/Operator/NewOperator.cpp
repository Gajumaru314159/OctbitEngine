//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Foundation/Memory/System/MemorySystem.h>

//@―---------------------------------------------------------------------------
//! @brief	ヒープからメモリを確保してインスタンスを作成
//@―---------------------------------------------------------------------------
_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* operator new(size_t size)
{
    return ob::MemorySystem::GetHeapAllocator().allocate(size);
}

//@―---------------------------------------------------------------------------
//! @brief	ヒープからメモリを確保してインスタンス配列を作成
//@―---------------------------------------------------------------------------
_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* operator new[](size_t size)
{
    return ob::MemorySystem::GetHeapAllocator().allocate(size);
}

//@―---------------------------------------------------------------------------
//! @brief	ヒープからメモリを確保してインスタンスを作成
//@―---------------------------------------------------------------------------
void* operator new(size_t size, std::align_val_t alignment)
{
    return ob::MemorySystem::GetHeapAllocator().allocate(size, static_cast<ob::u32>(alignment));
}

//@―---------------------------------------------------------------------------
//! @brief	ヒープからメモリを確保してインスタンス配列を作成
//@―---------------------------------------------------------------------------
void* operator new[](size_t size, std::align_val_t alignment)
{
    return ob::MemorySystem::GetHeapAllocator().allocate(size, static_cast<ob::u32>(alignment));
}

//@―---------------------------------------------------------------------------
//! @brief	ヒープからメモリを確保してインスタンスを作成
//@―---------------------------------------------------------------------------
void* operator new(size_t size, ob::Allocator& Allocator)noexcept
{
    return Allocator.allocate(size);
}

//@―---------------------------------------------------------------------------
//! @brief	ヒープからメモリを確保してインスタンス配列を作成
//@―---------------------------------------------------------------------------
void* operator new[](size_t size, ob::Allocator& Allocator)noexcept
{
    return Allocator.allocate(size);
}

//@―---------------------------------------------------------------------------
//! @brief	ヒープからメモリを確保してインスタンスを作成
//@―---------------------------------------------------------------------------
void* operator new(size_t size, size_t alignment, ob::Allocator& Allocator)noexcept
{
    return Allocator.allocate(size, static_cast<ob::u32>(alignment));
}

//@―---------------------------------------------------------------------------
//! @brief	ヒープからメモリを確保してインスタンス配列を作成
//@―---------------------------------------------------------------------------
void* operator new[](size_t size, size_t alignment, ob::Allocator& Allocator)noexcept
{
    return Allocator.allocate(size, static_cast<ob::u32>(alignment));
}


//@―---------------------------------------------------------------------------
//! @brief	指定したインスタンスの開放
//@―---------------------------------------------------------------------------
void operator delete(void* ptr)noexcept
{
    ob::Allocator::Free(ptr);
}

//@―---------------------------------------------------------------------------
//! @brief	指定したインスタンス配列の開放
//@―---------------------------------------------------------------------------
void operator delete[](void* ptr)noexcept
{
    ob::Allocator::Free(ptr);
}

//@―---------------------------------------------------------------------------
//! @brief	指定したインスタンスの開放
//@―---------------------------------------------------------------------------
void operator delete(void* ptr, std::align_val_t alignment)noexcept
{
    ob::Allocator::Free(ptr);
}

//@―---------------------------------------------------------------------------
//! @brief	指定したインスタンス配列の開放
//@―---------------------------------------------------------------------------
void operator delete[](void* ptr, std::align_val_t alignment)noexcept
{
    ob::Allocator::Free(ptr);
}

//@―---------------------------------------------------------------------------
//! @brief	指定したインスタンスの開放
//@―---------------------------------------------------------------------------
void operator delete(void* ptr, ob::Allocator& Allocator)noexcept
{
    ob::Allocator::Free(ptr);
}

//@―---------------------------------------------------------------------------
//! @brief	指定したインスタンス配列の開放
//@―---------------------------------------------------------------------------
void operator delete[](void* ptr, ob::Allocator& Allocator)noexcept
{
    ob::Allocator::Free(ptr);
}

//@―---------------------------------------------------------------------------
//! @brief	指定したインスタンスの開放
//@―---------------------------------------------------------------------------
void operator delete(void* ptr, size_t alignment, ob::Allocator& Allocator)noexcept
{
    ob::Allocator::Free(ptr);
}

//@―---------------------------------------------------------------------------
//! @brief	指定したインスタンス配列の開放
//@―---------------------------------------------------------------------------
void operator delete[](void* ptr, size_t alignment, ob::Allocator& Allocator)noexcept
{
    ob::Allocator::Free(ptr);
}