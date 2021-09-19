﻿//***********************************************************
//! @file
//! @brief		STLアロケータ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <new>
#include <Foundation/Memory/Device/MemoryDevice.h>
namespace ob
{
    //! @cond
    struct allocator_base {
    public:
        // 特殊関数
        // (デフォルトコンストラクタ、コピーコンストラクタ
        //  、ムーブコンストラクタ)
        allocator_base();

        // メモリ確保
        void* allocate(std::size_t n);

        // メモリ解放
        void deallocate(void* pBuffer);
    };
    //! @endcond


    //@―-------------------------------------------------------------------------------
    //! @brief  STLアロケータ
    //@―-------------------------------------------------------------------------------
    template <class T>
    struct allocator:public allocator_base {
    public:
        using value_type = T;   //! アロケート要素の型

    public:

        //@―-------------------------------------------------------------------------------
        // 特殊関数
        // (デフォルトコンストラクタ、コピーコンストラクタ
        //  、ムーブコンストラクタ)
        //@―-------------------------------------------------------------------------------
        allocator() {}

        //@―-------------------------------------------------------------------------------
        // 別な要素型のアロケータを受け取るコンストラクタ
        //@―-------------------------------------------------------------------------------
        template <class U>
        allocator(const allocator<U>&) {}

        //@―-------------------------------------------------------------------------------
        //! @brief メモリ確保
        //@―-------------------------------------------------------------------------------
        T* allocate(std::size_t n)
        {
            return reinterpret_cast<T*>(allocator_base::allocate(sizeof(T)*n));
        }

        //@―-------------------------------------------------------------------------------
        //! @brief メモリ解放
        //@―-------------------------------------------------------------------------------
        void deallocate(T* p, std::size_t n)
        {
            allocator_base::deallocate(p);
        }
    };






    //@―-------------------------------------------------------------------------------
    //! 等価演算子
    //@―-------------------------------------------------------------------------------
    template <class T, class U>
    bool operator==(const allocator<T>&, const allocator<U>&)
    {
        return true;
    }


    //@―-------------------------------------------------------------------------------
    //! 否等価演算子
    //@―-------------------------------------------------------------------------------
    template <class T, class U>
    bool operator!=(const allocator<T>&, const allocator<U>&)
    {
        return false;
    }

}// namespcae ob