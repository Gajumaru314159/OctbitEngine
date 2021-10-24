//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "MemorySystem.h"

#include <Runtime/Foundation/Memory/Allocator/MallocAllocator.h>
#include <Runtime/Foundation/Base/Fwd.h>
#include <Runtime/Foundation/Template/utility.h>
#include <Runtime/Foundation/Template/container/array.h>
#include <Runtime/Foundation/Log/LogMacro.h>

namespace ob {
    //! @cond
    //static bool s_isInitialized = false;
    static array<Allocator*, enum_cast(EHeapType::Max)> s_heaps;          //!< ヒープリスト
    static array<Allocator*, enum_cast(EHeapType::Max)> s_debugHeaps;     //!< デバッグ・ヒープリスト
    //! @endcond


    //@―---------------------------------------------------------------------------
    //! @brief              初期化
    //! 
    //! @details            MemorySystemSetings内の各HeapDescにヒープが設定されている場
    //!                     合はそのヒープが設定され、そうでない場合はclassTypeとheapSi
    //!                     zeから新規のヒープが生成される。
    //@―---------------------------------------------------------------------------
    void MemorySystem::Init() {
        // ゼロ初期化で必要なさそう
        
        //if (s_isInitialized)return;
        //for (auto& pHeap : s_heaps) {
        //    pHeap = nullptr;
        //}
        //for (auto& pHeap : s_debugHeaps) {
        //    pHeap = nullptr;
        //}
        //s_isInitialized = true;
    }


    //@―---------------------------------------------------------------------------
    //! @brief              解放
    //@―---------------------------------------------------------------------------
    void MemorySystem::Release() {
        for (s32 i = 0; i < enum_cast(EHeapType::Max); ++i) {
            if (s_heaps[i] != nullptr)
                s_heaps[i]->Release();
            if (s_debugHeaps[i] != nullptr)
                s_heaps[i]->Release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief              ヒープにアロケータを設定
    //! 
    //! @param heapType     設定対象のヒープ・タイプ
    //! @param pAllocator   アロケータ
    //@―---------------------------------------------------------------------------
    void MemorySystem::SetHeapAllocator(EHeapType heapType, Allocator* pAllocator) {
        const s32 index = static_cast<s32>(heapType);
        assert(0 <= index && index < enum_cast(EHeapType::Max));
        if (s_heaps[index] != nullptr) {
            LOG_WARNING_EX(TEXT("Memory"), TEXT("初期化済みのヒープを差し替えました。"));
        }
        s_heaps[index] = pAllocator;
    }


    //@―---------------------------------------------------------------------------
    //! @brief              デバッグ・ヒープにアロケータを設定
    //! 
    //! @param heapType     設定対象のヒープ・タイプ
    //! @param pAllocator   アロケータ
    //@―---------------------------------------------------------------------------
    void MemorySystem::SetDebugHeapAllocator(EHeapType heapType, Allocator* pAllocator) {
        const s32 index = enum_cast(heapType);
        assert(0 <= index && index < enum_cast(EHeapType::Max));
        if (s_debugHeaps[index] != nullptr) {
            LOG_WARNING_EX(TEXT("Memory"), TEXT("初期化済みのデバッグヒープを差し替えました。"));
        }
        s_debugHeaps[index] = pAllocator;
    }


    //@―---------------------------------------------------------------------------
    //! @brief              ヒープのアロケータを取得
    //! 
    //! @details            ヒープにアロケータが設定されていない場合は標準のアロケータが返される。
    //! @param heapType     対象のヒープ・タイプ
    //@―---------------------------------------------------------------------------
    Allocator& MemorySystem::GetHeapAllocator(EHeapType heapType) {
        const s32 index = enum_cast(heapType);
        assert(0 <= index && index < enum_cast(EHeapType::Max));
        auto pHeap = s_heaps[index];

        if (pHeap == nullptr) {
            static MallocAllocator systemHeap(TEXT("Default Allocator"));
            return systemHeap;
        }

        return *pHeap;
    }


    //@―---------------------------------------------------------------------------
    //! @brief              デバッグ・ヒープのアロケータを取得
    //! 
    //! @details            ヒープにアロケータが設定されていない場合は標準のアロケータが返される。
    //! @param heapType     対象のヒープ・タイプ
    //@―---------------------------------------------------------------------------
    Allocator& MemorySystem::GetDebugHeapAllocator(EHeapType heapType) {
        const s32 index = enum_cast(heapType);
        assert(0 <= index && index < enum_cast(EHeapType::Max));
        auto pHeap = s_debugHeaps[index];

        if (pHeap == nullptr) {
            static MallocAllocator systemHeap(TEXT("Default Debug Allocator"));
            return systemHeap;
        }

        return *pHeap;
    }

}// namespace ob


#if !defined(OB_BUILD)
extern "C"
{

    //@―---------------------------------------------------------------------------
    //! @brief              ヒープにアロケータを設定
    //! 
    //! @param heapType     設定対象のヒープ・タイプ
    //! @param pAllocator   アロケータ
    //@―---------------------------------------------------------------------------
    void OB_API SetHeapAllocator(ob::EHeapType heapType, ob::Allocator* pAllocator) {
        ob::MemorySystem::SetHeapAllocator(heapType, pAllocator);
    }


    //@―---------------------------------------------------------------------------
    //! @brief              デバッグ・ヒープにアロケータを設定
    //! 
    //! @param heapType     設定対象のヒープ・タイプ
    //! @param pAllocator   アロケータ
    //@―---------------------------------------------------------------------------
    void OB_API SetDebugHeapAllocator(ob::EHeapType heapType, ob::Allocator* pAllocator) {
        ob::MemorySystem::SetDebugHeapAllocator(heapType, pAllocator);
    }

}
#endif