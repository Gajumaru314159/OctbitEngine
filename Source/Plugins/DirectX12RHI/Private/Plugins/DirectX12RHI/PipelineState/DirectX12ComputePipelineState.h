//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/ComputePipelineState.h>
#include <Framework/RHI/Types/ComputePipelineStateDesc.h>
#include <Plugins/DirectX12RHI/RootSignature/DirectX12RootSignature.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::rhi {
    class DirectX12RHI;
}


//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi {

    //! @brief		コンピュート・パイプライン実装(DirectX12)
    class DirectX12ComputePipelineState :public ComputePipelineState {
    public:

        //! @brief		コンストラクタ
        DirectX12ComputePipelineState(DirectX12RHI&, const ComputePipelineStateDesc& desc);


        //! @brief  妥当な状態か
        bool isValid()const { return m_pipelineState != nullptr; }


        //! @brief      名前を取得
        const String& getName()const override;


        //! @brief      ネイティブオブジェクトを取得
        ID3D12PipelineState* getNative()const noexcept { return m_pipelineState.Get(); }

        DirectX12RootSignature* getRootSignature()const { 
            if (auto p = m_desc.rootSignature.cast<DirectX12RootSignature>()) {
                return p;
            }
            return nullptr;        
        }

    private:

        ComputePipelineStateDesc m_desc;
        ComPtr<ID3D12PipelineState> m_pipelineState;    //!< パイプラインステート
            
    };




    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //! @endcond
}