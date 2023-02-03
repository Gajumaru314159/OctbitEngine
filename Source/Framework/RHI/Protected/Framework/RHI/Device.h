//***********************************************************
//! @file
//! @brief		デバイス・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Misc/BlobView.h>
#include <Framework/Core/Utility/Ref.h>
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Types/RenderPassDesc.h>
#include <Framework/RHI/Types/FrameBufferDesc.h>
#include <Framework/RHI/Types/DisplayDesc.h>
#include <Framework/RHI/Types/CommandListDesc.h>
#include <Framework/RHI/Types/RootSignatureDesc.h>
#include <Framework/RHI/Types/PipelineStateDesc.h>
#include <Framework/RHI/Types/ShaderStage.h>
#include <Framework/RHI/Types/TextureDesc.h>
#include <Framework/RHI/Types/RenderTextureDesc.h>
#include <Framework/RHI/Types/BufferDesc.h>
#include <Framework/RHI/Types/DescriptorDesc.h>

//===============================================================
// クラス宣言
//===============================================================
namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      デバイス・インターフェイス
    //@―---------------------------------------------------------------------------
    class Device {
    public:

        static Device* Get();

    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  デストラクタ
        //@―---------------------------------------------------------------------------
        virtual ~Device() = default;


        //@―---------------------------------------------------------------------------
        //! @brief  妥当な状態か
        //@―---------------------------------------------------------------------------
        virtual bool isValid()const = 0;


        //===============================================================
        // 更新
        //===============================================================
        virtual void entryCommandList(const class CommandList&) = 0;

        virtual void update() = 0;


        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------

        virtual Ref<RenderPass>     createRenderPass(const RenderPassDesc& desc) = 0;
        virtual Ref<FrameBuffer>    createFrameBuffer(const FrameBufferDesc& desc) = 0;


        virtual Ref<Display>        createDisplay(const DisplayDesc& desc) = 0;

        virtual Ref<CommandList>    createCommandList(const CommandListDesc& desc) = 0;

        virtual Ref<RootSignature>  createRootSignature(const RootSignatureDesc& desc) = 0;
        virtual Ref<PipelineState>  createPipelineState(const PipelineStateDesc& desc) = 0;

        virtual Ref<Buffer>         createBuffer(const BufferDesc& desc) = 0;

        virtual Ref<Texture>        createTexture(const TextureDesc& desc) = 0;
        virtual Ref<Texture>        createTexture(BlobView blob,StringView name) = 0;
        virtual Ref<RenderTexture>  createRenderTexture(const RenderTextureDesc& desc) = 0;

        virtual Ref<Shader>         createShader(const String& code, ShaderStage stage) = 0;
        virtual Ref<Shader>         createShader(BlobView binary, ShaderStage stage) = 0;

        virtual Ref<DescriptorTable>createDescriptorTable(DescriptorHeapType type, s32 elementNum) = 0;

        //virtual IFence*           createFence(const FenceDesc&);

        //void changeSyncType(SyncType)=0;
        // virtual virtual bool isHdrSupported()const=0;
        // virtual bool isMultiDrawSupported()const=0;
        // virtual bool isAsyncComputeSupported()const=0;
        // virtual bool isRaytracingSupported()const=0;


    };


}// namespace pb::rhi