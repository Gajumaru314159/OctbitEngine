//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "MemoryDevice.h"

#include <Foundation/Base/Fwd.h>
#include <Foundation/Template/utility.h>
#include <Foundation/Template/container/array.h>
#include <Foundation/Log/LogMacro.h>

#include "../Heap/Platform/PlatformSystemHeap.h"
#include "../Heap/TLSFHeap.h"

namespace ob
{
    //! @cond
    static array<Heap*, enum_cast(HeapType::Max)> s_heaps;          //!< ヒープリスト
    static array<Heap*, enum_cast(HeapType::Max)> s_debugHeaps;     //!< デバッグ・ヒープリスト
    //! @endcond
    

    //-----------------------------------------------------------------------------
    //! @brief              初期化
    //-----------------------------------------------------------------------------
    void MemoryDevice::Init(const MemoryDeviceSetings& settings)
    {
        bool isInitialized = false;
        for (auto& pHeap : s_heaps) {
            if (pHeap == nullptr)continue;
            LOG_FATAL_EX(TEXT("Memory"), TEXT("初期化済みのヒープ {} を上書きしようとしました。"),pHeap->GetName());
        }
        for (auto& pHeap : s_debugHeaps)if (pHeap != nullptr)isInitialized = true;

        if (isInitialized) {
            LOG_FATAL_EX(TEXT("Memory"), TEXT("初期化済みのヒープを上書きしようとしました。"));
        }

        s_heaps[enum_cast(HeapType::VirtualMemory)  ] = Heap::Create(settings.virtualMemory.heapSize    , settings.virtualMemory.classType  , TEXT("GlobalVirtualMemory")     , nullptr);
        s_heaps[enum_cast(HeapType::CPUCached)      ] = Heap::Create(settings.cpuChached.heapSize       , settings.cpuChached.classType     , TEXT("GlobalCPUCached")         , nullptr);
        s_heaps[enum_cast(HeapType::GPUReadable)    ] = Heap::Create(settings.gpuReadable.heapSize      , settings.gpuReadable.classType    , TEXT("GlobalGPUReadable")       , nullptr);
        s_heaps[enum_cast(HeapType::GPUReadWritable)] = Heap::Create(settings.gpuReadWritable.heapSize  , settings.gpuReadWritable.classType, TEXT("GlobalGPUReadWritable")   , nullptr);
        
        s_debugHeaps[enum_cast(HeapType::VirtualMemory)  ] = Heap::Create(settings.debugVirtualMemory.heapSize    , settings.debugVirtualMemory.classType  , TEXT("GlobalDebugVirtualMemory")     , nullptr);
        s_debugHeaps[enum_cast(HeapType::CPUCached)      ] = Heap::Create(settings.debugCPUChached.heapSize       , settings.debugCPUChached.classType     , TEXT("GlobalDebugCPUCached")         , nullptr);
        s_debugHeaps[enum_cast(HeapType::GPUReadable)    ] = Heap::Create(settings.debugGPUReadable.heapSize      , settings.debugGPUReadable.classType    , TEXT("GlobalDebugGPUReadable")       , nullptr);
        s_debugHeaps[enum_cast(HeapType::GPUReadWritable)] = Heap::Create(settings.debugGPUReadWritable.heapSize  , settings.debugGPUReadWritable.classType, TEXT("GlobalDebugGPUReadWritable")   , nullptr);
    }


    //-----------------------------------------------------------------------------
    //! @brief              解放
    //-----------------------------------------------------------------------------
    void MemoryDevice::Release()
    {
        for (s32 i = 0; i < enum_cast(HeapType::Max); ++i)
        {
            if (s_heaps[i] != nullptr)
                s_heaps[i]->Release();
            if (s_debugHeaps[i] != nullptr)
                s_heaps[i]->Release();            
        }
    }


    //-----------------------------------------------------------------------------
    //! @brief              ヒープの設定
    //! 
    //! @details            ヒープが設定されていない場合はシステムヒープが返される。
    //! @param heapType     ヒープ・タイプ
    //! @param pHeap        ヒープ
    //-----------------------------------------------------------------------------
    void MemoryDevice::SetHeap(HeapType heapType, Heap* pHeap)
    {
        const s32 index = static_cast<s32>(heapType);
        if (s_heaps[index] != nullptr)
        {
            LOG_WARNING_EX(TEXT("Memory"), TEXT("初期化済みのヒープを差し替えました。"));
        }
        s_heaps[index]=pHeap;
    }


    //-----------------------------------------------------------------------------
    //! @brief              デバッグヒープの設定
    //! 
    //! @details            ヒープが設定されていない場合はシステムヒープが返される。
    //! @param heapType     ヒープ・タイプ
    //! @param pHeap        ヒープ
    //-----------------------------------------------------------------------------
    void MemoryDevice::SetDebugHeap(HeapType heapType, Heap* pHeap)
    {
        const s32 index = static_cast<s32>(heapType);
        if (s_debugHeaps[index] != nullptr)
        {
            LOG_WARNING_EX(TEXT("Memory"), TEXT("初期化済みのデバッグヒープを差し替えました。"));
        }
        s_debugHeaps[index] = pHeap;
    }


    //-----------------------------------------------------------------------------
    //! @brief              ヒープの取得
    //! 
    //! @details            ヒープが設定されていない場合はシステムヒープが返される。
    //! @param heapType     ヒープ・タイプ
    //-----------------------------------------------------------------------------
    Heap& MemoryDevice::GetHeap(HeapType heapType)
    {
        const s32 index = static_cast<s32>(heapType);
        auto pHeap = s_heaps[index];

        if (pHeap == nullptr) {
            static SystemHeap systemHeap;
            return systemHeap;
        }

        return *pHeap;
    }


    //-----------------------------------------------------------------------------
    //! @brief              デバッグヒープの取得
    //! 
    //! @details            ヒープが設定されていない場合はシステムヒープが返される。
    //! @param heapType     ヒープ・タイプ
    //-----------------------------------------------------------------------------
    Heap& MemoryDevice::GetDebugHeap(HeapType heapType)
    {
        const s32 index = static_cast<s32>(heapType);
        auto pHeap = s_debugHeaps[index];

        if (pHeap == nullptr) {
            static SystemHeap systemHeap;
            return systemHeap;
        }

        return *pHeap;
    }

}// namespace ob


#if !defined(OB_BUILD)
extern "C"
{

    //-----------------------------------------------------------------------------
    //! @brief              ヒープの設定(外部用)
    //! 
    //! @details            サブモジュールにメインプロセスのヒープを設定する場合に使用する。
    //! @param heapType     ヒープ・タイプ
    //! @retval true        成功
    //! @retval false       失敗
    //-----------------------------------------------------------------------------
    void OB_API SetHeap(ob::HeapType heapType, ob::Heap* pHeap)
    {
        ob::MemoryDevice::SetHeap(heapType, pHeap);
    }
}
#endif