//***********************************************************
//! @file
//! @brief		スタックヒープ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Core/Base/Fwd.hpp>

#include <Core/Memory/Heap/Heap.hpp>

namespace ob
{
	//-----------------------------------------------------------
	//! @brief		スタックヒープ
	//! @details	スタックベースのアロケータ。メモリ解放は個別に行えないが高速に動作する。
	//-----------------------------------------------------------
	class OB_API LinerAllocator final :public Heap
	{
	public:

		LinerAllocator();									// コンストラクタ
		~LinerAllocator();									// デストラクタ


		b32		Create(Size capacity, Heap& alloc);			// ヒープの作成
		void	Clear()override;							// 内部アロケート情報のクリア
		void	Destroy()override;							// アロケータの破棄

		Size	GetHeapSize()const override;				// アロケータのヒープサイズを取得
		Size	GetFreeHeapSize()const override;			// アロケータの使用可能なヒープサイズを取得
		b32		IsValid()const override;					// 利用可能な状態かどうか

	protected:

		void*	AllocateImpl(Size size)override;			// 内部メモリ確保
		void	DeallocateImpl(void* ptr)override;			// 内部メモリ開放

	private:

		Byte* m_heap;			//!< ヒープ領域
		Size  m_heapCapacity;	//!< ヒープサイズ
		Size  m_index;			//!< ヒーインデックス

	};






	//===============================================================
	// インライン関数
	//===============================================================

	//-----------------------------------------------------------
	//! @brief デフォルトコンストラクタ
	//-----------------------------------------------------------
	inline LinerAllocator::LinerAllocator() :
		m_heap(nullptr),
		m_heapCapacity(0),
		m_index(0)
	{}


	//-----------------------------------------------------------
	//! @brief		利用可能な状態かどうか
	//! 
	//! @retval		TRUE 利用可能
	//! @retval		FALSE 利用不可
	//-----------------------------------------------------------
	inline b32 LinerAllocator::IsValid()const
	{
		return m_heap!=nullptr;
	}


	//-----------------------------------------------------------
	//! @brief		アロケータのヒープサイズを取得
	//-----------------------------------------------------------
	inline Size LinerAllocator::GetHeapSize()const
	{
		return m_heapCapacity;
	}


	//-----------------------------------------------------------
	//! @brief		アロケータの使用可能なヒープサイズを取得
	//-----------------------------------------------------------
	inline Size LinerAllocator::GetFreeHeapSize()const
	{
		return m_heapCapacity-m_index;
	}

}// namespace ob