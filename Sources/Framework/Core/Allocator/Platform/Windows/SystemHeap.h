﻿//***********************************************************
//! @file
//! @brief		システムヒープ(Windows)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Allocator/Heap.h>
#include <Windows.h>


namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief		OSヒープ
    //! 
    //! @details	システムのnewから動的に割り当てるアロケータ。
    //@―---------------------------------------------------------------------------
    class SystemHeap final :public Heap {
        friend class Heap;
        friend class MemorySystem;
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief                  デストラクタ
        //@―---------------------------------------------------------------------------
        ~SystemHeap() override;


        //@―---------------------------------------------------------------------------
        //! @brief                  内部アロケート情報のクリア
        //@―---------------------------------------------------------------------------
        void release() override;


        //===============================================================
        //  メモリ確保 /解放
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief	                メモリの確保
        //! 
        //! @param[in] size         確保サイズ
        //! @param[in] alignment    アライメント
        //! @param[in] zeroClear    確保したバッファをゼロで初期化するか
        //@―---------------------------------------------------------------------------
        void* allocate(size_t size, u32 alignment, bool zeroClear) override;


        //@―---------------------------------------------------------------------------
        //! @brief	                メモリの開放
        //@―---------------------------------------------------------------------------
        void deallocate(void* pBuffer) override;


        //===============================================================
        //  取得
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief	                ヒープ名の取得
        //@―---------------------------------------------------------------------------
        Char* getName() const override;


        //@―---------------------------------------------------------------------------
        //! @brief	                アロケータのヒープサイズを取得
        //@―---------------------------------------------------------------------------
        size_t getHeapSize() const override;


        //@―---------------------------------------------------------------------------
        //! @brief	                アロケータの使用可能なヒープサイズを取得
        //@―---------------------------------------------------------------------------
        size_t getFreeHeapSize() const override;


        //@―---------------------------------------------------------------------------
        //! @brief	                利用可能な状態かどうか
        //@―---------------------------------------------------------------------------
        bool isValid()const override;


    protected:

        //@―---------------------------------------------------------------------------
        //! @brief	                コンストラクタ
        //@―---------------------------------------------------------------------------
        SystemHeap();

    private:

        HANDLE m_handle;
        Char* m_pName;

    };






    //===============================================================
    // インライン関数
    //===============================================================

}// namespace ob