//***********************************************************
//! @file
//! @brief		コマンドリスト実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/CommandList.h>
#include <Framework/RHI/Types/CommandListDesc.h>
#include <Framework/RHI/Constants.h>
#include <Framework/Core/Utility/Swapper.h>

namespace ob::rhi::vulkan {

    class VulkanRHI;

    //@―---------------------------------------------------------------------------
    //! @brief  コマンドリスト実装(DirectX12)
    //@―---------------------------------------------------------------------------
    class VulkanCommandList:public CommandList {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        VulkanCommandList(VulkanRHI& rhi,const CommandListDesc& desc);

        ~VulkanCommandList();

        //! @brief      名前を取得
        const String& getName()const override;


        //===============================================================
        // コマンド
        //===============================================================

#pragma region Command

        //! @brief      記録開始
        void begin() override;

        //! @brief      記録終了
        void end() override;

        //! @brief      記録終了
        void flush() override;

        void beginRenderPass(const RenderPassDesc& param) override;
        // virtual void nextSubpass();
        void endRenderPass() override;

        //! @brief      ディスプレイにテクスチャを適用
        void applyDisplay(const Ref<Display>& display, const Ref<RenderTexture>& texture) override;

        //! @brief      シザー矩形を設定
        void setScissorRect(const IntRect* pRect, s32 num) override;

        //! @brief      ビューポートを設定
        void setViewport(const Viewport* pViewport, s32 num) override;

        //! @brief      レンダーターゲットの色をRenderTargetに設定した色でクリア
        void clearColors(u32 mask) override;

        //! @brief      レンダーターゲットのデプスとステンシルをクリア
        void clearDepthStencil() override;

        //! @brief      頂点バッファを設定
        void setVertexBuffers(Span<Ref<Buffer>>)override;

        //! @brief      インデックスバッファを設定
        void setIndexBuffer(const Ref<Buffer>&)override;

        //! @brief      パイプラインステートを設定
        void setPipelineState(const Ref<PipelineState>&) override;

        //! @brief      描画
        void draw(const DrawParam&)override;

        //! @brief      インデックス描画
        void drawIndexed(const DrawIndexedParam&)override;


        //! @brief      デスクリプタ・テーブルを設定
        void setRootDesciptorTable(const rhi::SetDescriptorTableParam*, s32 num) override;


        //! @brief      ルート定数を設定
        void setRootConstant(const SetRootConstantsParam&) override;


        //! @brief      リソースバリアを挿入
        void insertResourceBarrier(const ResourceBarrier&) override;

        //! @brief      GPUマーカーをプッシュ
        void pushMarker(StringView name) override;

        //! @brief      GPUマーカーをポップ
        void popMarker() override;

#pragma endregion


    private:

        VulkanRHI& m_rhi;

        const CommandListDesc m_desc;
		
        vk::raii::CommandPool		m_commandPool = nullptr;
        vk::raii::CommandBuffer     m_commandBuffer = nullptr;

    };

}// ob::rhi::dx12 