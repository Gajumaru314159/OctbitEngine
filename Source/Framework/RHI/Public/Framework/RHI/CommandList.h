//***********************************************************
//! @file
//! @brief		コマンドリスト・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Geometory/Viewport.h>
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Types/CommandListDesc.h>
#include <Framework/RHI/Types/CommandParam.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::rhi {
    struct SetDescriptorTableParam;
    struct DrawParam;
    struct DrawIndexedParam;
}

//===============================================================
// クラス宣言
//===============================================================
namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      コマンドリスト・インターフェイス
    //@―---------------------------------------------------------------------------
    class CommandList :public GraphicObject {
    public:

        static Ref<CommandList> Create(const CommandListDesc& desc);

    public:

        virtual void begin() = 0;
        virtual void end() = 0;
        virtual void flush() = 0;

        virtual void beginRenderPass(const Ref<FrameBuffer>& frameBuffer) = 0; //!< レンダーパス開始
        virtual void nextSubpass() = 0;                                 //!< 次のサブパスに進める
        virtual void endRenderPass() = 0;                               //!< レンダーパス終了

        virtual void applyDisplay(const Ref<Display>& display, const Ref<RenderTexture>& texture) = 0;

        virtual void setViewport(const Viewport* pViewport, s32 num) = 0;  //!< ビューポートを設定
        virtual void setScissorRect(const IntRect* pRect, s32 num) = 0;    //!< シザー矩形を設定

        virtual void clearColors(u32 mask) = 0;                         //!< カラーをクリア
        virtual void clearDepthStencil() = 0;                           //!< 深度値をクリア

                void setVertexBuffer(const Ref<Buffer>&);         //!< 頂点バッファ設定
        virtual void setVertexBuffers(Span<const Ref<Buffer>*>) = 0;         //!< 頂点バッファ設定
        virtual void setIndexBuffer(const Ref<Buffer>&) = 0;                 //!< インデックスバッファ設定


        virtual void setRootSignature(const Ref<RootSignature>&) = 0;// TODO 削除
        virtual void setPipelineState(const Ref<PipelineState>&) = 0;        //!< パイプライン設定
        //virtual void setComputePipelineState(const ComputePipelineState&) = 0;

        // setBlendFactor
        // setStencilRef

        virtual void setRootDesciptorTable(const SetDescriptorTableParam*, s32 num) = 0;
        // void setRootDescriptor();
        // void setRootConstant();


        virtual void draw(const DrawParam& param) = 0;
        virtual void drawIndexed(const DrawIndexedParam& param) = 0;
        //virtual void drawInstanced() = 0;


        // バッファのコピー
        //virtual void copyBuffer();
        //virtual void copyBufferRegion();
        //
        //virtual void copyTexture(Texture& src, Texture& dst) = 0;
        //virtual void copyTexture(Texture& src, Texture& dst, const CopyTextureParam* pParam, s32 num) = 0;
        //
        //virtual void updateBuffer(Buffer& buffer, const void* pData, size_t offset, size_t size) = 0;

        // void updateBuffer();

        virtual void insertResourceBarrier(const ResourceBarrier&) = 0;

#ifdef OB_DEBUG
        virtual void pushMarker(const char* pName) = 0;
        virtual void popMarker() = 0;
#endif
    protected:

        virtual ~CommandList() = default;

    };


    inline void CommandList::setVertexBuffer(const Ref<Buffer>& buffer) {
        StaticArray<const Ref<Buffer>*,1> buffers = { &buffer };
        setVertexBuffers(buffers);
    }

}// namespace pb::rhi