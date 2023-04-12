﻿//***********************************************************
//! @file
//! @brief		レンダーパス実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/FrameBuffer.h>

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
    class FrameBufferImpl :public rhi::FrameBuffer{
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        FrameBufferImpl(DirectX12RHI& rDevice, const FrameBufferDesc& desc);


        //@―---------------------------------------------------------------------------
        //! @brief  デストラクタ
        //@―---------------------------------------------------------------------------
        ~FrameBufferImpl();


        //@―---------------------------------------------------------------------------
        //! @brief  妥当なオブジェクトか
        //@―---------------------------------------------------------------------------
        bool isValid()const override;


        //@―---------------------------------------------------------------------------
        //! @brief      名前を取得
        //@―---------------------------------------------------------------------------
        const String& getName()const override;


        //@―---------------------------------------------------------------------------
        //! @brief      定義取得
        //@―---------------------------------------------------------------------------
        const FrameBufferDesc& desc()const override { return m_desc; }

    private:
        const FrameBufferDesc m_desc;
    };

}