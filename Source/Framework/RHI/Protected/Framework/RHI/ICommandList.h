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
#include <Framework/RHI/RenderTexture.h>

namespace ob::rhi {

    //! @brief      コマンドリスト
    class ICommandList :public GraphicObject {
    public:

        virtual void begin() = 0;
        virtual void end() = 0;
        virtual void flush() = 0;

        virtual void setRenderTargets(const RenderTextureArray& colors, const RenderTexture& depth = {}) = 0; //!< レンダーパス開始

        virtual void applyDisplay(const Display& display, const RenderTexture& texture) = 0;

        virtual void setViewport(const Viewport* pViewport, s32 num) = 0;  //!< ビューポートを設定
        virtual void setScissorRect(const IntRect* pRect, s32 num) = 0;    //!< シザー矩形を設定

        virtual void clearColors(u32 mask = -1) = 0;                         //!< カラーをクリア
        virtual void clearDepthStencil() = 0;                           //!< 深度値をクリア

        virtual void setVertexBuffers(Span<Buffer>) = 0;         //!< 頂点バッファ設定
        virtual void setIndexBuffer(const Buffer&) = 0;                 //!< インデックスバッファ設定


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

        virtual void pushMarker(StringView name) = 0;
        virtual void popMarker() = 0;

        // virtual void setViewports(const Viewport* pViewport, s32 num) = 0;  //!< ビューポートを設定
        // virtual void setScissors(const IntRect* pRect, s32 num) = 0;    //!< シザー矩形を設定
        // virtual void setShaderResourceGroup(const ShaderResourceGroup&);
        // virtual void submit(const CopyItem&);
        // virtual void submit(const DrawItem&);
        // virtual void submit(const DispatchItem&);
        // virtual void submit(const CopyItem&);

    };

}