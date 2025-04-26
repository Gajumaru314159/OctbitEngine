//***********************************************************
//! @file
//! @brief		ルートシグネチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RootSignature.h>
#include <Framework/RHI/Forward.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::rhi::dx12 {
    class DirectX12RHI;
    class ITexture;
}


//===============================================================
// クラス宣言
//===============================================================
namespace ob::rhi::dx12 {

    //! @brief  ルートシグネチャ実装(DirectX12)
    class DirectX12RootSignature :public rhi::RootSignature {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //! @brief  コンストラクタ
        DirectX12RootSignature(DirectX12RHI& rDevice, const RootSignatureDesc& desc);


        //! @brief  デストラクタ
        ~DirectX12RootSignature();


        //! @brief  妥当なオブジェクトか
        bool isValid()const;

        
        //! @brief      名前を取得
        const String& getName()const override;


        //===============================================================
        // ゲッター
        //===============================================================

        //! @brief  定義を取得
        const RootSignatureDesc& getDesc()const noexcept override;


        //! @brief  ネイティブオブジェクトを取得
        auto getNative()const noexcept { return m_rootSignature.Get(); };

    private:
        RootSignatureDesc m_desc;

        ComPtr<ID3D12RootSignature> m_rootSignature;    //!< ルートシグネチャ

    };

}