//***********************************************************
//! @file
//! @brief		OSヒープ
//! @author		Gajumaru
//***********************************************************
#include <Core/Memory/Heap/OSHeap.hpp>

#include <memory>

namespace ob
{

	//-----------------------------------------------------------
	//! @brief 内部メモリ確保
	//! 
	//! @param size 確保するメモリ容量
	//! @return 割り当てられたメモリのポインタ
	//-----------------------------------------------------------
	void* OSHeap::AllocateImpl(Size size)
	{
		return new Byte[size];
	}


	//-----------------------------------------------------------
	//! @brief 内部メモリ開放
	//! 
	//! @param ptr 解放するポインタ
	//-----------------------------------------------------------
	void OSHeap::DeallocateImpl(void* ptr)
	{
		OB_REQUIRE(ptr != nullptr);
		delete[] ptr;
	}

}// namespace ob