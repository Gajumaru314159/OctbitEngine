//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include "Framework/Core/Memory/Memory.h"
#include "MallocAllocator.h"
#include "MimallocAllocator.h"
#include <Framework/Core/Log/Assertion.h>

namespace ob::core {

	constexpr size_t MEMORY_SIGNATURE = 0xDEAD000DEAD000;

    //! @brief メモリブロックヘッダー
    //! アロケートした領域の前に配置されるヘッダー情報
    struct MemoryBlockHeader {
        void* signature = (void*)MEMORY_SIGNATURE;
        size_t size;           //!< 確保されたサイズ
        size_t headerOffset;   //!< ヘッダーからユーザー領域までのオフセット
    };

    // デフォルトアロケーター
    static MimallocAllocator s_defaultAllocator;
    static Allocator* s_allocator = nullptr;

    // メモリ使用量のカウンター
    static std::atomic<size_t> s_memoryUsage = 0;

    void SetAllocator(Allocator& allocator) {
        // デフォルトアロケーターの場合のみ切り替え可能
        assert(s_allocator == &s_defaultAllocator);
        s_allocator = &allocator;
    }

    Allocator* GetAllocator() {
        return s_allocator;
    }

    void* Alloc(size_t size, size_t alignment) {

        if (s_allocator == nullptr) {
            return s_defaultAllocator.alloc(size,alignment);
        }

        // ヘッダーサイズとアライメントを計算
        constexpr size_t headerAlignment = alignof(MemoryBlockHeader);
        size_t headerSize = sizeof(MemoryBlockHeader);
        
        // ユーザー領域の直前にオフセット情報を格納するスペースを追加
        size_t offsetSize = sizeof(size_t);
        
        // ヘッダー + オフセット情報後のユーザー領域が指定されたアライメントになるように調整
        size_t preUserSize = headerSize + offsetSize;
        size_t alignedPreUserSize = (preUserSize + alignment - 1) & ~(alignment - 1);
        
        // 全体のアライメントはヘッダーとユーザー指定の最大値
        size_t totalAlignment = (headerAlignment > alignment) ? headerAlignment : alignment;
        
        size_t requiredSize = alignedPreUserSize + size;
        
        // アロケーターからメモリ確保
        void* ptr = s_allocator->alloc(requiredSize, totalAlignment);
        if (!ptr) {
            return nullptr;
        }

        // ヘッダー設定
        MemoryBlockHeader* header = static_cast<MemoryBlockHeader*>(ptr);
		header->signature = (void*)MEMORY_SIGNATURE;
        header->size = size;
        header->headerOffset = alignedPreUserSize;

        // ユーザー領域の直前にオフセット情報を格納
        size_t* offsetPtr = GetOffsetPtr<size_t>(ptr, alignedPreUserSize - sizeof(size_t));
        *offsetPtr = alignedPreUserSize;

        // メモリ使用量更新
        s_memoryUsage += size;

        // ユーザー領域のポインタを返す（適切にアライメントされている）
        return GetOffsetPtr(header, alignedPreUserSize);
    }

    void Free(void* ptr) {
        if (!ptr) {
            return;
        }

        if (!s_allocator) {
            s_defaultAllocator.free(ptr);
            return;
        }

        // ユーザー領域の直前にヘッダーオフセットが格納されている
        size_t* offsetPtr = GetOffsetPtr<size_t>(ptr, -static_cast<ptrdiff_t>(sizeof(size_t)));
        size_t headerOffset = *offsetPtr;
        
        // 実際のヘッダーを正しいオフセットで取得
        MemoryBlockHeader* header = GetOffsetPtr<MemoryBlockHeader>(ptr, -static_cast<ptrdiff_t>(headerOffset));

        // メモリー初期化前のアロケーションであれば通常解放
        if((size_t)header->signature != MEMORY_SIGNATURE) {
			s_defaultAllocator.free(ptr);
            return;
		}
        
        // メモリ使用量更新
        s_memoryUsage -= header->size;
        
        // アロケーターで解放
        s_allocator->free(header);
    }

    size_t GetMemoryUsage() {
        return s_memoryUsage;
    }

    namespace internal {

        void InitMemory() {
		    s_allocator = &s_defaultAllocator;
        }

        void FinalizeMemory() {
            if (0 < s_memoryUsage) {
		        LOG_ERROR("メモリリークがあります。リーク量: {} バイト", s_memoryUsage.load());
            }
            s_allocator = nullptr;
        }

    }

}