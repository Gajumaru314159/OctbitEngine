//***********************************************************
//! @file
//! @brief		パイプラインステート実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/IPipelineState.h>
#include <Framework/Graphic/Types/PipelineStateDesc.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::graphic::dx12 {
    class DeviceImpl;
}


//===============================================================
// クラス定義
//===============================================================
namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------

    // シェーダパラメータ情報
    struct ShaderVariableDesc {
        //ShaderParamType type2;
        D3D_SHADER_VARIABLE_TYPE type;
        s32 registerNum;
        s32 offset;
        s32 rows;
        s32 columns;
        s32 elementCount;
    };
    struct ShaderTextureDesc {
        s32 registerNum;
        s32 dimension;
    };

    //@―---------------------------------------------------------------------------
    //! @brief		グラフィック・パイプライン実装(DirectX12)
    //@―---------------------------------------------------------------------------
    class PipelineStateImpl :public IPipelineState {
    public:
        static constexpr s32 REGISTER_MAX=8;
        using variable_map = map<String, ShaderVariableDesc, less<>>;
        using texture_map = map<String, ShaderTextureDesc, less<>>;
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

        RootSignature   m_rootSignature;
        ComPtr<ID3D12PipelineState> m_pipelineState;    //!< パイプラインステート

        vector<String> m_inputLayoutNames;              //!< 頂点レイアウト・セマンティクス
        vector<D3D12_INPUT_ELEMENT_DESC> m_inputLayout; //!< 頂点レイアウト

        variable_map m_varMap;                          //!< 変数マップ
        texture_map  m_texMap;                          //!< テクスチャマップ

        array<s32, REGISTER_MAX> m_cBufDataSizes;       //!< 定数バッファのレジスタごとのサイズ
        s32 m_texNums[REGISTER_MAX];                    //!< テクスチャのレジスタごとの枚数

        array<s32, REGISTER_MAX> m_cBufIndices;         //!< 定数バッファのインデックス
        array<s32, REGISTER_MAX> m_texIndices;          //!< 手クスやインデックス

        s32 m_targetNum;                                // レンダーターゲットの枚数
            
    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //! @endcond
}// namespcae ob::graphic::dx12