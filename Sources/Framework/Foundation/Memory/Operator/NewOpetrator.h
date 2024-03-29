﻿//***********************************************************
//! @file
//! @brief		new / delete オーバーライド
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <memory>

namespace ob
{
    class Allocator;
}// namespcae ob

#pragma warning(push)
#pragma warning(disable : 28251)
_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new(size_t size);                                                  //!< ヒープからメモリを確保してインスタンスを作成
_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __CRTDECL operator new[](size_t size);                                                //!< ヒープからメモリを確保してインスタンス配列を作成
void* operator new(size_t size,std::align_val_t alignment);                                 //!< ヒープからメモリを確保してインスタンスを作成
void* operator new[](size_t size, std::align_val_t alignment);                              //!< ヒープからメモリを確保してインスタンス配列を作成

void* operator new(size_t size, ob::Allocator& allocator)noexcept;                          //!< ヒープからメモリを確保してインスタンスを作成
void* operator new[](size_t size, ob::Allocator& allocator)noexcept;                        //!< ヒープからメモリを確保してインスタンス配列を作成
void* operator new(size_t size, size_t alignment, ob::Allocator& allocator)noexcept;        //!< ヒープからメモリを確保してインスタンスを作成
void* operator new[](size_t size, size_t alignment, ob::Allocator& allocator)noexcept;      //!< ヒープからメモリを確保してインスタンス配列を作成


void operator delete(void* ptr);                                                            //!< 指定したアドレスのインスタンスの開放
void operator delete[](void* ptr);                                                          //!< 指定したアドレスのインスタンス配列の開放
void operator delete(void* ptr, std::align_val_t alignment);                                //!< 指定したアドレスのインスタンスの開放(new失敗用)
void operator delete[](void* ptr, std::align_val_t alignment);                              //!< 指定したアドレスのインスタンス配列の開放(new失敗用)

void operator delete(void* ptr, ob::Allocator& allocator)noexcept;                          //!< 指定したアドレスのインスタンスの開放(new失敗用)
void operator delete[](void* ptr, ob::Allocator& allocator)noexcept;                        //!< 指定したアドレスのインスタンス配列の開放(new失敗用)
void operator delete(void* ptr, size_t alignment, ob::Allocator& allocator)noexcept;        //!< 指定したアドレスのインスタンスの開放(new失敗用)
void operator delete[](void* ptr, size_t alignment, ob::Allocator& allocator)noexcept;      //!< 指定したアドレスのインスタンス配列の開放(new失敗用)
#pragma warning(pop)