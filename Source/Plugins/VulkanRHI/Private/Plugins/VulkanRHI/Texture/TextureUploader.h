//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Buffer.h>
#include <Framework/Core/Utility/Swapper.h>

namespace ob::rhi::vulkan {

    class VulkanRHI;

    //! @brief  テクスチャ・アップローダー
    //! 
    //! テクスチャのデータを効率的にアップロードするための機能を提供します。
    //! CPUからデータを書き込めるGPUリソースは読み取り速度に制限がかかるため、
    //! 書き込み用のステージングバッファに書き込んだものをまとめてハイパフォーマンスなバッファにコピーします。
    //! コピー処理はフレームの先頭に行われます。必ず毎フレーム描画処理より前に実行してください。         
    class TextureUploader {
    public:
        struct Subresource {
            BlobView data;
            UINT rowPitch;
            UINT slicePitch;
        };
    public:

        TextureUploader(VulkanRHI& rhi);

        void add(const vk::raii::Image& dest,vk::ImageCreateInfo info, Span<Subresource> subresources);

        void update(vk::raii::CommandBuffer& commandBuffer);

    private:

        vk::raii::DeviceMemory createUploadResource(const vk::BufferCreateInfo& info);

    private:

        // コピーリクエスト
        struct Request {
            vk::Image dest;

            vk::raii::Buffer source;
            vk::raii::DeviceMemory memory;

            vk::ImageLayout destLayout;
            u32 mipLevels;
			u32 layerCount;

            // UINT sourceSubresource = 0;
            // UINT destSubresource = 0;
            // 
            // IntVec3 destOffset;
            // 
            // D3D12_TEXTURE_COPY_LOCATION destLocation;
            // D3D12_TEXTURE_COPY_LOCATION sourceLocation;
            // UINT DstX;
            // UINT DstY;
            // UINT DstZ;
            // D3D12_BOX sourceBox;
        };

        struct FrameData {
            Vector<Request>     requests;

            void clear() {
                requests.clear();
            }
		};

    private:

        VulkanRHI& m_rhi;
        bool m_isUMA = false;

		size_t m_blockSize;

		SpinLock m_lock;
		Swapper<FrameData> m_frames;

        Vector<vk::ImageMemoryBarrier> m_barriers;
        Vector<vk::MemoryBarrier> m_barriers2;

    };

}