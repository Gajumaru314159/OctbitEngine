//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Framework/Core/Memory/Allocator.h"
#include "MemoryUtility.h"
#include <atomic>
#include <cstddef>
#include <cassert>

namespace ob::core {

    //! @brief カスタムメモリ確保関数
    //! @param size 確保するサイズ
    //! @param alignment アライメント
    //! @return 確保されたメモリのポインタ
    void* Alloc(size_t size, size_t alignment = alignof(std::max_align_t));

    //! @brief カスタムメモリ解放関数
    //! @param ptr 解放するメモリのポインタ
    void Free(void* ptr);

    //! @brief 現在のメモリ使用量を取得
    //! @return メモリ使用量（バイト）
    size_t GetMemoryUsage();


#define OB_MEMORY_SCOPE(category) ob::core::internal::MemoryCategoryScope __memoryScope(category)



    namespace internal {

		//! @brief メモリシステムの初期化
        //! @details メモリのプロファイリングを開始します。
        void InitMemory();

        //! @brief メモリシステムの終了
        void FinalizeMemory();

        //! @brief      メモリカテゴリのスコープを管理するクラス
		//! @details    スレッドごとに現在のメモリカテゴリを保持し、スコープ内で変更を行います。
        class MemoryCategoryScope {
        private:
            const char* m_store;
        public:
            MemoryCategoryScope(const char* category) {
				m_store = GetCurrentCategory();
                std::swap(GetCurrentCategory(),category);
            }
            ~MemoryCategoryScope() {
                std::swap(GetCurrentCategory(), m_store);
            }
            static const char*& GetCurrentCategory() {
                thread_local const char* category = "Unknown";
                return category;
			}
        };

    }

}