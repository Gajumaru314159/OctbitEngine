//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "MimallocHeap.h"

namespace ob
{

	//@―---------------------------------------------------------------------------
	MimallocHeap::MimallocHeap(void* pBuffer, size_t size)
	{
		m_pName = TC("MimallocHeap");
		OB_REQUIRE(pBuffer != nullptr);
	}

	MimallocHeap::~MimallocHeap()
	{

	}

	void* MimallocHeap::allocate(size_t size, u32 alignment, bool zeroClear)
	{
		return nullptr;
	}

	void MimallocHeap::deallocate(void* pBuffer)
	{
	}

	void MimallocHeap::release()
	{
	}


	//@―---------------------------------------------------------------------------
	//! @brief	                ヒープ名の取得
	//@―---------------------------------------------------------------------------
	Char* MimallocHeap::getName() const {
		return m_pName;
	}

	size_t MimallocHeap::getHeapSize() const
	{
		return size_t();
	}

	size_t MimallocHeap::getFreeHeapSize() const
	{
		return size_t();
	}

	bool MimallocHeap::isValid() const
	{
		return false;
	}

}// namespace ob