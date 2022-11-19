﻿//***********************************************************
//! @file
//! @brief		コマンドリスト実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/ICommandList.h>
#include <Framework/RHI/Types/CommandListDesc.h>
#include <Framework/RHI/Constants.h>
#include <Framework/Core/Utility/Swapper.h>

namespace ob::rhi::dx12 {

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

#pragma region Command

        //@―---------------------------------------------------------------------------
        //! @brief      記録開始
        //@―---------------------------------------------------------------------------
         void begin() override;

         //@―---------------------------------------------------------------------------
         //! @brief      記録終了
         //@―---------------------------------------------------------------------------
        void end() override;
        
        //@―---------------------------------------------------------------------------
        //! @brief      スワップチェーンにテクスチャを適用
        //@―---------------------------------------------------------------------------
        void applySwapChain(const SwapChain& swapChain, const Texture& texture) override;

        //@―---------------------------------------------------------------------------
        //! @brief      描画開始
        //@―---------------------------------------------------------------------------
        void beginRender(const class rhi::RenderTarget& target) override;

        //@―---------------------------------------------------------------------------
        //! @brief      描画終了
        //@―---------------------------------------------------------------------------
        void endRender() override;

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
        void setVertexBuffers(Span<const Buffer*>)override;

        //@―---------------------------------------------------------------------------
        //! @brief      インデックスバッファを設定
        //@―---------------------------------------------------------------------------
        void setIndexBuffer(const Buffer&)override;

        //@―---------------------------------------------------------------------------
        //! @brief      ルートシグネチャを設定
        //@―---------------------------------------------------------------------------
        void setRootSignature(const RootSignature&) override;

        //@―---------------------------------------------------------------------------
        //! @brief      パイプラインステートを設定
        //@―---------------------------------------------------------------------------
        void setPipelineState(const PipelineState&) override;

        //@―---------------------------------------------------------------------------
        //! @brief      描画
        //@―---------------------------------------------------------------------------
        void draw(const DrawParam&)override;

        //@―---------------------------------------------------------------------------
        //! @brief      インデックス描画
        //@―---------------------------------------------------------------------------
        void drawIndexed(const DrawIndexedParam&)override;


        //@―---------------------------------------------------------------------------
        //! @brief      デスクリプタ・テーブルを設定
        //@―---------------------------------------------------------------------------
        void setRootDesciptorTable(const rhi::SetDescriptorTableParam*, s32 num) override;


        //@―---------------------------------------------------------------------------
        //! @brief      リソースバリアを挿入
        //@―---------------------------------------------------------------------------
        void insertResourceBarrier(const ResourceBarrier&) override;


#ifdef OB_DEBUG
        //@―---------------------------------------------------------------------------
        //! @brief      マーカーを挿入
        //@―---------------------------------------------------------------------------
        void pushMarker(const char* pName) override;

        //@―---------------------------------------------------------------------------
        //! @brief      マーカーを終了
        //@―---------------------------------------------------------------------------
        void popMarker() override;
#endif

#pragma endregion

    public:

        ID3D12GraphicsCommandList5* getNative()const { return m_cmdList.Get(); }

    private:

        void clearDescriptorHandle();
        void onNameChanged()override;

    private:

        class DeviceImpl& m_device;
        const CommandListDesc m_desc;

        ComPtr<ID3D12CommandAllocator> m_cmdAllocator;
        ComPtr<ID3D12GraphicsCommandList5> m_cmdList;

        D3D12_CPU_DESCRIPTOR_HANDLE m_hRTV[RENDER_TARGET_MAX];  // 現在の描画ターゲット(クリア用)
        D3D12_CPU_DESCRIPTOR_HANDLE m_hDSV;                     // 現在の描画ターゲット(クリア用)

        const class RenderTarget* m_pRenderTarget = nullptr;

        Array<D3D12_RESOURCE_BARRIER> m_barriers;

    };

}// ob::rhi::dx12 