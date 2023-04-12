﻿//***********************************************************
//! @file
//! @brief		ルートシグネチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RootSignature.h>

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

    //@―---------------------------------------------------------------------------
    //! @brief  ルートシグネチャ実装(DirectX12)
    //@―---------------------------------------------------------------------------
    class RootSignatureImpl :public rhi::RootSignature {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        RootSignatureImpl(DirectX12RHI& rDevice, const RootSignatureDesc& desc);


        //@―---------------------------------------------------------------------------
        //! @brief  デストラクタ
        //@―---------------------------------------------------------------------------
        ~RootSignatureImpl();


        //@―---------------------------------------------------------------------------
        //! @brief  妥当なオブジェクトか
        //@―---------------------------------------------------------------------------
        bool isValid()const override;

        
        //@―---------------------------------------------------------------------------
        //! @brief      名前を取得
        //@―---------------------------------------------------------------------------
        const String& getName()const override;


        //===============================================================
        // ゲッター
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  定義を取得
        //@―---------------------------------------------------------------------------
        const RootSignatureDesc& getDesc()const noexcept override;


        //@―---------------------------------------------------------------------------
        //! @brief  ネイティブオブジェクトを取得
        //@―---------------------------------------------------------------------------
        auto getNative()const noexcept { return m_rootSignature.Get(); };


    private:

        const RootSignatureDesc m_desc;

        ComPtr<ID3D12RootSignature> m_rootSignature;    //!< ルートシグネチャ

    };

}