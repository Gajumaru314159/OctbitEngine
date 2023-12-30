//***********************************************************
//! @file
//! @brief		コマンドリスト
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Geometry/Viewport.h>
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/CommandListDesc.h>
#include <Framework/RHI/Types/CommandParam.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      コマンドリスト
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
        virtual void setVertexBuffers(Span<Ref<Buffer>>) = 0;         //!< 頂点バッファ設定
        virtual void setIndexBuffer(const Ref<Buffer>&) = 0;                 //!< インデックスバッファ設定


        virtual void setPipelineState(const Ref<PipelineState>&) = 0;        //!< パイプライン設定
        //virtual void setComputePipelineState(const ComputePipelineState&) = 0;

        // setBlendFactor
        // setStencilRef

        virtual void setRootDesciptorTable(const SetDescriptorTableParam*, s32 num) = 0;
        // void setRootDescriptor();
        virtual void setRootConstant(const SetRootConstantsParam&) = 0;


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
        virtual void pushMarker(StringView name) = 0;
        virtual void popMarker() = 0;
#endif

    };


    inline void CommandList::setVertexBuffer(const Ref<Buffer>& buffer) {
        Ref<Buffer> buffers[1];
        buffers[0] = buffer;
        setVertexBuffers(buffers);
    }



#if 0
    enum class TextureAspect {
        Color,
        Depth,
        Stencil,
    };

    struct TextureSubresource {
        u16 mipSlice = 0;
        u16 arraySlice = 0;
        TextureAspect aspect;
    };


    struct CopyBufferParam {
        Ref<Buffer> source;
        Ref<Buffer> target;
        s32 sourceOffset = 0;
        s32 targetOffset = 0;
        s32 size = 0;
    };
    struct CopyTextureParam {
        Ref<Texture> source;
        Ref<Texture> target;
        TextureSubresource sourceSubresource;
        TextureSubresource targetSubresource;
        Point sourceOrigin;
        Point targetOrigin;
        Size size;
    };
    struct CopyBufferToTextureParam {};
    struct CopyTextureToBufferParam {};

    using CopyItemParam = Variant<CopyBufferParam, CopyTextureParam, CopyBufferToTextureParam, CopyTextureToBufferParam>;


    struct DrawItemParam {};
    struct DispatchItemParam {};


    class CommandListSimple {
    public:
        virtual void setViewports(const Viewport* viewports, s32 num) = 0;  //!< ビューポートを設定
        virtual void setScissors(const IntRect* scissors, s32 num) = 0;    //!< シザー矩形を設定
                void setViewport(const Viewport& viewport) { setViewports(&viewport, 1); }
                void setScissors(const IntRect& scissor) { setScissors(&scissor, 1); }

        virtual void submit(const CopyItemParam& param) = 0;
        virtual void submit(const DrawItemParam& param) = 0;
        virtual void submit(const DispatchItemParam& param) = 0;
        virtual void submit(const DispatchItemParam& param) = 0;
    };
#endif

}