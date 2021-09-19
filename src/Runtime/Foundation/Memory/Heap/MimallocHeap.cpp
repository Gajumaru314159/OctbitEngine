//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "MimallocHeap.h"
#include <Foundation/Log/LogMacro.h>
#include <Foundation/Assertion/Assertion.h>

namespace ob
{

	//@―-------------------------------------------------------------------------------
	MimallocHeap::MimallocHeap(void* pBuffer, size_t size)
	{
		m_pName = TEXT("MimallocHeap");
		OB_REQUIRE(pBuffer != nullptr);
	}

	MimallocHeap::~MimallocHeap()
	{

	}

	void* MimallocHeap::Allocate(size_t size, u32 alignment, bool zeroClear)
	{
		return nullptr;
	}

	void MimallocHeap::Deallocate(void* pBuffer)
	{
	}

	void MimallocHeap::Release()
	{
	}


	//@―-------------------------------------------------------------------------------
	//! @brief	                ヒープ名の取得
	//@―-------------------------------------------------------------------------------
	Char* MimallocHeap::GetName() const {
		return m_pName;
	}

	size_t MimallocHeap::GetHeapSize() const
	{
		return size_t();
	}

	size_t MimallocHeap::GetFreeHeapSize() const
	{
		return size_t();
	}

	bool MimallocHeap::IsValid() const
	{
		return false;
	}

}// namespace ob