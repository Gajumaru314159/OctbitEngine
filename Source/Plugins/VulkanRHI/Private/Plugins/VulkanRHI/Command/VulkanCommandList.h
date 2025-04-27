//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/CommandList.h>
#include <Framework/RHI/Types/CommandListDesc.h>
#include <Framework/RHI/Constants.h>
#include <Framework/Core/Utility/Swapper.h>
#include <Plugins/VulkanRHI/Command/VulkanResourceStateCache.h>

namespace ob::rhi {

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

        //! @brief      スワップチェーンにテクスチャを適用
        void applySwapChain(const Ref<SwapChain>& swapChain, const Ref<RenderTexture>& texture) override;

        //! @brief      シザー矩形を設定
        void setScissorRect(const IntRect* pRect, s32 num) override;

        //! @brief      ビューポートを設定
        void setViewport(const Viewport* pViewport, s32 num) override;

        //! @brief      頂点バッファを設定
        void setVertexBuffers(Span<Ref<Buffer>> buffers, s32 first)override;

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


        //! @brief      GPUマーカーをプッシュ
        void pushMarker(StringView name) override;


        //! @brief      GPUマーカーをポップ
        void popMarker() override;

#pragma endregion

    public:

		vk::CommandBuffer getNative() {
			return m_commandBuffer;
		}

    private:

        void clearRenderTargets();

    private:

        VulkanRHI& m_rhi;

        const CommandListDesc m_desc;
		
        vk::raii::CommandPool		m_commandPool = nullptr;
        vk::raii::CommandBuffer     m_commandBuffer = nullptr;

		Ref<PipelineState> m_pipeline;                          // 現在のパイプラインステート
        RenderTargetArray m_colorTextures;                     // 現在の描画ターゲット(クリア用)
        Ref<RenderTexture> m_depthTexture;                      // 現在の描画ターゲット(クリア用)

        VulkanResourceStateCache m_cache;

    };

}// ob::rhi 