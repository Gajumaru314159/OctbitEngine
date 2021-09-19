//***********************************************************
//! @file
//! @brief		Mimallocヒープ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Memory/Heap/Heap.h>
#include <Foundation/Template/thread/mutex.h>


namespace ob
{

    //@―-------------------------------------------------------------------------------
    //! @brief  Mimallocヒープ
    //! 
    //! @ref    https://github.com/microsoft/mimalloc
    //@―-------------------------------------------------------------------------------
    class MimallocHeap final:public Heap
    {
        friend class Heap;
    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================
        ~MimallocHeap()override;                        // デストラクタ
        void	Release() override;                     // 内部アロケート情報のクリア


        //===============================================================
        //  メモリ確保 /解放
        //===============================================================
        void* Allocate(size_t size, u32 alignment, bool zeroClear) override;    // メモリの確保
        void Deallocate(void* pBuffer) override;                                // メモリの開放


        //===============================================================
        //  取得
        //===============================================================
        Char* GetName()		        const override;     // ヒープ名の取得
        size_t	GetHeapSize()		const override;     // アロケータのヒープサイズを取得
        size_t	GetFreeHeapSize()	const override;     // アロケータの使用可能なヒープサイズを取得
        bool	IsValid()           const override;     // 利用可能な状態かどうか

    protected:

        MimallocHeap(void* pBuffer, size_t size);       // コンストラクタ

    private:

        mutex m_mutex;

        Char* m_pName;


    };






    //===============================================================
    // インライン関数
    //===============================================================

    //@―-------------------------------------------------------------------------------


}// namespcae ob