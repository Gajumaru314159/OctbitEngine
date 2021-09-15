//***********************************************************
//! @file
//! @brief		ヒープ基底クラス
//! @author		Gajumaru
//***********************************************************
#include <Core/Memory/Heap/Heap.hpp>

#include <memory>

namespace ob
{

	namespace
	{

		static const u32 g_signature = 0xDEADC0DE;		//!< 署名(メモリ破壊チェック用)

	}

	//-----------------------------------------------------------
	//! @brief				構造体のセット
	//-----------------------------------------------------------
	void Heap::Header::Set(Heap* heap, Size size, Size adjustment)
	{
		this->heap = heap;
		this->size = size;
		this->adjustment = adjustment;
		this->signature = g_signature;
	}


	//-----------------------------------------------------------
	//! @brief				メモリ確保
	//! 
	//! @param size			確保するメモリ容量
	//! @param alignment	アラインメント(1以上128以下の2のベキ乗)
	//! @return				割り当てられたメモリのポインタ
	//-----------------------------------------------------------
	void* Heap::Allocate(Size size, Size alignment)
	{
		if ((alignment & (alignment - 1)) != 0)OB_ERROR(TC("アライメントは2のべき乗である必要があります。"));
		if (alignment < 1 && 128 < alignment)OB_ERROR(TC("アライメントは1以上128以下である必要があります。"));

		constexpr Size headerSize = sizeof(Header);
		constexpr Size headerAlignment = alignof(Header);
		alignment = OB_MAX(alignment, headerAlignment);

		// ヘッダー用に2回分アライメントを追加
		Size allocSize = alignment + headerSize + alignment + size;

#ifdef OB_MEMORY_DESTRUCT_CHECK
		allocSize += sizeof(g_signature);//チェックバイト
#endif

		void* rawPtr = this->AllocateImpl(allocSize);

		// アラインされたポインタを取得
		void* ptr = reinterpret_cast<Byte*>(rawPtr) + headerSize;
		Size space = allocSize - headerSize;
		std::align(alignment, size, ptr, space);

		// ヘッダの書き込み
		Header* pHeader = reinterpret_cast<Header*>(ptr) - 1;
		pHeader->Set(this, size, (Size)ptr - (Size)rawPtr);

#ifdef OB_MEMORY_DESTRUCT_CHECK
		// メモリ破壊チェックの値を追加
		SetSignature(reinterpret_cast<Byte*>(ptr) + size);
#endif
		return ptr;
	}


	//-----------------------------------------------------------
	//! @brief		メモリ開放
	//! 
	//! @param ptr	解放するポインタ
	//-----------------------------------------------------------
	void Heap::Deallocate(void* ptr)
	{
		OB_REQUIRE(ptr != nullptr);

		Header* pTag = reinterpret_cast<Header*>(ptr) - 1;
		if (pTag->heap != this)OB_ERROR(TC("異なるアロケータで確保されたメモリ領域を解放しようとしました。"));

#ifdef OB_MEMORY_DESTRUCT_CHECK
		if (CheckCheckValue(reinterpret_cast<Byte*>(ptr) + pTag->size) == FALSE)
		{
			OB_ERROR("メモリの破壊を検知しました。");
		}
#endif

		Byte* rawPtr = reinterpret_cast<Byte*>(ptr) - pTag->adjustment;
		pTag->heap->DeallocateImpl(rawPtr);
	}


	//-----------------------------------------------------------
	//! @brief		指定ポインタにシグネチャを書き込む
	//-----------------------------------------------------------
	void Heap::SetSignature(void* ptr)
	{
		*reinterpret_cast<u32*>(ptr) = g_signature;
	}


	//-----------------------------------------------------------
	//! @brief		指定ポインタのシグネチャが正しいか調べる
	//-----------------------------------------------------------
	b32 Heap::CheckSignature(void* ptr)const
	{
		return *reinterpret_cast<u32*>(ptr) == g_signature;
	}

}// namespace ob