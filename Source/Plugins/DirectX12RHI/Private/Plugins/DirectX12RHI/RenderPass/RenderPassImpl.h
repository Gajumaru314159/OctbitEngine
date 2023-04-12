﻿//***********************************************************
//! @file
//! @brief		レンダーパス実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RenderPass.h>

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
    //! @brief  レンダーパス(DirectX12)
    //@―---------------------------------------------------------------------------
    class RenderPassImpl :public rhi::RenderPass {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        RenderPassImpl(DirectX12RHI& rDevice, const RenderPassDesc& desc);


        //@―---------------------------------------------------------------------------
        //! @brief  デストラクタ
        //@―---------------------------------------------------------------------------
        ~RenderPassImpl();


        //@―---------------------------------------------------------------------------
        //! @brief  妥当なオブジェクトか
        //@―---------------------------------------------------------------------------
        bool isValid()const override;


        //@―---------------------------------------------------------------------------
        //! @brief      名前を取得
        //@―---------------------------------------------------------------------------
        const String& getName()const override;

        const RenderPassDesc& desc()const override;

    private:
        const RenderPassDesc m_desc;
    };

}