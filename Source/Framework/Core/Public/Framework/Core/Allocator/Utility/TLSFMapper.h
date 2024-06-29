//***********************************************************
//! @file
//! @brief		デスクリプタ・ヒープ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Utility/Noncopyable.h>
#ifdef ENALBE_TLSF_STACK_TRACE
#include <Framework/Core/Log/StackTrace.h>
#endif

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief      Two Level Segregate Fit Mapper Block
	//@―---------------------------------------------------------------------------
	struct TLSFBlock {
		void*		pUser;		//!< ユーザデータ
		TLSFBlock*	pPrev;      //!< 前のポインタ(ブロックヒープ内)
		TLSFBlock*	pNext;      //!< 次のポインタ(ブロックヒープ内)
		TLSFBlock*	pFreePrev;  //!< 前のポインタ(フリーブロック内)
		TLSFBlock*	pFreeNext;  //!< 次のポインタ(フリーブロック内)
		s32			index;      //!< 割り当て位置
		s32			capacity;   //!< 割り当て可能容量
		bool		allocated;  //!< 確保済みか
#ifdef ENALBE_TLSF_STACK_TRACE
		StackTrace stack;
#endif
	};


	//@―---------------------------------------------------------------------------
	//! @brief      Two Level Segregate Fit Mapper
	//! 
	//! @details    TLSF を利用してブロックを割り当てるユーティリティ。
	//!				ブロックの割り当てのみを行うのでallocateで得られたTLSFBlockの	
	//!				pUserに必要なデータを設定して使用してください。
	//! @note		コンストラクタに設定されたcapacityより大きめのヒープが確保される場合があります。
	//!				実際に確保されたcapacityはTLSFMapper::capacity()で確認してください。
	//@―---------------------------------------------------------------------------
	class TLSFMapper :private Noncopyable {
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief          コンストラクタ
		//! 
		//! @param capacity 容量
		//@―---------------------------------------------------------------------------
		TLSFMapper(s32 capacity);


		//@―---------------------------------------------------------------------------
		//! @brief          デストラクタ
		//@―---------------------------------------------------------------------------
		~TLSFMapper();


		//@―---------------------------------------------------------------------------
		//! @brief          ハンドルをアロケート
		//! 
		//! @param handle   アロケート先ハンドル
		//! @param size  割り当て個数
		//@―---------------------------------------------------------------------------
		auto allocate(s32 size)-> const TLSFBlock*;


		//@―---------------------------------------------------------------------------
		//! @brief          ハンドルを解放
		//@―---------------------------------------------------------------------------
		void free(const TLSFBlock* block);


		//@―---------------------------------------------------------------------------
		//! @brief          キャパシティを取得
		//@―---------------------------------------------------------------------------
		auto capacity()const { return m_capacity; }


	private:

		TLSFBlock*	allocateFreeBlock(s32 size);							//!< ブロックをアロケート
		void		returnSurplusBlock(TLSFBlock& block, s32 size);         //!< 余分なブロックを分割してフリーリストに戻す
		TLSFBlock*	mergeFreeBlocks(TLSFBlock& block1, TLSFBlock& block2);  //!< 2つのブロックをマージして2つめをフリーブロックに戻す

		void addFreeBlock(TLSFBlock& block);								//!< フリーブロックをリストに追加
		void separateFreeList(TLSFBlock& block);							//!< ブロックをフリーブロックから分離

		void entryFreeListBitState(s32 firstLevel, s32 secondLevel);		//!< フリーリストビットへ登録
		void removeFreeListBitState(s32 firstLevel, s32 secondLevel);		//!< フリーリストビットから削除

		s32  getFreeBlockIndex(s32 firstLevel, s32 secondLevel)const noexcept;							//!< レベルからフリーブロックリストのインデックスを計算
		void getLevelIndex(s32 size, s32& firstLevel, s32& secondLevel)const noexcept;					//!< サイズから各レベルのカテゴリを計算
		void getLevelAndIndex(s32 size, s32& firstLevel, s32& secondLevel, s32& index)const noexcept;	//!< サイズからレベルとブロックインデックスを計算


	private:

		s32					m_capacity;     //!< 最大容量

		u32					m_freeFLI;      //!< First Level のフリー・ビットフラグ
		Vector<u32>			m_freeSLI;      //!< Second Level のフリー・ビットフラグ

		Vector<TLSFBlock>  	m_buffer;		//!< バッファ(ブロック実体)
		Vector<TLSFBlock*> 	m_freeList;		//!< ブロックリスト(実体)
		Vector<TLSFBlock*> 	m_blocks;       //!< カテゴリに属するブロックの先頭ポインタ

	};

}