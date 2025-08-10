//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Buffer.h>
#include <Framework/Core/Utility/Swapper.h>

namespace ob::rhi {

    //! @brief  バッファ・アップローダー
    //! 
    //! バッファのデータを効率的にアップロードするための機能を提供します。
    //! CPUからデータを書き込めるGPUリソースは読み取り速度に制限がかかるため、
    //! 書き込み用のステージングバッファに書き込んだものをまとめてハイパフォーマンスなバッファにコピーします。
    //! コピー処理はフレームの先頭に行われます。必ず毎フレーム描画処理より前に実行してください。         
    class DirectX12BufferUploader {
    public:

        DirectX12BufferUploader(ID3D12Device& device,size_t blockSize);

        void add(BlobView blob, const ComPtr<ID3D12Resource>& dest, size_t offset);
        void add(const Buffer::CopyFunc& func,size_t size, const ComPtr<ID3D12Resource>& dest,size_t offset);

        void update(ID3D12GraphicsCommandList& commandList);

    private:

        void extend(size_t size);

    private:

        // コピーリクエストごとの
        struct Request {
            ComPtr<ID3D12Resource> source;
            ComPtr<ID3D12Resource> dest;
            UINT64 sourceOffset;
            UINT64 destOffset;
            UINT64 size;
        };

        struct FrameBlock {
            ComPtr<ID3D12Resource> resource;
            Blob blob;

			bool available(size_t size)const {
                return size <= blob.capacity() - blob.size();
			}
        };

        struct FrameData {
			s32                 blockIndex = -1;
            Vector<FrameBlock>  blocks;
            Vector<Request>     requests;

            FrameBlock& block() {
                return blocks.at(blockIndex);
            }

            bool available(size_t size)const {
                return -1 < blockIndex &&  blockIndex < blocks.size() && blocks[blockIndex].available(size);
            }
            void clear(size_t removeThreshold) {
                blockIndex = -1;
                for (auto itr = blocks.begin(); itr != blocks.end();) {
                    auto& block = *itr;
                    if (removeThreshold < block.blob.capacity()) {
                        itr = blocks.erase(itr);
                    } else {
                        block.blob.clear();
                        ++itr;
                    }
                }
                requests.clear();
            }
		};

    private:

        ID3D12Device& m_device;

		size_t m_blockSize;

		SpinLock m_lock;
		Swapper<FrameData> m_frames;

		Vector<D3D12_RESOURCE_BARRIER> m_barriers;

        HashSet<ID3D12Resource*> m_enteredResources;

    };

}