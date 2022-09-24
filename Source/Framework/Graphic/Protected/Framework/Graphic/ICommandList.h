//***********************************************************
//! @file
//! @brief		コマンドリスト・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/GraphicObject.h>
#include <Framework/Core/Geometory/Viewport.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::graphic {
    class SwapChain;
    class VertexBuffer;
    class IndexBuffer;
    class RootSignature;
    class PipelineState;
    class ResourceBarrier;
    class Buffer;
    struct SetDescriptorTableParam;
    struct DrawParam;
    struct DrawIndexedParam;
}

//===============================================================
// クラス宣言
//===============================================================
namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      コマンドリスト・インターフェイス
    //@―---------------------------------------------------------------------------
    class ICommandList :public GraphicObject {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  妥当な状態か
        //@―---------------------------------------------------------------------------
        virtual bool isValid()const = 0;

        virtual void begin()=0;
        virtual void end()=0;

        virtual void applySwapChain(const class SwapChain& swapChain, const class Texture& texture)=0;
        virtual void beginRender(const class RenderTarget& target) = 0; //!< 描画開始
        virtual void endRender() = 0;                                   //!< 描画終了

        virtual void setViewport(const Viewport* pViewport,s32 num)=0;  //!< ビューポートを設定
        virtual void setScissorRect(const IntRect* pRect,s32 num)=0;    //!< シザー矩形を設定
        
        virtual void clearColors(u32 mask) = 0;                         //!< カラーをクリア
        virtual void clearDepthStencil() = 0;                           //!< 深度値をクリア

        virtual void setVertexBuffers(Span<const Buffer*>) = 0;         //!< 頂点バッファ設定
        virtual void setIndexBuffer(const Buffer&) = 0;                 //!< インデックスバッファ設定


        virtual void setRootSignature(const RootSignature&) = 0;// TODO 削除
        virtual void setPipelineState(const PipelineState&) = 0;        //!< パイプライン設定
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

        virtual ~ICommandList() = default;

    };

}// namespace pb::graphic