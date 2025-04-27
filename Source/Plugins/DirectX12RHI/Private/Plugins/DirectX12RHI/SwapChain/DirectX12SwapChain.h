//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Utility/Swapper.h>
#include <Framework/Platform/Type/SystemEventType.h>
#include <Framework/RHI/SwapChain.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/RHI/PipelineState.h>
#include <Framework/RHI/Buffer.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorHandle.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::rhi {
    class DirectX12RHI;
    class DirectX12CommandList;
    class ITexture;
}


//===============================================================
// クラス宣言
//===============================================================
namespace ob::rhi {

    //! @brief  説明
    class DirectX12SwapChain :public rhi::SwapChain {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //! @brief  コンストラクタ
        DirectX12SwapChain(DirectX12RHI& rDevice, const SwapChainDesc& desc);


        //! @brief  デストラクタ
        ~DirectX12SwapChain();


        //! @brief  妥当なオブジェクトか
        bool isValid()const;


        //! @brief      名前を取得
        const String& getName()const override;


        //! @brief  バックバッファの数を取得
        const SwapChainDesc& getDesc()const noexcept override;


        //! @brief      更新
        //! 
        //! @details    表示するテクスチャを次のバックバッファにします。
        void update() override;


        //! @brief      イベントリスナ追加
        void addEventListener(SwapChainEventHandle& handle, SwapChainEventDelegate func)override;


    public:

        //! @brief      デスクリプタCPUハンドルを取得
        D3D12_CPU_DESCRIPTOR_HANDLE getCpuHandle()const;


        //! @brief      デスクリプタGPUハンドルを取得
        D3D12_GPU_DESCRIPTOR_HANDLE getGpuHandle()const;


        //! @brief      ビューポートを取得
        D3D12_VIEWPORT getViewport()const;


        //! @brief      シザー矩形を取得
        D3D12_RECT getScissorRect()const;


        //! @brief      リソース取得
        ID3D12Resource* getResource()const;


        //! @brief      テクスチャをスワップチェーンにコピー
        void recordApplySwapChain(DirectX12CommandList&, const Ref<Texture>& texture);

    private:

        void onWindowChanged(const platform::WindowEventArgs& args);

    private:

        bool createSwapChain(DirectX12RHI& rDevice);
        bool createResources(DirectX12RHI& rDevice);
        bool createBuffers(DirectX12RHI& rDevice);
        
        bool setColorSpace();


    private:

        DirectX12RHI& m_device;

        SwapChainDesc m_desc;

        platform::WindowEventHandle m_hEvent;
        
        ComPtr<IDXGISwapChain4>     m_swapChain;
        Swapper<Ref<RenderTexture>> m_textures;

        // 描画リソース    
        Ref<DescriptorLayout>       m_layout;
        Ref<RootSignature>          m_signature;
        Ref<PipelineState>          m_pipeline;
        Ref<Buffer>                 m_verices;
        Ref<Texture>                m_bindedTexture;
        Ref<DescriptorTable>        m_bindedTextureTable;

        D3D12_VIEWPORT              m_viewport;
        D3D12_RECT                  m_scissorRect;

        UINT m_syncInterval;
        UINT m_flags;

        SwapChainEventNotifier        m_notifier;
        bool m_visible=true;

        s32                         m_resizeCountDown = -1;
        Vec2                        m_newSize;
    };

}