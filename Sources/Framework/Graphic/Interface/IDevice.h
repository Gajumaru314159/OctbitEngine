//***********************************************************
//! @file
//! @brief		デバイス・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/FeatureLevel.h>
#include <Framework/Graphic/Types/CommandListType.h>
#include <Framework/Graphic/Types/TextureDesc.h>
#include <Framework/Graphic/Types/BufferDesc.h>
#include <Framework/Graphic/Types/SwapchainDesc.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::graphic {
    class ISwapChain;
    class ITexture;
    class IRenderTexture;
}


//===============================================================
// クラス宣言
//===============================================================
namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      デバイス・インターフェイス
    //@―---------------------------------------------------------------------------
    class IDevice{
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        /*
        virtual void beginRender()=0;
        virtual void entryCommandQeue(CommandList& commandList)=0;
        virtual void endRender()=0;
        virtual void flip()=0;
        */

        virtual ISwapChain* createSwapChain(SwapchainDesc& desc)=0;

        /*
        virtual void createCommandList(CommandList& commandList, CommandListType type)=0;
        virtual void createTexture(Texture& texture, const TextureData& data,StringView name)=0;
        */
        virtual ITexture* createTexture(const TextureDesc& desc, StringView name)=0;
        virtual IRenderTexture* createRenderTexture(const gsl::span<TextureDesc> targets, const TextureDesc& depth, StringView name)=0;
        /*
        virtual void createBuffer(Buffer& buffer,const BufferDesc& desc,const virtual void* data,StringView name)=0;

        virtual void createRootSignature(RootSignature& signature, const RootSignatureDesc& desc, StringView name)=0;
        //virtual void createCommandSignature(CommandSignature& signature, const CommandSignatureDesc& desc, StringView name)=0;

        virtual void createVertexShader(VertexShader& shader, const virtual void* data, s32 dataSize,StringView name)=0;
        virtual void createPixelShader(Pixel& shader, const virtual void* data, s32 dataSize,StringView name)=0;
        virtual void createGeometoryShader(GeometoryShader& shader, const virtual void* data, s32 dataSize,StringView name)=0;
        virtual void createHullShader(HullShader& shader, const virtual void* data, s32 dataSize,StringView name)=0;
        virtual void createDomainShader(DomainShader& shader, const virtual void* data, s32 dataSize,StringView name)=0;
        virtual void createComputeShader(ComputeShader& shader, const virtual void* data, s32 dataSize,StringView name)=0;
        virtual void createRaygenShader(RaygenShader& shader, const virtual void* data, s32 dataSize,StringView name)=0;
        virtual void createClosestShader(ClosestShader& shader, const virtual void* data, s32 dataSize,StringView name)=0;
        virtual void createAnyHitShader(AnHitShader& shader, const virtual void* data, s32 dataSize,StringView name)=0;
        virtual void createIntersectionShader(IntersectionShader& shader, const virtual void* data, s32 dataSize,StringView name)=0;
        virtual void createMissShader(MissShader& shader, const virtual void* data, s32 dataSize,StringView name)=0;

        virtual void createPipelineState(PipelineState& state, const PipelineStateDesc& desc, StringView name)=0;



        //void changeSyncType(SyncType)=0;
        virtual virtual bool isHdrSupported()const=0;
        virtual bool isMultiDrawSupported()const=0;
        virtual bool isAsyncComputeSupported()const=0;
        virtual bool isRaytracingSupported()const=0;
        */

    protected:

        ~IDevice() = default;

    };


}// namespace pb::graphic