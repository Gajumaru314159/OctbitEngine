//***********************************************************
//! @file
//! @brief		メモリーデバイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Base/Fwd.h>
#include <Foundation/Memory/Heap/Heap.h>
#include <Foundation/Template/container/array.h>

namespace ob
{

    //-----------------------------------------------------------------------------
    //! @brief ヒープ生成情報
    //-----------------------------------------------------------------------------
    struct HeapDesc
    {
        HeapClass   classType;  // ヒープのアルゴリズム
        size_t      heapSize;   // ヒープサイズ
    };

    //-----------------------------------------------------------------------------
    //! @brief メモリーデバイス生成情報
    //-----------------------------------------------------------------------------
    struct MemoryDeviceSetings
    {
        HeapDesc virtualMemory;
        HeapDesc cpuChached;
        HeapDesc gpuReadable;
        HeapDesc gpuReadWritable;

#ifdef OB_DEBUG
        HeapDesc debugVirtualMemory;
        HeapDesc debugCPUChached;
        HeapDesc debugGPUReadable;
        HeapDesc debugGPUReadWritable;
#endif
    };

    struct HeapDescList
    {
        Heap* virtualMemory;
    };


    //-----------------------------------------------------------------------------
    //! @brief メモリーデバイス
    //-----------------------------------------------------------------------------
    class MemoryDevice
    {
    public:

        static void Init(const MemoryDeviceSetings& settings);                      // 初期化
        static void Release();                                                      // 解放

        static void SetHeap(HeapType heapType,Heap* pHeap);                         // ヒープの取得
        static void SetDebugHeap(HeapType heapType,Heap* pHeap);                    // ヒープの取得(デバッグ用)

        static Heap& GetHeap(HeapType heapType=HeapType::VirtualMemory);            // ヒープの取得
        static Heap& GetDebugHeap(HeapType heapType = HeapType::VirtualMemory);     // ヒープの取得(デバッグ用)

    };


}// namespcae ob



#if !defined(OB_BUILD)
extern "C"
{
    void OB_API SetHeap(ob::HeapType heapType, ob::Heap* pHeap);
}
#endif
