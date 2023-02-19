﻿//***********************************************************
//! @file
//! @brief		ディスプレイ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Utility/Swapper.h>
#include <Framework/RHI/Display.h>
#include <Framework/RHI/RenderTexture.h>
#include <Framework/RHI/RenderPass.h>
#include <Framework/RHI/FrameBuffer.h>
#include <Framework/RHI/DescriptorTable.h>
#include <Framework/RHI/PipelineState.h>
#include <Framework/RHI/Buffer.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorHandle.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::rhi::dx12 {
    class DeviceImpl;
    class CommandListImpl;
    class ITexture;
}


//===============================================================
// クラス宣言
//===============================================================
namespace ob::rhi::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class DisplayImpl :public rhi::Display {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        DisplayImpl(DeviceImpl& rDevice, const DisplayDesc& desc);


        //@―---------------------------------------------------------------------------
        //! @brief  デストラクタ
        //@―---------------------------------------------------------------------------
        ~DisplayImpl();


        //@―---------------------------------------------------------------------------
        //! @brief  妥当なオブジェクトか
        //@―---------------------------------------------------------------------------
        bool isValid()const override;


        //@―---------------------------------------------------------------------------
        //! @brief      名前を取得
        //@―---------------------------------------------------------------------------
        const String& getName()const override;


        //===============================================================
        // ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  バックバッファの数を取得
        //@―---------------------------------------------------------------------------
        const DisplayDesc& getDesc()const noexcept override;


        //===============================================================
        // 更新
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      更新
        //! 
        //! @details    表示するテクスチャを次のバックバッファにします。
        //@―---------------------------------------------------------------------------
        void update() override;


        //@―---------------------------------------------------------------------------
        //! @brief      デスクリプタCPUハンドルを取得
        //@―---------------------------------------------------------------------------
        D3D12_CPU_DESCRIPTOR_HANDLE getCpuHandle()const;


        //@―---------------------------------------------------------------------------
        //! @brief      デスクリプタGPUハンドルを取得
        //@―---------------------------------------------------------------------------
        D3D12_GPU_DESCRIPTOR_HANDLE getGpuHandle()const;


        //@―---------------------------------------------------------------------------
        //! @brief      ビューポートを取得
        //@―---------------------------------------------------------------------------
        D3D12_VIEWPORT getViewport()const;


        //@―---------------------------------------------------------------------------
        //! @brief      シザー矩形を取得
        //@―---------------------------------------------------------------------------
        D3D12_RECT getScissorRect()const;


        //@―---------------------------------------------------------------------------
        //! @brief      リソース取得
        //@―---------------------------------------------------------------------------
        ID3D12Resource* getResource()const;


        void recordApplyDisplay(CommandListImpl&, const Ref<Texture>& texture);

    private:

        bool createDisplay(DeviceImpl& rDevice);
        bool createResources(DeviceImpl& rDevice);
        bool createBuffers(DeviceImpl& rDevice);
        
        bool setColorSpace();


    private:

        DisplayDesc m_desc;
        
        ComPtr<IDXGISwapChain4>     m_swapChain;
        Swapper<Ref<RenderTexture>> m_textures;         
        Swapper<Ref<FrameBuffer>>   m_buffers;

        // 描画リソース
        Ref<RenderPass>             m_renderPass;       
        Ref<RootSignature>          m_signature;
        Ref<PipelineState>          m_pipeline;
        Ref<Buffer>                 m_verices;
        Ref<Texture>                m_bindedTexture;
        Ref<DescriptorTable>        m_bindedTextureTable;

        D3D12_VIEWPORT              m_viewport;
        D3D12_RECT                  m_scissorRect;

        UINT m_syncInterval;

        bool m_initialized = false;



    };

}// namespcae ob::rhi::dx12