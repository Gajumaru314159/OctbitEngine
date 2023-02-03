//***********************************************************
//! @file
//! @brief		パイプラインステート実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/PipelineState.h>
#include <Framework/RHI/Types/PipelineStateDesc.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::rhi::dx12 {
    class DeviceImpl;
}


//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief		グラフィック・パイプライン実装(DirectX12)
    //@―---------------------------------------------------------------------------
    class PipelineStateImpl :public PipelineState {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief		コンストラクタ
        //@―---------------------------------------------------------------------------
        PipelineStateImpl(DeviceImpl&, const PipelineStateDesc& desc);


        //@―---------------------------------------------------------------------------
        //! @brief  妥当な状態か
        //@―---------------------------------------------------------------------------
        bool isValid()const { return m_pipelineState != nullptr; }


        //@―---------------------------------------------------------------------------
        //! @brief      名前を取得
        //@―---------------------------------------------------------------------------
        const String& getName()const override;


        //@―---------------------------------------------------------------------------
        //! @brief      定義を取得
        //@―---------------------------------------------------------------------------
        //const PipelineStateDesc& getDesc()const override { return m_desc; };


        //@―---------------------------------------------------------------------------
        //! @brief      ネイティブオブジェクトを取得
        //@―---------------------------------------------------------------------------
        ID3D12PipelineState* getNative()const noexcept { return m_pipelineState.Get(); }


    private:
        
        void setupFormats(D3D12_GRAPHICS_PIPELINE_STATE_DESC& dst, const PipelineStateDesc& src);
        void setupBlend(D3D12_BLEND_DESC& dst, const PipelineStateDesc& src);
        void setupRenderTargetBlend(D3D12_RENDER_TARGET_BLEND_DESC& dst, const BlendDesc& src);
        void setupVertexLayout(D3D12_INPUT_ELEMENT_DESC& dst, const VertexAttribute& src);
        void setupRasterizerState(D3D12_RASTERIZER_DESC& dst, const RasterizerDesc& src);
        void setupDepthStencilState(D3D12_DEPTH_STENCIL_DESC& dst, const DepthStencilDesc& src);


    private:

        const PipelineStateDesc m_desc;

        Ref<RootSignature>   m_rootSignature;
        ComPtr<ID3D12PipelineState> m_pipelineState;    //!< パイプラインステート
            
    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //! @endcond
}// namespcae ob::rhi::dx12