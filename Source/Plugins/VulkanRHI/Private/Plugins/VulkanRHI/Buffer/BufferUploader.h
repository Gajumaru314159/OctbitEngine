//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Buffer.h>
#include <Framework/Core/Utility/Swapper.h>

namespace ob::rhi::vulkan {

	class VulkanRHI;

    //! @brief  バッファ・アップローダー
    //! 
    //! バッファのデータを効率的にアップロードするための機能を提供します。
    //! CPUからデータを書き込めるGPUリソースは読み取り速度に制限がかかるため、
    //! 書き込み用のステージングバッファに書き込んだものをまとめてハイパフォーマンスなバッファにコピーします。
    //! コピー処理はフレームの先頭に行われます。必ず毎フレーム描画処理より前に実行してください。         
    class BufferUploader {
    public:

        BufferUploader(VulkanRHI& device,size_t blockSize);

        void add(BlobView blob, vk::raii::Buffer& dest, size_t offset, vk::AccessFlags postAccessFlags);
        void add(const Buffer::CopyFunc& func,size_t size,vk::raii::Buffer& dest,size_t offset, vk::AccessFlags postAccessFlags);

        void update(vk::raii::CommandBuffer& commandBuffer, bool useDebugMarker);

    private:

        void extend();

        void shurink();

    private:

        // コピーリクエストごとの
        // バッファの寿命はBufferImplが保証し、かつGraphicObjectManagerが数フレームの寿命延長を行っているためvk::Bufferでの受け取りで問題ない
        struct Request {
            vk::Buffer source;
            vk::Buffer dest;
            UINT64 sourceOffset;
            UINT64 destOffset;
            UINT64 size;
        };

        struct FrameBlock {
            vk::raii::Buffer buffer = nullptr;
            vk::raii::DeviceMemory memory = nullptr;
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
            void clear() {
                blockIndex = -1;
                for (auto& block : blocks) {
                    block.blob.clear();
                }
                requests.clear();
            }
		};

    private:

        VulkanRHI& m_rhi;

		size_t m_blockSize;

		SpinLock m_lock;
		Swapper<FrameData> m_frames;

		Vector<vk::BufferMemoryBarrier> m_barriers;
		HashMap<VkBuffer, vk::AccessFlags> m_entriedBuffers;
    };

}