//***********************************************************
//! @file
//! @brief		RHI・モジュール
//! @author		Gajumaru
//***********************************************************
#pragma once
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
#include <Framework/Core/Misc/BlobView.h>
#include <Framework/Core/Utility/Ref.h>

namespace ob::rhi {

	//@―---------------------------------------------------------------------------
	//! @brief      RHI・モジュール
	//! 
	//! @details    RHIのモジュール基底です。
	//!             DirectXやVulkanなど、APIを共通化する場合はこのモジュール・インターフェースを実装して下さい。
    //@―---------------------------------------------------------------------------
	class RHI:public Singleton<RHI> {
	public:

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //@―---------------------------------------------------------------------------
		RHI(GraphicObjectManager&);

        //@―---------------------------------------------------------------------------
        //! @brief      デストラクタ
        //@―---------------------------------------------------------------------------
        virtual ~RHI();

		//@―---------------------------------------------------------------------------
		//! @brief      
		//@―---------------------------------------------------------------------------
        virtual void update();


        //@―---------------------------------------------------------------------------
        //! @brief  妥当な状態か
        //@―---------------------------------------------------------------------------
        virtual bool isValid()const { return true; };


        //===============================================================
        // 更新
        //===============================================================
        virtual void entryCommandList(const CommandList&) {}


        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------

        virtual Ref<RenderPass>     createRenderPass(const RenderPassDesc& desc) { return nullptr; }
        virtual Ref<FrameBuffer>    createFrameBuffer(const FrameBufferDesc& desc) { return nullptr; }


        virtual Ref<Display>        createDisplay(const DisplayDesc& desc) { return nullptr; }

        virtual Ref<CommandList>    createCommandList(const CommandListDesc& desc) { return nullptr; }

        virtual Ref<RootSignature>  createRootSignature(const RootSignatureDesc& desc) { return nullptr; }
        virtual Ref<PipelineState>  createPipelineState(const PipelineStateDesc& desc) { return nullptr; }

        virtual Ref<Buffer>         createBuffer(const BufferDesc& desc) { return nullptr; }

        virtual Ref<Texture>        createTexture(const TextureDesc& desc) { return nullptr; }
        virtual Ref<Texture>        createTexture(BlobView blob, StringView name) { return nullptr; }
        virtual Ref<Texture>        createTexture(Size size, Span<IntColor> colors) { return nullptr; }
        virtual Ref<RenderTexture>  createRenderTexture(const RenderTextureDesc& desc) { return nullptr; }

        virtual Ref<Shader>         compileShader(const String& code, ShaderStage stage) { return nullptr; }
        virtual Ref<Shader>         loadShader(BlobView binary, ShaderStage stage) { return nullptr; }

        virtual Ref<DescriptorTable>createDescriptorTable(DescriptorHeapType type, s32 elementNum) { return nullptr; }

        //virtual IFence*           createFence(const FenceDesc&);

        //void changeSyncType(SyncType)=0;
        // virtual virtual bool isHdrSupported()const=0;
        // virtual bool isMultiDrawSupported()const=0;
        // virtual bool isAsyncComputeSupported()const=0;
        // virtual bool isRaytracingSupported()const=0;

    protected:

        //@―---------------------------------------------------------------------------
        //! @brief      終了処理
        //! @details    GraphicObjectの解放が遅延する影響で派生クラスのデストラクタの最初で呼び出す必要があります。
        //@―---------------------------------------------------------------------------
        void finalize();

    private:
        GraphicObjectManager& m_objectManager;
	};

}