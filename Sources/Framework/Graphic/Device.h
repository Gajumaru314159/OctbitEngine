//***********************************************************
//! @file
//! @brief		スワップチェーン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/FeatureLevel.h>
#include <Framework/Graphic/Types/CommandListType.h>
#include <Framework/Graphic/Types/TextureDesc.h>
#include <Framework/Graphic/Types/BufferDesc.h>
#include <Framework/Graphic/Types/SwapchainDesc.h>

#include <Framework/Graphic/SwapChain.h>
#include <Framework/Graphic/Texture.h>


//===============================================================
// 前方宣言
//===============================================================
namespace ob::graphic {
    class IDevice;
}


//===============================================================
// クラス宣言
//===============================================================
namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      グラフィック・デバイス
    //@―---------------------------------------------------------------------------
    class Device{
        friend class System;
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        /*
        virtual void beginRender();
        virtual void entryCommandQeue(CommandList& commandList);
        virtual void endRender();
        virtual void flip();
        */

        void createSwapChain(SwapChain& display, SwapchainDesc& desc);

        /*
        virtual void createCommandList(CommandList& commandList, CommandListType type);

        virtual void createTexture(Texture& texture, const TextureData& data,StringView name);
        */
        void createTexture(Texture& texture, const TextureDesc& desc, StringView name);
        /*
        //virtual void createRenderTexture(Texture& texture, const TextureDesc& desc, StringView name);

        virtual void createBuffer(Buffer& buffer,const BufferDesc& desc,const virtual void* data,StringView name);

        virtual void createRootSignature(RootSignature& signature, const RootSignatureDesc& desc, StringView name);
        //virtual void createCommandSignature(CommandSignature& signature, const CommandSignatureDesc& desc, StringView name);

        virtual void createVertexShader(VertexShader& shader, const virtual void* data, s32 dataSize,StringView name);
        virtual void createPixelShader(Pixel& shader, const virtual void* data, s32 dataSize,StringView name);
        virtual void createGeometoryShader(GeometoryShader& shader, const virtual void* data, s32 dataSize,StringView name);
        virtual void createHullShader(HullShader& shader, const virtual void* data, s32 dataSize,StringView name);
        virtual void createDomainShader(DomainShader& shader, const virtual void* data, s32 dataSize,StringView name);
        virtual void createComputeShader(ComputeShader& shader, const virtual void* data, s32 dataSize,StringView name);
        virtual void createRaygenShader(RaygenShader& shader, const virtual void* data, s32 dataSize,StringView name);
        virtual void createClosestShader(ClosestShader& shader, const virtual void* data, s32 dataSize,StringView name);
        virtual void createAnyHitShader(AnHitShader& shader, const virtual void* data, s32 dataSize,StringView name);
        virtual void createIntersectionShader(IntersectionShader& shader, const virtual void* data, s32 dataSize,StringView name);
        virtual void createMissShader(MissShader& shader, const virtual void* data, s32 dataSize,StringView name);

        virtual void createPipelineState(PipelineState& state, const PipelineStateDesc& desc, StringView name);



        //void changeSyncType(SyncType);
        virtual virtual bool isHdrSupported()const;
        virtual bool isMultiDrawSupported()const;
        virtual bool isAsyncComputeSupported()const;
        virtual bool isRaytracingSupported()const;
        */
    private:
        IDevice* m_impl;
    };


}// namespace pb::graphic