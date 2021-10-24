//***********************************************************
//! @file
//! @brief		メモリーデバイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Runtime/Foundation/Base/Fwd.h>
#include <Runtime/Foundation/Memory/Allocator/Allocator.h>
#include <Runtime/Foundation/Memory/Type/HeapType.h>

namespace ob {

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
        static void SetHeapAllocator(EHeapType heapType, Allocator* pAllocator);


        //@―---------------------------------------------------------------------------
        //! @brief              デバッグ・ヒープにアロケータを設定
        //! 
        //! @param heapType     設定対象のヒープ・タイプ
        //! @param pAllocator   アロケータ
        //@―---------------------------------------------------------------------------
        static void SetDebugHeapAllocator(EHeapType heapType, Allocator* pAllocator);


        //@―---------------------------------------------------------------------------
        //! @brief              ヒープのアロケータを取得
        //! 
        //! @details            ヒープにアロケータが設定されていない場合は標準のアロケータが返される。
        //! @param heapType     対象のヒープ・タイプ
        //@―---------------------------------------------------------------------------
        static Allocator& GetHeapAllocator(EHeapType heapType = EHeapType::VirtualMemory);


        //@―---------------------------------------------------------------------------
        //! @brief              デバッグ・ヒープのアロケータを取得
        //! 
        //! @details            ヒープにアロケータが設定されていない場合は標準のアロケータが返される。
        //! @param heapType     対象のヒープ・タイプ
        //@―---------------------------------------------------------------------------
        static Allocator& GetDebugHeapAllocator(EHeapType heapType = EHeapType::VirtualMemory);

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
    void OB_API SetHeapAllocator(ob::EHeapType heapType, ob::Allocator* pAllocator);


    //@―---------------------------------------------------------------------------
    //! @brief              デバッグ・ヒープにアロケータを設定
    //! 
    //! @param heapType     設定対象のヒープ・タイプ
    //! @param pAllocator   アロケータ
    //@―---------------------------------------------------------------------------
    void OB_API SetDebugHeapAllocator(ob::EHeapType heapType, ob::Allocator* pAllocator);

}
#endif
