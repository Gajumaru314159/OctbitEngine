﻿//***********************************************************
//! @file
//! @brief		Mimallocヒープ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Allocator/Heap.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  Mimallocヒープ
    //! 
    //! @details https://github.com/microsoft/mimalloc
    //@―---------------------------------------------------------------------------
    class MimallocHeap final :public Heap {
        friend class Heap;
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief                  デストラクタ
        //@―---------------------------------------------------------------------------
        ~MimallocHeap() override;


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
        //! @brief	                ヒープ名を取得
        //@―---------------------------------------------------------------------------
        Char* name() const override;


        //@―---------------------------------------------------------------------------
        //! @brief	                アロケータのヒープサイズを取得
        //@―---------------------------------------------------------------------------
        size_t heapSize() const override;


        //@―---------------------------------------------------------------------------
        //! @brief	                アロケータの使用可能なヒープサイズを取得
        //@―---------------------------------------------------------------------------
        size_t freeHeapSize() const override;


        //@―---------------------------------------------------------------------------
        //! @brief	                利用可能な状態かどうか
        //@―---------------------------------------------------------------------------
        bool isValid()const override;

    protected:

        //@―---------------------------------------------------------------------------
        //! @brief	                コンストラクタ
        //@―---------------------------------------------------------------------------
        MimallocHeap(void* pBuffer, size_t size);       // コンストラクタ

    private:

        mutex m_mutex;

        Char* m_pName;


    };






    //===============================================================
    // インライン関数
    //===============================================================

    //@―---------------------------------------------------------------------------


}// namespcae ob