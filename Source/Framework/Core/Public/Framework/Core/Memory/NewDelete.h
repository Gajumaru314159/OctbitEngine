//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <memory>
#if 0
#pragma warning(push)
#pragma warning(disable : 28251)
void* operator new(size_t size);                                                  //!< ヒープからメモリを確保してインスタンスを作成
void* operator new[](size_t size);                                                //!< ヒープからメモリを確保してインスタンス配列を作成
void* operator new(size_t size, std::align_val_t alignment);                                 //!< ヒープからメモリを確保してインスタンスを作成
void* operator new[](size_t size, std::align_val_t alignment);                              //!< ヒープからメモリを確保してインスタンス配列を作成

void operator delete(void* ptr);                                                            //!< 指定したアドレスのインスタンスの開放
void operator delete[](void* ptr);                                                          //!< 指定したアドレスのインスタンス配列の開放
void operator delete(void* ptr, std::align_val_t alignment);                                //!< 指定したアドレスのインスタンスの開放(new失敗用)
void operator delete[](void* ptr, std::align_val_t alignment);                              //!< 指定したアドレスのインスタンス配列の開放(new失敗用)

#pragma warning(pop)
#endif