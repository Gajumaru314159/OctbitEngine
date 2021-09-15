//***********************************************************
//! @file
//! @brief		OSヒープ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Core/Base/Fwd.hpp>

#include <cstdint>
#include <Core/Memory/Heap/Heap.hpp>

namespace ob
{

    //-----------------------------------------------------------
    //! @brief		OSヒープ
    //! 
    //! @details	システムのnewから動的に割り当てるアロケータ。
    //-----------------------------------------------------------
    class OB_API OSHeap final :public Heap
    {
    public:

        void	Clear()override;                            // 内部アロケート情報のクリア
        void	Destroy()override;                          // アロケータの破棄
        Size	GetHeapSize()const override;                // アロケータのヒープサイズを取得
        Size	GetFreeHeapSize()const override;            // アロケータの使用可能なヒープサイズを取得
        b32		IsValid()const override;                    // 利用可能な状態かどうか

    protected:

        void* AllocateImpl(Size size)override;              // 内部メモリ確保
        void	DeallocateImpl(void* ptr)override;          // 内部メモリ開放

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //-----------------------------------------------------------
    // @brief 内部アロケート情報のクリア
    //-----------------------------------------------------------
    inline void OSHeap::Clear()
    {

    }

    //-----------------------------------------------------------
    // @brief アロケータの破棄
    //-----------------------------------------------------------
    inline void OSHeap::Destroy()
    {

    }

    //-----------------------------------------------------------
    // @brief アロケータのヒープサイズを取得
    //-----------------------------------------------------------
    inline Size OSHeap::GetHeapSize()const
    {
        return (Size)-1;
    };


    //-----------------------------------------------------------
    // @brief アロケータの使用可能なヒープサイズを取得
    //-----------------------------------------------------------
    inline Size OSHeap::GetFreeHeapSize()const
    {
        return (Size)-1;
    };


    //-----------------------------------------------------------
    //! @brief 利用可能な状態かどうか
    //! 
    //! @retval TRUE  利用可能
    //! @retval FALSE 利用不可
    //-----------------------------------------------------------
    inline b32 OSHeap::IsValid()const
    {
        return TRUE;
    };

}// namespace ob