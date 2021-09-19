//***********************************************************
//! @file
//! @brief		メモリーデバイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Base/Fwd.h>
#include <Foundation/Memory/Heap/Heap.h>
#include <Foundation/Memory/Type/HeapType.h>
#include <Foundation/Template/container/array.h>

namespace ob
{

    //@―---------------------------------------------------------------------------
    //! @brief ヒープ生成情報
    //@―---------------------------------------------------------------------------
    struct HeapDesc
    {
        HeapClass   classType   = HeapClass::System;    //!< ヒープのアルゴリズム
        size_t      heapSize    = 0;                    //!< ヒープサイズ
        Heap*       pHeap       = nullptr;              //!< ヒープのポインタ
    };


    //@―---------------------------------------------------------------------------
    //! @brief メモリーデバイス生成情報
    //@―---------------------------------------------------------------------------
    struct MemoryDeviceSetings
    {
        HeapDesc virtualMemory;             //!< 仮想メモリ
        HeapDesc cpuChached;                //!< CPUキャッチ
        HeapDesc gpuReadable;               //!< GPU読み込み可能
        HeapDesc gpuReadWritable;           //!< GPU読み書き可能
                                                 
#ifdef OB_DEBUG                                  
        HeapDesc debugVirtualMemory;        //!< デバッグ用仮想メモリ
        HeapDesc debugCPUChached;           //!< デバッグ用CPUキャッチ
        HeapDesc debugGPUReadable;          //!< デバッグ用GPU読み込み可能
        HeapDesc debugGPUReadWritable;      //!< デバッグ用GPU読み書き可能
#endif
    };


    //@―---------------------------------------------------------------------------
    //! @brief メモリーデバイス
    //@―---------------------------------------------------------------------------
    class MemoryDevice
    {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief              初期化
        //! 
        //! @details            MemoryDeviceSetings内の各HeapDescにヒープが設定されている場
        //!                     合はそのヒープが設定され、そうでない場合はclassTypeとheapSi
        //!                     zeから新規のヒープが生成される。
        //@―---------------------------------------------------------------------------
        static void Init(const MemoryDeviceSetings& settings);


        //@―---------------------------------------------------------------------------
        //! @brief              解放
        //@―---------------------------------------------------------------------------
        static void Release(); 


        //@―---------------------------------------------------------------------------
        //! @brief              ヒープの設定
        //! 
        //! @details            ヒープが設定されていない場合はシステムヒープが返される。
        //! @param heapType     ヒープ・タイプ
        //! @param pHeap        ヒープ
        //@―---------------------------------------------------------------------------
        static void SetHeap(HeapType heapType,Heap* pHeap);


        //@―---------------------------------------------------------------------------
        //! @brief              デバッグヒープの設定
        //! 
        //! @details            ヒープが設定されていない場合はシステムヒープが返される。
        //! @param heapType     ヒープ・タイプ
        //! @param pHeap        ヒープ
        //@―---------------------------------------------------------------------------
        static void SetDebugHeap(HeapType heapType,Heap* pHeap); 


        //@―---------------------------------------------------------------------------
        //! @brief              メモリ・デバイスの設定を取得
        //@―---------------------------------------------------------------------------
        static MemoryDeviceSetings GetSettings();


        //@―---------------------------------------------------------------------------
        //! @brief              ヒープの取得
        //! 
        //! @details            ヒープが設定されていない場合はシステムヒープが返される。
        //! @param heapType     ヒープ・タイプ
        //@―---------------------------------------------------------------------------
        static Heap& GetHeap(HeapType heapType=HeapType::VirtualMemory);


        //@―---------------------------------------------------------------------------
        //! @brief              デバッグヒープの取得
        //! 
        //! @details            ヒープが設定されていない場合はシステムヒープが返される。
        //! @param heapType     ヒープ・タイプ
        //@―---------------------------------------------------------------------------
        static Heap& GetDebugHeap(HeapType heapType = HeapType::VirtualMemory);     // ヒープの取得(デバッグ用)

    };


}// namespcae ob



#if !defined(OB_BUILD)
extern "C"
{

    //@―---------------------------------------------------------------------------
    //! @brief              ヒープの設定
    //! 
    //! @details            ヒープが設定されていない場合はシステムヒープが返される。
    //! @param heapType     ヒープ・タイプ
    //@―---------------------------------------------------------------------------
    void OB_API SetHeap(ob::HeapType heapType, ob::Heap* pHeap);
}
#endif
