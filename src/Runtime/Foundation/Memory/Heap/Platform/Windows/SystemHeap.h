//***********************************************************
//! @file
//! @brief		システムヒープ(Windows)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Memory/Heap/Heap.h>

#include <Windows.h>


namespace ob
{

    //-----------------------------------------------------------------------------
    //! @brief		OSヒープ
    //! 
    //! @details	システムのnewから動的に割り当てるアロケータ。
    //-----------------------------------------------------------------------------
    class SystemHeap final:public Heap
    {
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================
        SystemHeap();
        virtual ~SystemHeap() override;
        virtual void	Release() override;                     // 内部アロケート情報のクリア

        //===============================================================
        //  メモリ確保 /解放
        //===============================================================
        virtual void* Allocate(size_t size, u32 alignment, bool zeroClear) override;    // メモリの確保
        virtual void Deallocate(void* pBuffer) override;                                // メモリの開放


        //===============================================================
        //  取得
        //===============================================================
        virtual Char*	GetName()		    const override;    // ヒープ名の取得
        virtual size_t	GetHeapSize()		const override;    // アロケータのヒープサイズを取得
        virtual size_t	GetFreeHeapSize()	const override;    // アロケータの使用可能なヒープサイズを取得
        virtual bool	IsValid()           const override;    // 利用可能な状態かどうか

    private:

        HANDLE m_handle;
        Char* m_pName;

    };






    //===============================================================
    // インライン関数
    //===============================================================

}// namespace ob