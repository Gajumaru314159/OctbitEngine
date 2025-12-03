//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Utility/Swapper.h>

namespace ob::rhi {

    //! @brief  バッファ・アップローダー
    //! 
    //! バッファのデータを効率的にアップロードするための機能を提供します。
    //! CPUからデータを書き込めるGPUリソースは読み取り速度に制限がかかるため、
    //! 書き込み用のステージングバッファに書き込んだものをまとめてハイパフォーマンスなバッファにコピーします。
    //! コピー処理はフレームの先頭に行われます。必ず毎フレーム描画処理より前に実行してください。         
    class DirectX12DescriptorUploader {
    public:

        DirectX12DescriptorUploader(ID3D12Device& device);

        void add(D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_CPU_DESCRIPTOR_HANDLE src, D3D12_CPU_DESCRIPTOR_HANDLE dst);
        void update();

    private:


    private:
        
        struct Request {
			D3D12_CPU_DESCRIPTOR_HANDLE src; //!< コピー元のCPUハンドル
			D3D12_CPU_DESCRIPTOR_HANDLE dst; //!< コピー先のCPUハンドル
        };

        struct FrameData {
            Vector<Request>     requests[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];

            void clear() {
                for (auto& reqs : requests) {
                    reqs.clear();
                }
            }
        };
    private:

        ID3D12Device& m_device;

		SpinLock m_lock;
		Swapper<FrameData> m_frames;

    };

}