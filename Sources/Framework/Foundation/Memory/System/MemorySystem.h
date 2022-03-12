﻿//***********************************************************
//! @file
//! @brief		メモリーデバイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Foundation/Memory/Allocator/Allocator.h>
#include <Framework/Foundation/Memory/Types/HeapUsage.h>

namespace ob::foundation {

    //@―---------------------------------------------------------------------------
    //! @brief メモリーデバイス
    //@―---------------------------------------------------------------------------
    class MemorySystem {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief              初期化
        //! 
        //! @details            MemoryDeviceSetings内の各HeapDescにヒープのアロケータが設定されている場
        //!                     合はそのヒープのアロケータが設定され、そうでない場合はclassTypeとheapSi
        //!                     zeから新規のヒープのアロケータが生成される。
        //@―---------------------------------------------------------------------------
        static void Init();


        //@―---------------------------------------------------------------------------
        //! @brief              解放
        //@―---------------------------------------------------------------------------
        static void Release();


        //@―---------------------------------------------------------------------------
        //! @brief              ヒープにアロケータを設定
        //! 
        //! @param heapType     設定対象のヒープ・タイプ
        //! @param pAllocator   アロケータ
        //@―---------------------------------------------------------------------------
        static void SetHeapAllocator(HeapUsage heapType, Allocator* pAllocator);


        //@―---------------------------------------------------------------------------
        //! @brief              デバッグ・ヒープにアロケータを設定
        //! 
        //! @param heapType     設定対象のヒープ・タイプ
        //! @param pAllocator   アロケータ
        //@―---------------------------------------------------------------------------
        static void SetDebugHeapAllocator(HeapUsage heapType, Allocator* pAllocator);


        //@―---------------------------------------------------------------------------
        //! @brief              ヒープのアロケータを取得
        //! 
        //! @details            ヒープにアロケータが設定されていない場合は標準のアロケータが返される。
        //! @param heapType     対象のヒープ・タイプ
        //@―---------------------------------------------------------------------------
        static Allocator& GetHeapAllocator(HeapUsage heapType = HeapUsage::VirtualMemory);


        //@―---------------------------------------------------------------------------
        //! @brief              デバッグ・ヒープのアロケータを取得
        //! 
        //! @details            ヒープにアロケータが設定されていない場合は標準のアロケータが返される。
        //! @param heapType     対象のヒープ・タイプ
        //@―---------------------------------------------------------------------------
        static Allocator& GetDebugHeapAllocator(HeapUsage heapType = HeapUsage::VirtualMemory);

    };


}// namespcae ob



#if !defined(OB_BUILD)
extern "C"
{

    //@―---------------------------------------------------------------------------
    //! @brief              ヒープにアロケータを設定
    //! 
    //! @param heapType     設定対象のヒープ・タイプ
    //! @param pAllocator   アロケータ
    //@―---------------------------------------------------------------------------
    void OB_API SetHeapAllocator(ob::foundation::HeapUsage heapType, ob::foundation::Allocator* pAllocator);


    //@―---------------------------------------------------------------------------
    //! @brief              デバッグ・ヒープにアロケータを設定
    //! 
    //! @param heapType     設定対象のヒープ・タイプ
    //! @param pAllocator   アロケータ
    //@―---------------------------------------------------------------------------
    void OB_API SetDebugHeapAllocator(ob::foundation::HeapUsage heapType, ob::foundation::Allocator* pAllocator);

}
#endif
