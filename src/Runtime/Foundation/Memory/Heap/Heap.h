﻿//***********************************************************
//! @file
//! @brief		ヒープ基底クラス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Base/Fwd.h>
#include <Foundation/Memory/Utility/MemoryUtility.h>
#include <Foundation/Kernel/CriticalSection/CriticalSection.h>

#include "../Type/HeapType.h"

namespace ob {

    //! @brief システムのデフォルトアライメント
#define DEFAULT_ALIGNMENT __STDCPP_DEFAULT_NEW_ALIGNMENT__


    //! @brief      メモリ・ヒープ・インターフェース
    //! 
    //! @details    このインターフェースを経由してメモリの割り当てを行う。
    //!             割り当てアルゴリズムは継承クラスを参照してください。
    class Heap {
    protected:

        //!< 署名(メモリ破壊チェック用)
        enum {
            MEMORY_SIGNATURE = 0xDEADC0DE,
        };


        //! @brief      ヒープヘッダ
        //! 
        //! @details    Heapから割り当てられたメモリは直前に HeapHeader を持つ必要がある。
        struct HeapHeader {
            Heap* pHeap;            //!< 確保元のヒープ
            void* pRaw;             //!< 実際に確保されたポインタ

#if defined(OB_DEBUG)
            s32 timeStamp;
            u32 signature = MEMORY_SIGNATURE;
#endif
        };

    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================

        //! @brief                  デストラクタ
        virtual ~Heap() {};

        //===============================================================
        //  解放
        //===============================================================

        //! @brief                  内部アロケート情報のクリア
        virtual void	Release() = 0;


        //===============================================================
        //  メモリ確保 /解放
        //===============================================================

        //! @brief	                メモリの確保
        //! 
        //! @param[in] size         確保サイズ
        //! @param[in] alignment    アライメント
        //! @param[in] zeroClear    確保したバッファをゼロで初期化するか
        virtual void* Allocate(size_t size, u32 alignment = DEFAULT_ALIGNMENT, bool zeroClear = false) = 0;


        //! @brief	                メモリの開放
        virtual void Deallocate(void* pBuffer) = 0;


        //===============================================================
        //  取得
        //===============================================================

        //! @brief	                ヒープ名の取得
        virtual Char* GetName()const = 0;


        //! @brief	                アロケータのヒープサイズを取得
        virtual size_t GetHeapSize()const = 0;


        //! @brief	                アロケータの使用可能なヒープサイズを取得
        virtual size_t GetFreeHeapSize()const = 0;


        //! @brief	                利用可能な状態かどうか
        virtual bool IsValid()const = 0;


    public:

        //! @brief                  メモリの開放
        //! 
        //! @param pBuffer          解放先ポインタ
        static void Free(void* pBuffer);      
        
        
        //! @brief                              ヒープの作成
        //! 
        //! @details                            親ヒープを指定した場合、ヒープの内部メモリを親ヒープから確保する。
        //! @exception BadAllocationException   ヒープの内部メモリの確保に失敗 
        //! @param size                         ヒープ・サイズ             
        //! @param mode                         ヒープ・モード
        //! @param pName                        ヒープ名
        //! @param pParent                      親ヒープ
        //! @return                 
        static Heap* Create(size_t size, HeapClass mode, const Char* pName, Heap* pParent = nullptr);

    };


}// namespace ob