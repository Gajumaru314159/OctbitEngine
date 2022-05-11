﻿//***********************************************************
//! @file
//! @brief		コマンドリスト実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/ICommandList.h>
#include <Framework/Graphic/Types/CommandListDesc.h>
#include <Framework/Graphic/Constants.h>
#include <Framework/Core/Utility/Swapper.h>

namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  コマンドリスト実装(DirectX12)
    //@―---------------------------------------------------------------------------
    class CommandListImpl:public ICommandList {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        CommandListImpl(class DeviceImpl& device, const CommandListDesc& desc);


        //@―---------------------------------------------------------------------------
        //! @brief  妥当な状態か
        //@―---------------------------------------------------------------------------
        bool isValid()const override;


        //===============================================================
        // コマンド
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      記録開始
        //@―---------------------------------------------------------------------------
         void begin() override;

         //@―---------------------------------------------------------------------------
         //! @brief      記録終了
         //@―---------------------------------------------------------------------------
        void end() override;

        //@―---------------------------------------------------------------------------
        //! @brief      レンダーターゲットを設定
        //@―---------------------------------------------------------------------------
        void setRenderTarget(const class SwapChain& target) override;

        //@―---------------------------------------------------------------------------
        //! @brief      レンダーターゲットを設定
        //@―---------------------------------------------------------------------------
        void setRenderTarget(const class RenderTarget& target) override;

        //@―---------------------------------------------------------------------------
        //! @brief      シザー矩形を設定
        //@―---------------------------------------------------------------------------
        void setScissorRect(const IntRect* pRect, s32 num) override;

        //@―---------------------------------------------------------------------------
        //! @brief      ビューポートを設定
        //@―---------------------------------------------------------------------------
        void setViewport(const Viewport* pViewport, s32 num) override;

        //@―---------------------------------------------------------------------------
        //! @brief      レンダーターゲットの色をRenderTargetに設定した色でクリア
        //@―---------------------------------------------------------------------------
        void clearColors(u32 mask) override;

        //@―---------------------------------------------------------------------------
        //! @brief      レンダーターゲットのデプスとステンシルをクリア
        //@―---------------------------------------------------------------------------
        void clearDepthStencil();

        //@―---------------------------------------------------------------------------
        //! @brief      頂点バッファを設定
        //@―---------------------------------------------------------------------------
        void setVertexBuffer(const Buffer&)override;

        //@―---------------------------------------------------------------------------
        //! @brief      インデックスバッファを設定
        //@―---------------------------------------------------------------------------
        void setIndexBuffer(const Buffer&)override;



        void setRootSignature(const RootSignature&) override;
        void setPipelineState(const PipelineState&) override;

        //@―---------------------------------------------------------------------------
        //! @brief      インデックスバッファを設定
        //@―---------------------------------------------------------------------------
        void drawIndexed()override;


        //@―---------------------------------------------------------------------------
        //! @brief      デスクリプタ・テーブルを設定
        //@―---------------------------------------------------------------------------
        void setRootDesciptorTable(const graphic::SetDescriptorTableParam*, s32 num) override;


        //@―---------------------------------------------------------------------------
        //! @brief      リソースバリアを挿入
        //@―---------------------------------------------------------------------------
        void insertResourceBarrier(const ResourceBarrier&) override;


#ifdef OB_DEBUG
        void pushMarker(const char* pName) override;
        void popMarker() override;
#endif

    public:

        const ComPtr<ID3D12GraphicsCommandList5>& getNative()const {
            return m_cmdList;
        }

    private:

        void clearDescriptorHandle();

    private:

        class DeviceImpl& m_device;
        const CommandListDesc m_desc;
        ComPtr<ID3D12CommandAllocator> m_cmdAllocator;
        ComPtr<ID3D12GraphicsCommandList5> m_cmdList;

        D3D12_CPU_DESCRIPTOR_HANDLE m_hRTV[RENDER_TARGET_MAX];  // 現在の描画ターゲット(クリア用)
        D3D12_CPU_DESCRIPTOR_HANDLE m_hDSV;                     // 現在の描画ターゲット(クリア用)

        const class RenderTarget* m_pRenderTarget=nullptr;



        vector<D3D12_RESOURCE_BARRIER> m_barriers;

        //// class DepthStencilView
        //s32 m_listIndex;
        //bool m_depthReadOnly;
        //
        //class FrameBufferImpl*    m_pFrameBuffer;
        //s32                 m_subpassIndex;
        //vector<ResourceState> m_atttachmentLayout;

        //D3D12_RECT m_renderArea;
        //bool m_bRenderArea;

    };

}// ob::graphic::dx12 