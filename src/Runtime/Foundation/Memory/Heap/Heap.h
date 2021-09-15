//***********************************************************
//! @file
//! @brief		ヒープ基底クラス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Base/Fwd.h>
#include <Foundation/Memory/Utility/MemoryUtility.h>
#include <Foundation/Kernel/CriticalSection/CriticalSection.h>

#include "../Type/HeapType.h"

namespace ob
{

    //! @brief システムのデフォルトアライメント
#define DEFAULT_ALIGNMENT __STDCPP_DEFAULT_NEW_ALIGNMENT__


    //-----------------------------------------------------------------------------
    //! @brief      メモリ・ヒープ・インターフェース
    //! 
    //! @details    このインターフェースを経由してメモリの割り当てを行う。
    //!             割り当てアルゴリズムは継承クラスを参照してください。
    //-----------------------------------------------------------------------------
    class Heap
    {
    protected:

        enum
        {
            MEMORY_SIGNATURE = 0xDEADC0DE,		//!< 署名(メモリ破壊チェック用)
        };


        //-----------------------------------------------------------------------------
        //! @brief      ヒープヘッダ
        //! 
        //! @details    Heapから割り当てられたメモリは直前に HeapHeader を持つ必要がある。
        //-----------------------------------------------------------------------------
        struct HeapHeader     {
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
        virtual ~Heap() {};

        virtual void	Release() = 0;                                      // 内部アロケート情報のクリア

        //===============================================================
        //  メモリ確保 /解放
        //===============================================================
        virtual void* Allocate(size_t size, u32 alignment = DEFAULT_ALIGNMENT, bool zeroClear = false) = 0; // メモリの確保
        virtual void Deallocate(void* pBuffer)=0;                                                           // メモリの開放


        //===============================================================
        //  取得
        //===============================================================
        virtual Char*	GetName()		    const = 0;      // ヒープ名の取得
        virtual size_t	GetHeapSize()		const = 0;      // アロケータのヒープサイズを取得
        virtual size_t	GetFreeHeapSize()	const = 0;      // アロケータの使用可能なヒープサイズを取得
        virtual bool	IsValid()           const = 0;      // 利用可能な状態かどうか


    public:

        static void Free(void* pBuffer);                                                                // メモリの開放
        static Heap* Create(size_t size, HeapClass mode, const Char* pName,Heap* pParent=nullptr);      // ヒープの作成

    };


}// namespace ob