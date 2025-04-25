//***********************************************************
//! @file
//! @brief		RHI・モジュール
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Config.h>
#include <Framework/RHI/Types/DisplayDesc.h>
#include <Framework/RHI/Types/CommandListDesc.h>
#include <Framework/RHI/Types/RootSignatureDesc.h>
#include <Framework/RHI/Types/PipelineStateDesc.h>
#include <Framework/RHI/Types/ShaderStage.h>
#include <Framework/RHI/Types/TextureDesc.h>
#include <Framework/RHI/Types/RenderTextureDesc.h>
#include <Framework/RHI/Types/BufferDesc.h>
#include <Framework/RHI/Types/RenderPassDesc.h>
#include <Framework/RHI/GraphicFile.h>
#include <Framework/Core/Misc/BlobView.h>
#include <Framework/Core/Utility/Ref.h>

namespace ob::rhi {

	//! @brief      RHI・モジュール
	//! 
	//! @details    RHIのモジュール基底です。
	//!             DirectXやVulkanなど、APIを共通化する場合はこのモジュール・インターフェースを実装して下さい。
	class RHI:public Singleton<RHI> {
	public:

        //! @brief      コンストラクタ
		RHI(GraphicObjectManager&, RHIConfig*);

        //! @brief      デストラクタ
        virtual ~RHI();

		//! @brief      
        virtual void update();


        //! @brief  妥当な状態か
        virtual bool isValid()const { return true; };

        virtual String getAPIName()const { return {}; };

        //===============================================================
        // 更新
        //===============================================================
        virtual void entryCommandList(const Ref<CommandList>&) {}


        //! @brief  説明

        virtual Ref<Display>        createDisplay(const DisplayDesc& desc) { return nullptr; }

        virtual Ref<CommandList>    createCommandList(const CommandListDesc& desc) { return nullptr; }

        virtual Ref<RootSignature>  createRootSignature(const RootSignatureDesc& desc) { return nullptr; }
        virtual Ref<PipelineState>  createPipelineState(const PipelineStateDesc& desc) { return nullptr; }

        virtual Ref<Buffer>         createBuffer(const BufferDesc& desc) { return nullptr; }

        virtual Ref<Texture>        createTexture(const TextureDesc& desc) { return nullptr; }
        virtual Ref<Texture>        createTexture(StringView name,BlobView blob) { return nullptr; }
        virtual Ref<Texture>        createTexture(StringView name, TextureType type, Size size, Span<const IntColor> colors) { return nullptr; }
        virtual Ref<RenderTexture>  createRenderTexture(const RenderTextureDesc& desc) { return nullptr; }

        virtual Ref<Sampler>        createSampler(const SamplerDesc& desc) { return nullptr; }

        virtual Ref<Shader>         compileShader(const ShaderCompileDesc& desc) { return nullptr; }
        virtual Ref<Shader>         loadShader(BlobView binary, ShaderStage stage) { return nullptr; }

        virtual Ref<DescriptorTable>createDescriptorTable(const BindingSlot& desc) { return nullptr; }
        virtual Ref<DescriptorTable>createDescriptorTable(const Ref<RootSignature>& signature, s32 slot) { return nullptr; }

        virtual Ref<RenderPass>     createRenderPass(const RenderPassDesc& desc) { return nullptr; }

        //virtual IFence*           createFence(const FenceDesc&);

        //void changeSyncType(SyncType)=0;
        // virtual virtual bool isHdrSupported()const=0;
        // virtual bool isMultiDrawSupported()const=0;
        // virtual bool isAsyncComputeSupported()const=0;
        // virtual bool isRaytracingSupported()const=0;

        virtual Ref<GraphicFileHandle>  createGraphicFileHandle(StringView path) { return nullptr; }
        virtual Ref<GraphicFileEvent>   createGraphicFileEvent() { return nullptr; }
        virtual Ref<GraphicFileQueue>   createGraphicFileQueue(const GraphicFileQueueDesc&) { return nullptr; }

        virtual bool                    generateGraphicFile(StringView input, StringView output, s32 compressionLevel) { return false; }
        virtual Vector<GraphicFileMipInfo> prepareGraphicFile(StringView path) { return {}; }

		const RHIConfig& getConfig()const { return m_config; }

    public:

        virtual bool supports(TextureFormat format, TextureType type)const { return false; }
        virtual bool supportsForRenderTexture(TextureFormat format)const { return false; }
        virtual bool supports(ShaderStage stage)const { return false; }

    protected:

        //! @brief      終了処理
        //! @details    GraphicObjectの解放が遅延する影響で派生クラスのデストラクタの最初で呼び出す必要があります。
        void finalize();

    private:
        RHIConfig m_config;
        GraphicObjectManager& m_objectManager;
	};

}