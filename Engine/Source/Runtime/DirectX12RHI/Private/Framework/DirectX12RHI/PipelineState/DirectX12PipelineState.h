//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/PipelineState.h>
#include <Framework/RHI/Types/PipelineStateDesc.h>
#include <Framework/DirectX12RHI/RootSignature/DirectX12RootSignature.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::rhi {
    class DirectX12Device;
}


//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi {

    //! @brief		グラフィック・パイプライン実装(DirectX12)
    class DirectX12PipelineState :public PipelineState {
    public:

        //! @brief		コンストラクタ
        DirectX12PipelineState(DirectX12Device&, const PipelineStateDesc& desc);


        //! @brief  妥当な状態か
        bool isValid()const { return m_pipelineState != nullptr; }


        //! @brief      名前を取得
        const String& getName()const override;


        //! @brief      定義を取得
        //const PipelineStateDesc& getDesc()const override { return m_desc; };


        //! @brief      ネイティブオブジェクトを取得
        ID3D12PipelineState* getNative()const noexcept { return m_pipelineState.Get(); }

        DirectX12RootSignature* getRootSignature()const { 
            if (auto p = m_desc.rootSignature.cast<DirectX12RootSignature>()) {
                return p;
            }
            return nullptr;        
        }

    private:
        
        static void SetupFormats(D3D12_GRAPHICS_PIPELINE_STATE_DESC& dst, const PipelineStateDesc& src);
        static void SetupBlend(D3D12_BLEND_DESC& dst, const PipelineStateDesc& src);
        static void SetupRenderTargetBlend(D3D12_RENDER_TARGET_BLEND_DESC& dst, const BlendDesc& src);
        static void SetupVertexLayout(D3D12_INPUT_ELEMENT_DESC& dst, const VertexAttribute& src);
        static void SetupRasterizerState(D3D12_RASTERIZER_DESC& dst, const RasterizerDesc& src);
        static void SetupDepthStencilState(D3D12_DEPTH_STENCIL_DESC& dst, const DepthStencilDesc& src);


    private:

        const PipelineStateDesc m_desc;
        ComPtr<ID3D12PipelineState> m_pipelineState;    //!< パイプラインステート
            
    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //! @endcond
}