//***********************************************************
//! @file
//! @brief		デバイス・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/SwapchainDesc.h>
#include <Framework/Graphic/Types/RenderPassDesc.h>
#include <Framework/Graphic/Types/FrameBufferDesc.h>
#include <Framework/Graphic/Types/RootSignatureDesc.h>
#include <Framework/Graphic/Types/PipelineStateDesc.h>
#include <Framework/Graphic/Types/ShaderStage.h>
#include <Framework/Graphic/Types/TextureDesc.h>
#include <Framework/Graphic/Types/BufferDesc.h>
#include <Framework/Graphic/Types/DescriptorDesc.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::graphic {
    class ISwapChain;
    class IRenderPass;
    class IFrameBuffer;
    class IRootSignature;
    class IPipelineState;
    class ITexture;
    class IRenderTexture;
    class IShader;
    class IBuffer;
    class IDescriptorTable;
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
        //! @brief  デストラクタ
        //@―---------------------------------------------------------------------------
        virtual ~IDevice() = default;


        //@―---------------------------------------------------------------------------
        //! @brief  妥当な状態か
        //@―---------------------------------------------------------------------------
        virtual bool isValid()const = 0;


        //===============================================================
        // 更新
        //===============================================================
        virtual void update() = 0;

        //virtual ICommandQueue* getCommandQueue()const = 0;
        //virtual Result        waitIdle()=0;
        //virtual ICommandAllocator* createCommanndAllocator();
        //virtual ICommandList* createCommandList();// アロケータ/タイプを指定して生成
        //virtual IFence*   createFence(const FenceDesc&);

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        /*
        virtual void beginRender()=0;
        virtual void entryCommandQeue(CommandList& commandList)=0;
        virtual void endRender()=0;
        virtual void flip()=0;
        */

        virtual ISwapChain*     createSwapChain(const SwapchainDesc& desc)=0;

        virtual IRenderPass*    createRenderPass(const RenderPassDesc& desc) = 0;
        virtual IFrameBuffer*   createFrameBuffer(const FrameBufferDesc& desc) = 0;

        virtual IRootSignature* createRootSignature(const RootSignatureDesc& desc) = 0;
        virtual IPipelineState* createPipelineState(const PipelineStateDesc& desc) = 0;

        virtual IBuffer*        createBuffer(const BufferDesc& desc) = 0;

        virtual ITexture*       createTexture(const TextureDesc& desc)=0;
        virtual IRenderTexture* createRenderTexture(const gsl::span<TextureDesc> targets, const TextureDesc& depth)=0;
        
        virtual IShader*        createShader(const String& code,ShaderStage stage) = 0;
        virtual IShader*        createShader(const Blob& binary, ShaderStage stage) = 0;


        virtual IDescriptorTable* createDescriptorTable(DescriptorHeapType type,s32 elementNum) = 0;

        /*
        virtual void createCommandList(CommandList& commandList, CommandListType type)=0;
        virtual void createTexture(Texture& texture, const TextureData& data,StringView name)=0;
        */
        /*

        
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


    };


}// namespace pb::graphic