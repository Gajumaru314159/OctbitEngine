//***********************************************************
//! @file
//! @brief		スタックヒープ
//! @author		Gajumaru
//***********************************************************
#include <Core/Memory/Heap/StackHeap.hpp>

namespace ob
{
	
	//-----------------------------------------------------------
	//! @brief デストラクタ
	//-----------------------------------------------------------
	LinerAllocator::~LinerAllocator()
	{
		Destroy();
	}


	//-----------------------------------------------------------
	//! @brief内部アロケート情報のクリア
	//-----------------------------------------------------------
	void LinerAllocator::Clear()
	{
		ScopedCriticalSection sc(m_criticalSection);
		m_index = 0;
	}


	//-----------------------------------------------------------
	//! @brief アロケータの破棄
	//-----------------------------------------------------------
	void LinerAllocator::Destroy()
	{
		if (m_heap == nullptr)return;
		m_parentHeap->Deallocate(m_heap);
	}


	//-----------------------------------------------------------
	//! @brief			ヒープの作成
	//! 
	//! @details		作成済みのヒープに対してヒープを作成しようとした場合は失敗する。
	//! @param			ヒープの確保量
	//! @param	alloc	ヒープの作成に使用するアロケータ
	//! @retval TRUE	成功
	//! @retval FALSE	成功
	//-----------------------------------------------------------
	b32 LinerAllocator::Create(Size capacity, Heap& alloc)
	{
		if (m_heap != nullptr)return FALSE;
		m_heap = reinterpret_cast<Byte*>(alloc.Allocate(capacity));
		m_parentHeap = &alloc;
		m_heapCapacity = capacity;
		m_index = 0;
		return TRUE;
	}


	//-----------------------------------------------------------
	//! @brief			内部メモリ確保
	//! 
	//! @param size		確保するメモリ容量
	//! @return			割り当てられたメモリのポインタ
	//-----------------------------------------------------------
	void* LinerAllocator::AllocateImpl(Size size)
	{
		if (m_heapCapacity <= m_index + size)
		{
			OB_ERROR(TC("ヒープのサイズをオーバーしました"));
			return nullptr;
		}

		ScopedCriticalSection sc(m_criticalSection);

		return &m_heap[m_index += size];
	}


	//-----------------------------------------------------------
	//! @brief			内部メモリ開放
	//! 
	//! @param ptr		解放するポインタ
	//-----------------------------------------------------------
	void LinerAllocator::DeallocateImpl(void* ptr)
	{
		//OB_ERROR(TC("LinerAllocatorは解放をサポートしません。clear関数を使用してください。"));
		return;
	}

}// namespace ob