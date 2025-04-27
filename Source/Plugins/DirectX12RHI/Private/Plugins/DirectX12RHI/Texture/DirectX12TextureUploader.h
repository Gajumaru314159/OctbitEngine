//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Buffer.h>
#include <Framework/Core/Utility/Swapper.h>

namespace ob::rhi {

    //! @brief  テクスチャ・アップローダー
    //! 
    //! テクスチャのデータを効率的にアップロードするための機能を提供します。
    //! CPUからデータを書き込めるGPUリソースは読み取り速度に制限がかかるため、
    //! 書き込み用のステージングバッファに書き込んだものをまとめてハイパフォーマンスなバッファにコピーします。
    //! コピー処理はフレームの先頭に行われます。必ず毎フレーム描画処理より前に実行してください。         
    class DirectX12TextureUploader {
    public:
        struct Subresource {
            BlobView data;
            UINT rowPitch;
            UINT slicePitch;
        };
    public:

        DirectX12TextureUploader(ID3D12Device& device);

        void add(const ComPtr<ID3D12Resource>& dest, Span<Subresource> subresources);

        void update(ID3D12GraphicsCommandList& commandList);

    private:

        ComPtr<ID3D12Resource> createUploadResource(const D3D12_RESOURCE_DESC& desc);

    private:

        // コピーリクエスト
        struct Request {
            ComPtr<ID3D12Resource> source;
            ComPtr<ID3D12Resource> dest;
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

        ID3D12Device& m_device;
        bool m_isUMA = false;

		size_t m_blockSize;

		SpinLock m_lock;
		Swapper<FrameData> m_frames;

		Vector<D3D12_RESOURCE_BARRIER> m_barriers;

    };

}