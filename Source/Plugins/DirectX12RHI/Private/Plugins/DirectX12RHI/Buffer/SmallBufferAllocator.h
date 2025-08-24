//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Allocator/Utility/TLSFMapper.h>
#include <Framework/Core/Utility/Noncopyable.h>
#include <Framework/RHI/Types/BufferDesc.h>
#include <d3d12.h>
#include <wrl/client.h>

namespace ob::rhi {

	using Microsoft::WRL::ComPtr;
	using core::TLSFMapper;
	using core::TLSFBlock;

	//! @brief バッファの用途別アライメント情報
	enum class BufferUsageAlignment : u32 {
		None = 1,           //!< アライメントなし（頂点バッファ等）
		Index16 = 2,        //!< 16bitインデックスバッファ
		Index32 = 4,        //!< 32bitインデックスバッファ  
		ConstantBuffer = 256, //!< 定数バッファ
		ByteAddress = 16,   //!< ByteAddressBuffer
		TypedR32 = 4,      //!< R32フォーマット
		TypedRG32 = 8,     //!< RG32フォーマット
		TypedRGBA32 = 16,  //!< RGBA32フォーマット
	};

	//! @brief バッファ確保情報
	struct BufferAllocation {
		ID3D12Resource* resource;   //!< DirectX12リソース
		size_t offset;             //!< ビュー開始オフセット
		size_t size;               //!< ビューサイズ
		const TLSFBlock* block;    //!< TLSFブロック（解放用）
		bool isSubAllocation;      //!< サブアロケーションか
	};

	//! @brief Small Buffer Allocator
	//!
	//! @details D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT以下の
	//!          小さなバッファを効率的に管理するアロケーター
	class SmallBufferAllocator : private core::Noncopyable {
	public:

		//! @brief コンストラクタ
		//! @param device DirectX12デバイス
		//! @param heapType ヒープタイプ
		//! @param chunkSize チャンクサイズ（デフォルト64KB）
		SmallBufferAllocator(ID3D12Device& device, D3D12_HEAP_TYPE heapType, size_t chunkSize = 65536);

		//! @brief デストラクタ
		~SmallBufferAllocator();

		//! @brief バッファをアロケート
		//! @param size バッファサイズ
		//! @param alignment アライメント要求
		//! @return 確保情報
		BufferAllocation allocate(size_t size, BufferUsageAlignment alignment);

		//! @brief バッファを解放
		//! @param allocation 確保情報
		void free(const BufferAllocation& allocation);

		//! @brief アライメント調整されたサイズを計算
		//! @param size 元のサイズ
		//! @param alignment アライメント要求
		//! @return アライメント調整後のサイズ
		static size_t GetAlignedSize(size_t size, BufferUsageAlignment alignment);

	private:

		//! @brief バッファチャンク
		struct BufferChunk {
			ComPtr<ID3D12Resource> resource;  //!< 64KBリソース
			TLSFMapper mapper;               //!< メモリマッパー
			D3D12_HEAP_TYPE heapType;        //!< ヒープタイプ

			BufferChunk(ID3D12Device& device, D3D12_HEAP_TYPE type, size_t size);
		};

		//! @brief 新しいチャンクを作成
		core::UPtr<BufferChunk> createChunk();

		//! @brief 指定サイズを確保できるチャンクを検索
		BufferChunk* findAvailableChunk(size_t alignedSize);

	private:
		ID3D12Device& m_device;                   //!< DirectX12デバイス
		D3D12_HEAP_TYPE m_heapType;               //!< ヒープタイプ
		size_t m_chunkSize;                      //!< チャンクサイズ
		Vector<core::UPtr<BufferChunk>> m_chunks; //!< チャンクリスト
		mutable core::SpinLock m_spinLock;        //!< スピンロック
	};

	//! @brief バッファ用途からアライメント要求を取得
	BufferUsageAlignment getAlignmentFromUsage(rhi::BufferState usage);

}