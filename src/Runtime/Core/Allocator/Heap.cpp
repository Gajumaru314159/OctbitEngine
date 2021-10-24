//***********************************************************
//! @file
//! @brief		ヒープ基底クラス
//! @author		Gajumaru
//***********************************************************
#include <memory>
#include <Runtime/Foundation/Log/LogMacro.h>

#include "Heap.h"
#include "Platform/PlatformSystemHeap.h"
#include "MimallocHeap.h"
#include "TLSFHeap.h"

namespace ob
{

	//@―---------------------------------------------------------------------------
	//! @brief		メモリの解放
	//! 
	//! @details	この関数はoperator deleteからも呼び出される。
	//!				解放するポインタは Heap から割り当てられたものである必要がある。
	//@―---------------------------------------------------------------------------
	void Heap::Free(void* pBuffer)
	{
		if (pBuffer == nullptr)return;
		auto pHeader = GetOffsetPtr<HeapHeader>(pBuffer, -static_cast<s32>(sizeof(HeapHeader)));

		assert(pHeader->pHeap != nullptr);
#if defined(OB_DEBUG)
		// メモリの破壊を検知しました。
		assert(pHeader->signature == MEMORY_SIGNATURE);
#endif
		pHeader->pHeap->Deallocate(pHeader->pRaw);
	}


	//@―---------------------------------------------------------------------------
	//! @brief	ヒープの作成
	//@―---------------------------------------------------------------------------
	Heap* Heap::Create(size_t size, HeapClass mode, const Char* pName, Heap* pParent)
	{
		Heap* result = nullptr;
		switch (mode)
		{
		case ob::HeapClass::System:
			result = new SystemHeap();
			break;
		case ob::HeapClass::Mimalloc:
			result = new MimallocHeap(nullptr,0);
			break;
		case ob::HeapClass::TLSF:
			LOG_ERROR_EX(TEXT("Memory"), TEXT("not implemented heap mode."));
			break;
		case ob::HeapClass::Invalid:
			LOG_ERROR_EX(TEXT("Memory"), TEXT("invalid heap mode."));
			break;
		default:
			LOG_ERROR_EX(TEXT("Memory"), TEXT("not implemented heap mode."));
			break;
		}

		return result;
	}

}// namespace ob