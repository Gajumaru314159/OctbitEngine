//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Memory/System/MemorySystem.h>

#include <Framework/Core/Memory/Allocator/MallocAllocator.h>
#include <Framework/Core/Template/include.h>

namespace ob::core {
    //! @cond
    static StaticArray<Allocator*, enum_cast(HeapUsage::Max)> s_heaps;          //!< ヒープリスト
    static StaticArray<Allocator*, enum_cast(HeapUsage::Max)> s_debugHeaps;     //!< デバッグ・ヒープリスト
    //! @endcond


    //@―---------------------------------------------------------------------------
    //! @brief              初期化
    //! 
    //! @details            MemorySystemSetings内の各HeapDescにヒープが設定されている場
    //!                     合はそのヒープが設定され、そうでない場合はclassTypeとheapSi
    //!                     zeから新規のヒープが生成される。
    //@―---------------------------------------------------------------------------
    void MemorySystem::Init() {
    }


    //@―---------------------------------------------------------------------------
    //! @brief              解放
    //@―---------------------------------------------------------------------------
    void MemorySystem::Release() {
        for (s32 i = 0; i < enum_cast(HeapUsage::Max); ++i) {
            if (s_heaps[i] != nullptr)
                s_heaps[i]->release();
            if (s_debugHeaps[i] != nullptr)
                s_heaps[i]->release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief              ヒープにアロケータを設定
    //! 
    //! @param HeapUsage     設定対象のヒープ・タイプ
    //! @param pAllocator   アロケータ
    //@―---------------------------------------------------------------------------
    void MemorySystem::SetHeapAllocator(HeapUsage HeapUsage, Allocator* pAllocator) {
        const s32 index = static_cast<s32>(HeapUsage);
        OB_ASSERT_RANGE(index,0,enum_cast(HeapUsage::Max));
        if (s_heaps[index] != nullptr) {
            //LOG_WARNING_EX("Memory", "初期化済みのヒープを差し替えました。");
            assert(false);
        }
        s_heaps[index] = pAllocator;
    }


    //@―---------------------------------------------------------------------------
    //! @brief              デバッグ・ヒープにアロケータを設定
    //! 
    //! @param HeapUsage     設定対象のヒープ・タイプ
    //! @param pAllocator   アロケータ
    //@―---------------------------------------------------------------------------
    void MemorySystem::SetDebugHeapAllocator(HeapUsage HeapUsage, Allocator* pAllocator) {
        const s32 index = enum_cast(HeapUsage);
        OB_ASSERT_RANGE(index, 0, enum_cast(HeapUsage::Max));
        OB_CHECK_ASSERT(s_debugHeaps[index]==nullptr,"デバッグヒープが割り当て済みです。");
        if (s_debugHeaps[index] != nullptr) {
            //LOG_WARNING_EX("Memory", "初期化済みのデバッグヒープを差し替えました。");
            assert(false);
        }
        s_debugHeaps[index] = pAllocator;
    }


    //@―---------------------------------------------------------------------------
    //! @brief              ヒープのアロケータを取得
    //! 
    //! @details            ヒープにアロケータが設定されていない場合は標準のアロケータが返される。
    //! @param HeapUsage     対象のヒープ・タイプ
    //@―---------------------------------------------------------------------------
    Allocator& MemorySystem::GetHeapAllocator(HeapUsage HeapUsage) {
        const s32 index = enum_cast(HeapUsage);
        OB_ASSERT_RANGE(index, 0, enum_cast(HeapUsage::Max));
        auto pHeap = s_heaps[index];

        if (pHeap == nullptr) {
            static MallocAllocator systemHeap(TC("Default Allocator"));
            return systemHeap;
        }

        return *pHeap;
    }


    //@―---------------------------------------------------------------------------
    //! @brief              デバッグ・ヒープのアロケータを取得
    //! 
    //! @details            ヒープにアロケータが設定されていない場合は標準のアロケータが返される。
    //! @param HeapUsage     対象のヒープ・タイプ
    //@―---------------------------------------------------------------------------
    Allocator& MemorySystem::GetDebugHeapAllocator(HeapUsage HeapUsage) {
        const s32 index = enum_cast(HeapUsage);
        OB_ASSERT_RANGE(index, 0, enum_cast(HeapUsage::Max));
        auto pHeap = s_debugHeaps[index];

        if (pHeap == nullptr) {
            static MallocAllocator systemHeap(TC("Default Debug Allocator"));
            return systemHeap;
        }

        return *pHeap;
    }

}// namespace ob::core::core


#if !defined(OB_BUILD)
extern "C"
{

    //@―---------------------------------------------------------------------------
    //! @brief              ヒープにアロケータを設定
    //! 
    //! @param HeapUsage     設定対象のヒープ・タイプ
    //! @param pAllocator   アロケータ
    //@―---------------------------------------------------------------------------
    void OB_API SetHeapAllocator(ob::core::HeapUsage HeapUsage, ob::core::Allocator* pAllocator) {
        ob::core::MemorySystem::SetHeapAllocator(HeapUsage, pAllocator);
    }


    //@―---------------------------------------------------------------------------
    //! @brief              デバッグ・ヒープにアロケータを設定
    //! 
    //! @param HeapUsage     設定対象のヒープ・タイプ
    //! @param pAllocator   アロケータ
    //@―---------------------------------------------------------------------------
    void OB_API SetDebugHeapAllocator(ob::core::HeapUsage HeapUsage, ob::core::Allocator* pAllocator) {
        ob::core::MemorySystem::SetDebugHeapAllocator(HeapUsage, pAllocator);
    }

}
#endif