﻿//***********************************************************
//! @file
//! @brief		STLアロケータ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <new>
namespace ob::core {

    //! @cond
    struct STLAllocatorBase {
    public:
        // 特殊関数
        // (デフォルトコンストラクタ、コピーコンストラクタ
        //  、ムーブコンストラクタ)
        STLAllocatorBase();

        // メモリ確保
        void* allocate(std::size_t n);

        // メモリ解放
        void deallocate(void* pBuffer);
    };
    //! @endcond


    //@―---------------------------------------------------------------------------
    //! @brief  STLアロケータ
    //@―---------------------------------------------------------------------------
    template <class T>
    struct STLAllocator :public STLAllocatorBase {
    public:

        using value_type = T;   //!< アロケート要素の型

    public:

        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ
        //@―---------------------------------------------------------------------------
        STLAllocator() {}

        //@―---------------------------------------------------------------------------
        //! @brief 別な要素型のアロケータを受け取るコンストラクタ
        //@―---------------------------------------------------------------------------
        template <class U>
        STLAllocator(const STLAllocator<U>&) {}

        //@―---------------------------------------------------------------------------
        //! @brief メモリ確保
        //@―---------------------------------------------------------------------------
        T* allocate(std::size_t n) {
            return reinterpret_cast<T*>(STLAllocatorBase::allocate(sizeof(T) * n));
        }

        //@―---------------------------------------------------------------------------
        //! @brief メモリ解放
        //@―---------------------------------------------------------------------------
        void deallocate(T* p, std::size_t n) {
            STLAllocatorBase::deallocate(p);
        }
    };






    //@―---------------------------------------------------------------------------
    //! 等価演算子
    //@―---------------------------------------------------------------------------
    template <class T, class U>
    bool operator==(const STLAllocator<T>&, const STLAllocator<U>&) {
        return true;
    }


    //@―---------------------------------------------------------------------------
    //! 否等価演算子
    //@―---------------------------------------------------------------------------
    template <class T, class U>
    bool operator!=(const STLAllocator<T>&, const STLAllocator<U>&) {
        return false;
    }

}// namespcae ob