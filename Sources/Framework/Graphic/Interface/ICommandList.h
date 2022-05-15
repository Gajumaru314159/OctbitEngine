//***********************************************************
//! @file
//! @brief		コマンドリスト・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/GraphicObject.h>
#include <Framework/Core/Geometory/Viewport.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::graphic {
    class VertexBuffer;
    class IndexBuffer;
    class RootSignature;
    class PipelineState;
    class ResourceBarrier;
    class Buffer;
    struct SetDescriptorTableParam;
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
        virtual void beginRender(const class RenderTarget& target) = 0;
        virtual void endRender() = 0;
        // TODO endRender

        virtual void setViewport(const Viewport* pViewport,s32 num)=0;
        virtual void setScissorRect(const IntRect* pRect,s32 num)=0;
        
        virtual void clearColors(u32 mask) = 0;
        virtual void clearDepthStencil() = 0;

        virtual void setVertexBuffer(const Buffer&) = 0;
        virtual void setIndexBuffer(const Buffer&) = 0;


        virtual void setRootSignature(const RootSignature&) = 0;// TODO 削除
        virtual void setPipelineState(const PipelineState&) = 0;
        //virtual void setComputePipelineState(const ComputePipelineState&) = 0;

        // setBlendFactor
        // setStencilRef

        virtual void setRootDesciptorTable(const SetDescriptorTableParam*, s32 num) = 0;
        // void setRootDescriptor();
        // void setRootConstant();


        virtual void drawIndexed() = 0;
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