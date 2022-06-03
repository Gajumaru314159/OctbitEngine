//***********************************************************
//! @file
//! @brief		デスクリプタ・ヒープ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/DescriptorDesc.h>

namespace ob::graphic::dx12 {

	//@―---------------------------------------------------------------------------
	//! @brief      BoundaryBlock
	//! 
	//! @details    ヒープの分割された範囲を表す構造体
	//@―---------------------------------------------------------------------------
	struct BBlock {
		class DescriptorHeap* pHeap;//!< 確保元ヒープ
		BBlock* pPrev;      //!< 前のポインタ(ブロックヒープ内)
		BBlock* pNext;      //!< 次のポインタ(ブロックヒープ内)
		BBlock* pFreePrev;  //!< 前のポインタ(フリーブロック内)
		BBlock* pFreeNext;  //!< 次のポインタ(フリーブロック内)
		s32     index;      //!< 割り当て位置
		s32     capacity;   //!< 割り当て可能容量
		bool    allocated;  //!< 確保済みか
	};


	//@―---------------------------------------------------------------------------
	//! @brief      デスクリプタのアロケータ
	//! 
	//! @details    Two-Level Segregate Fit を利用してデスクリプタを割り当てます。
	//@―---------------------------------------------------------------------------
	class DescriptorHeap :private Noncopyable {
		friend class DescriptorHandle;
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief          コンストラクタ
		//! 
		//! @param device   デバイス
		//! @param type     ヒープ・タイプ
		//! @param capacity 容量
		//@―---------------------------------------------------------------------------
		DescriptorHeap(class DeviceImpl& device, DescriptorHeapType type, s32 capacity);


		//@―---------------------------------------------------------------------------
		//! @brief          デストラクタ
		//@―---------------------------------------------------------------------------
		~DescriptorHeap();


		//@―---------------------------------------------------------------------------
		//! @brief          ハンドルをアロケート
		//! 
		//! @param handle   アロケート先ハンドル
		//! @param size  割り当て個数
		//@―---------------------------------------------------------------------------
		void allocateHandle(class DescriptorHandle& handle, s32 size);


		//@―---------------------------------------------------------------------------
		//! @brief          ハンドルを解放
		//@―---------------------------------------------------------------------------
		void releaseHandle(class DescriptorHandle& handle);


		//@―---------------------------------------------------------------------------
		//! @brief          CPUハンドルを取得
		//@―---------------------------------------------------------------------------
		D3D12_CPU_DESCRIPTOR_HANDLE getCpuHandle(u32 index);


		//@―---------------------------------------------------------------------------
		//! @brief          CPUハンドルを取得
		//@―---------------------------------------------------------------------------
		D3D12_GPU_DESCRIPTOR_HANDLE getGpuHandle(u32 index);


		//@―---------------------------------------------------------------------------
		//! @brief  タイプを取得
		//@―---------------------------------------------------------------------------
		DescriptorHeapType getHeapType()const;


		//@―---------------------------------------------------------------------------
		//! @brief  ネイティブクラスを取得
		//@―---------------------------------------------------------------------------
		const auto& getNative()const { return m_heap; };


	private:

		BBlock* allocateFreeBlock(s32 size);							//!< ブロックをアロケート
		void	returnSurplusBlock(BBlock& block, s32 size);            //!< 余分なブロックを分割してフリーリストに戻す
		BBlock* mergeFreeBlocks(BBlock& block1, BBlock& block2);        //!< 2つのブロックをマージして2つめをフリーブロックに戻す

		void addFreeBlock(BBlock& block);                               //!< フリーブロックをリストに追加
		void separateFreeList(BBlock& block);                           //!< ブロックをフリーブロックから分離

		void entryFreeListBitState(s32 firstLevel, s32 secondLevel);    //!< フリーリストビットへ登録
		void removeFreeListBitState(s32 firstLevel, s32 secondLevel);   //!< フリーリストビットから削除

		s32  getFreeBlockIndex(s32 firstLevel, s32 secondLevel)const noexcept;							//!< レベルからフリーブロックリストのインデックスを計算
		void getLevelIndex(s32 size, s32& firstLevel, s32& secondLevel)const noexcept;					//!< サイズから各レベルのカテゴリを計算
		void getLevelAndIndex(s32 size, s32& firstLevel, s32& secondLevel, s32& index)const noexcept;	//!< サイズからレベルとブロックインデックスを計算


	private:

		ComPtr<ID3D12DescriptorHeap> m_heap;
		u32	m_descriptorSize;

		using mutex_type = Mutex;
		using lock_guard_type = ScopeLock<mutex_type>;
		mutex_type		m_mutex;		//!< ミューテックス

		const DescriptorHeapType m_type;
		s32				m_capacity;     //!< 最大容量

		u32				m_freeFLI;      //!< First Level のフリー・ビットフラグ
		vector<u32>		m_freeSLI;      //!< Second Level のフリー・ビットフラグ

		vector<BBlock>  m_buffer;		//!< バッファ(ブロック実体)
		std::vector<BBlock*> m_freeList;		//!< ブロックリスト(実体)
		vector<BBlock*> m_blocks;       //!< カテゴリに属するブロックの先頭ポインタ

		static s32 s_maxSecondLevelLog2;
		static s32 s_maxSecondLevel;
		static s32 s_linearManagementSize;
		static s32 s_linearManagementSizeLog2;
		static s32 s_secondLevelShift;

	};

}// namespcae ob::graphic::dx12